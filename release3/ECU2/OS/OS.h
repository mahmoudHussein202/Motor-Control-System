#ifndef _OS_H_
#define _OS_H_
#define PART_TM4C123GH6PM
#include "../MCAL/MCAL.h"
#include "../HAL/Led.h"
#include "../SERVICES/CAN/Can.h"
#include "../MCAL/SYSTICK_DRIV.h".h"
#include "../SERVICES/ADC/adc.h"

/****************************************************
********************* APIs **************************
****************************************************/
void can_start_recieving_ack(void);
void can_send_Temp(uint8_t Temp);
#endif