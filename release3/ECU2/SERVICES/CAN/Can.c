#include "../../MCAL/MCAL.h"
#include "Can.h"
#include "../../HAL/Led.h"
/**********************************/
tCANMsgObject tx_msg;
uint8_t tx_data;
tCANMsgObject rx_msg;
uint8_t rx_data;
uint8_t sending_flag;
uint32_t cut_off_counter = 0 ;
extern uint8_t sending_volt_flag;
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
void Can_rx_msg_config(uint8_t ID, uint8_t msg_size)
{
    rx_msg.ui32MsgID = ID;
    rx_msg.ui32MsgIDMask=0;
    rx_msg.ui32Flags = MSG_OBJ_RX_INT_ENABLE | MSG_OBJ_USE_ID_FILTER;
    rx_msg.ui32MsgLen = msg_size;
    rx_msg.pui8MsgData = (uint8_t*)&rx_data;
	  CANMessageSet(CAN0_BASE, 1, &rx_msg, MSG_OBJ_TYPE_RX);
}
void can_send_msg(uint8_t data)
{
		tx_data = data;
    CANMessageSet(CAN0_BASE, 1, &tx_msg, MSG_OBJ_TYPE_TX);
    sending_flag = 1;
}

uint32_t can_recevie_msg_sync()
{
	  IntMasterDisable();
    while(CANStatusGet(CAN0_BASE, CAN_STS_CONTROL)!=0x10){}
    CANMessageGet(CAN0_BASE, 1, &rx_msg, 0);
		IntMasterEnable();
    return rx_data; 
}
uint32_t can_recevie_msg_Async()
{
    if(g_bRXFlag)
    {
				uint32_t msg_rec_temp;
        CANMessageGet(CAN0_BASE, 1, &rx_msg, 0);
				msg_rec_temp = rx_data;
				rx_data = 0;
        return msg_rec_temp; 
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
    else
    {
        uint32_t ui32Status;
        ui32Status = CANIntStatus(CAN0_BASE, CAN_INT_STS_CAUSE);
        if(ui32Status == CAN_INT_INTID_STATUS && CANStatusGet(CAN0_BASE, CAN_STS_CONTROL)!=0x10)
        {
            ui32Status = CANStatusGet(CAN0_BASE, CAN_STS_CONTROL);
            RX_ErrFlag = 1;
        }
        else
        {
						uint32_t recieved_msg = 0;
            CANIntClear(CAN0_BASE, 1);
            RX_ui32MsgCount++;
            g_bRXFlag = 1;
            RX_ErrFlag = 0;
						recieved_msg= can_recevie_msg_Async();
						if (recieved_msg == '^')
						{
							LED_voidSetGreen();
						}
						else if( recieved_msg == 'c')
						{
							sending_volt_flag = 1;
							LED_voidSetBlue();
						}
						else 
						{
							LED_voidSetOff();				
						}    
    }
}
		}
