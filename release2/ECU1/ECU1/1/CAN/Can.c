#define PART_TM4C123GH6PM
#include "Can.h"
#include <stdbool.h>
#include <stdint.h>
#include "inc/hw_can.h"
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/can.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"
#include "Can.h"
#include "../RGB/RGB.h"
#include "../OS/OS.h"
/**********************************
 * ******** EXTERNS **************
*********************************/
extern void APP_InterruptBased_GetCANmsg(uint32_t recevied_data);
extern uint32_t getKnownVoltage;
extern Packet PacketToPc ;
extern IRQ_Handling mainIRQs[MAX_IRQ_HANDLING] ; 
/**********************************/
tCANMsgObject tx_msg;
uint8_t tx_data;
tCANMsgObject rx_msg;
uint8_t rx_data;
tCANMsgObject tx_volt_msg;
uint8_t tx_volt_data;
tCANMsgObject rx_volt_msg;
uint8_t rx_volt_data;
uint8_t sending_flag;
/**********************************/
volatile uint32_t TX_ui32MsgCount = 0;
volatile uint32_t RX_ui32MsgCount = 0;
volatile bool g_bRXFlag = 0;
volatile bool TX_ErrFlag = 0;
volatile bool RX_ErrFlag = 0;
void Can_Driv_init()
{
    //[1]--------GPIO PortB configuration-------------
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
    GPIOPinConfigure(GPIO_PB4_CAN0RX);
    GPIOPinConfigure(GPIO_PB5_CAN0TX);
    GPIOPinTypeCAN(GPIO_PORTB_BASE, GPIO_PIN_4 | GPIO_PIN_5);
    //[2]--------Can 0 init---------- 
    SysCtlPeripheralEnable(SYSCTL_PERIPH_CAN0);
    CANInit(CAN0_BASE);
    CANBitRateSet(CAN0_BASE, SysCtlClockGet(), 500000);
    CANIntEnable(CAN0_BASE, CAN_INT_MASTER | CAN_INT_ERROR | CAN_INT_STATUS);
    IntEnable(INT_CAN0);
    CANEnable(CAN0_BASE);
}
void Can_tx_msg_config(uint8_t ID, uint8_t msg_size)
{
    tx_msg.ui32MsgID = ID;
    tx_msg.ui32MsgIDMask=0;
    tx_msg.ui32Flags = MSG_OBJ_TX_INT_ENABLE;
    tx_msg.ui32MsgLen = msg_size;
    tx_msg.pui8MsgData = &tx_data;
}
void Can_tx_check_voltage_config()
{
    tx_volt_msg.ui32MsgID = 0x14;
    tx_volt_msg.ui32MsgIDMask=0;
    tx_volt_msg.ui32Flags = MSG_OBJ_TX_INT_ENABLE;
    tx_volt_msg.ui32MsgLen = 8;
    tx_volt_msg.pui8MsgData = (uint8_t*)&tx_volt_data;
}
void Can_rx_msg_config(uint8_t ID, uint8_t msg_size)
{
    rx_msg.ui32MsgID = ID;
    rx_msg.ui32MsgIDMask=0;
    rx_msg.ui32Flags = MSG_OBJ_RX_INT_ENABLE | MSG_OBJ_USE_ID_FILTER;
    rx_msg.ui32MsgLen = msg_size;
    rx_msg.pui8MsgData = (uint8_t*)&rx_data;
	  CANMessageSet(CAN0_BASE, 1, &rx_msg, MSG_OBJ_TYPE_RX);
}
void Can_rx_check_voltage()
{
    rx_volt_msg.ui32MsgID = 0x14;
    rx_volt_msg.ui32MsgIDMask=0;
    rx_volt_msg.ui32Flags = MSG_OBJ_RX_INT_ENABLE | MSG_OBJ_USE_ID_FILTER;
    rx_volt_msg.ui32MsgLen = 8;
    rx_volt_msg.pui8MsgData = (uint8_t*)&rx_volt_data;
	  CANMessageSet(CAN0_BASE, 2, &rx_volt_msg, MSG_OBJ_TYPE_RX);
}
void can_send_msg(uint8_t data)
{
		tx_data = data;
    CANMessageSet(CAN0_BASE, 1, &tx_msg, MSG_OBJ_TYPE_TX);
    sending_flag = 1;
}
void can_volt_msg(uint8_t data)
{
		tx_volt_data = data;
    CANMessageSet(CAN0_BASE, 1, &tx_msg, MSG_OBJ_TYPE_TX);
    sending_flag = 2;
}

uint32_t can_recevie_msg_Async()
{
    if(g_bRXFlag)
    {
        CANMessageGet(CAN0_BASE, 1, &rx_msg, 0);
        return rx_data; 
    }
    else 
        return 0;
}
uint32_t can_recevie_volt_Async()
{
    if(g_bRXFlag)
    {
        CANMessageGet(CAN0_BASE, 2, &rx_volt_msg, 0);
        return rx_volt_data; 
    }
    else 
        return 0;
}

void CAN0_Handler(void)
{
    if (sending_flag ==1)
    {
        uint32_t ui32Status;
        ui32Status = CANIntStatus(CAN0_BASE, CAN_INT_STS_CAUSE);
        if(ui32Status == CAN_INT_INTID_STATUS)
        {
            ui32Status = CANStatusGet(CAN0_BASE, CAN_STS_CONTROL);
            TX_ErrFlag = 1;
        }
        else if(ui32Status == 1)
        {
            CANIntClear(CAN0_BASE, 1);
            TX_ui32MsgCount++;
            TX_ErrFlag = 0;
        }
        sending_flag = 0;
    }
    else if (sending_flag == 0)
    /*   Recieving message  */
    {
        /*---- Check if there's an error or not -----*/
        uint32_t ui32Status;
        ui32Status = CANIntStatus(CAN0_BASE, CAN_INT_STS_CAUSE);
        if(ui32Status == CAN_INT_INTID_STATUS && CANStatusGet(CAN0_BASE, CAN_STS_CONTROL)!=0x10)
        {
            ui32Status = CANStatusGet(CAN0_BASE, CAN_STS_CONTROL);
            RX_ErrFlag = 1;
        }
        else
        {
            CANIntClear(CAN0_BASE, 1);
            RX_ui32MsgCount++;
            g_bRXFlag = 1;
            RX_ErrFlag = 0;
            uint32_t recieved_data = can_recevie_msg_Async();
            APP_InterruptBased_GetCANmsg(recieved_data);
        }
    }
		else 
			/*  need to wait until receving volt */
		{
			/*
			while(1)
			{
				CANIntClear(CAN0_BASE, 1);
				RGB_RED();
				int counter = 0;
				while(counter < 4000)
				{
						Can_tx_msg_config(0x13,8);
						can_send_msg('c');
						counter++;
				}

				PacketToPc.copySystemState = GET_VOLTAGE_ERROR ; 
				APP_SendSystemState();
				Can_rx_msg_config(0x13,8);
				uint32_t recieved_volt = can_recevie_msg_Async();
				if(recieved_volt > 0)
				{
					RGB_OFF();
					getKnownVoltage = recieved_volt ;
					CANIntClear(CAN0_BASE, 1);
					mainIRQs[IRQ_COMM_LOST]=0;
					mainIRQs[IRQ_ALIVE_ACK]=0;
					break;
				}
			}*/
		}
}
