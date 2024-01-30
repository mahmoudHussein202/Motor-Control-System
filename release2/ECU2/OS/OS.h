#ifndef _OS_H_
#define _OS_H_
#define PART_TM4C123GH6PM
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
#include "../RGB/RGB.h"
#include "../CAN/Can.h"
#include "../SYSTICK/systick.h"
#include "../ADC/adc.h"

/****************************************************
********************* APIs **************************
****************************************************/
void can_start_recieving_ack(void);
void can_send_Temp(uint8_t Temp);
#endif