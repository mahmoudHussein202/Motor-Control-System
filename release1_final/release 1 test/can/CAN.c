
#define PART_TM4C123GH6PM

#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/can.h"
#include "sysclk.h"
#include "CAN.h"
#define NULL (void*)0
#define FALSE 0 
#define TRUE 1 


#define CAN0_BASE_ADDR CAN0_BASE_ADDR  // Base address of CAN0 module
#define CAN1_BASE_ADDR 0x40041000  // Base address of CAN1 module

#define WORKING_CAN	
#if CAN_NUMBER != CAN_NUM_0 || CAN1_BASE_ADDR != CAN_NUM_1
#define CAN_NUMBER	CAN0_BASE_ADDR
#endif
static void CAN_Init(uint32_t baseAddr, uint32_t canBitRate);

ReturnState CAN_xInit(void)
{
	ReturnState retVal = E_NOT_OK ; 
	CAN_Init(CAN_NUMBER, CAN_SPEED);
	return retVal ; 
}

ReturnState CAN_xSendRemoteFrame(uint32_t messageID)
{
	ReturnState retVal = E_NOT_OK ; 
	uint8_t isCanRemoteFrameIsSend = FALSE;
	// Create a CAN message object
	tCANMsgObject canMessage;
	canMessage.ui32MsgID = messageID;           		 // Message ID
	canMessage.ui32MsgIDMask = 0;            					// No mask used
	canMessage.ui32MsgLen = sizeof(uint8_t);					// Message length
	canMessage.ui32Flags = MSG_OBJ_TYPE_TX_REMOTE ;
	canMessage.pui8MsgData = NULL;           					// No message data
  while (isCanRemoteFrameIsSend == FALSE)
  {
		// Send the remote frame
		CANMessageSet(CAN_NUMBER, 1, &canMessage, MSG_OBJ_TYPE_TX);

		// Check if the remote frame is sent
		if (1/* Some condition to check if the remote frame is sent */)
		{
				isCanRemoteFrameIsSend = TRUE;
		}
  }
	
		return retVal ; 
}
ReturnState CAN_xReceive(uint32_t messageID ,uint32_t* P2Data )
{
		ReturnState retVal = E_NOT_OK ; 

		// Create a CAN message object
    tCANMsgObject canMessage;
    canMessage.ui32MsgID = messageID;            // Message ID
    canMessage.ui32MsgIDMask = 0;            // No mask used
    canMessage.ui32MsgLen = sizeof(uint8_t); // Message length
    canMessage.pui8MsgData = (uint8_t*)P2Data;           // No message data
		// Check if a message is available
		if (CANStatusGet(CAN_NUMBER, CAN_STS_NEWDAT))
		{
				// Receive the message
				CANMessageGet(CAN_NUMBER, 1, &canMessage, 1);

				// Process the received message
				// ...
		}
		return retVal ; 

}

static void CAN_Init(uint32_t baseAddr, uint32_t canBitRate)
{
	#if CAN_NUMBER == CAN1_BASE_ADDR
	// CAN 1
		SysCtlPeripheralEnable(SYSCTL_PERIPH_CAN1);  // Enable CAN1 module
    SysCtlPeripheralReset(SYSCTL_PERIPH_CAN1);   // Reset CAN1 module
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA); // Enable GPIOA peripheral

    GPIOPinConfigure(GPIO_PA0_CAN0RX);   // Configure PA0 as CAN0RX
    GPIOPinConfigure(GPIO_PA1_CAN0TX);   // Configure PA1 as CAN0TX
	
    GPIOPinTypeCAN(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);
	#else 
	// CAN 0 
		SysCtlPeripheralEnable(SYSCTL_PERIPH_CAN0);  // Enable CAN0 module
    SysCtlPeripheralReset(SYSCTL_PERIPH_CAN0);   // Reset CAN0 module
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF); // Enable GPIOF peripheral

    GPIOPinConfigure(GPIO_PF0_CAN0RX);   // Configure PF0 as CAN0RX
    GPIOPinConfigure(GPIO_PF3_CAN0TX);   // Configure PF3 as CAN0TX
	
    GPIOPinTypeCAN(GPIO_PORTF_BASE, GPIO_PIN_0 | GPIO_PIN_3);
	#endif 
    CANInit(baseAddr);                   // Initialize CAN controller
    CANBitRateSet(baseAddr, SysCtlClockGet(), canBitRate); // Set CAN bit rate
    CANEnable(baseAddr);                 // Enable CAN controller
}


