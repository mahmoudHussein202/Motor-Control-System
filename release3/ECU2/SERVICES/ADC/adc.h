#ifndef _ADC_D_H_
#define _ADC_D_H_
#include "stdint.h"
#include "../../MCAL/tm4c123gh6pm.h"
/************* APIs ************/
void ADC_DRIV_Init(void);
float ADC_get_volt(uint8_t channel_num);
#endif