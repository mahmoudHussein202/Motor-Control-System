#ifndef CAN_CONTROLLER_H
#define CAN_CONTROLLER_H
#include "../sysclk/sysclk.h"
#include <stdint.h>
#include <stdbool.h>

#define CAN_SPD_125kpbs 125000
#define CAN_SPD_500Kpbs 500000
#define CAN_SPD_1Mpbs   1000000	

#define CAN_NUM_0				0x40040000
#define CAN_NUM_1				0x40041000

/******************** Configuration	************************/

#define CAN_NUMBER	CAN_NUM_0
#define CAN_SPEED		CAN_SPD_125kpbs


/******************** Public APIs	************************/
ReturnState CAN_xInit(void);

ReturnState CAN_xSendRemoteFrame(uint32_t messageID);

ReturnState CAN_xReceive(uint32_t messageID ,uint32_t* P2Data );




#endif
