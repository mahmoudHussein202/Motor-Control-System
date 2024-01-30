/*
 * RGB.c
 *
 *  Created on: Jan 19, 2024
 *      Author: Mahmoud Hussein
 */
#include "RGB.h"
#include "ttttm4c123gh6pm.h"
void RGB_INIT(void)
{
    // Enable the clock for GPIO port F
    SYSCTL_RCGCGPIO_R |= 0x20;

    // Unlock GPIO port F
    GPIO_PORTF_LOCK_R = 0x4C4F434B;

    // Enable pins PF1, PF2, and PF3 for digital function
    GPIO_PORTF_DEN_R |= 0x0E;

    // Configure pins PF1, PF2, and PF3 as output
    GPIO_PORTF_DIR_R |= 0x0E;
}
void RGB_RED(void)
{
    // Turn on the red LED
    GPIO_PORTF_DATA_R = 0x02;
}
void RGB_BLUE(void)
{
    GPIO_PORTF_DATA_R = 0x04;
}
void RGB_GREEN(void)
{
    GPIO_PORTF_DATA_R = 0x08;
}
void RGB_WHITE(void)
{
    GPIO_PORTF_DATA_R = 0x0E;
}
void RGB_OFF(void)
{
    GPIO_PORTF_DATA_R = 0x00;
}
