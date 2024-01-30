/**************************
 *
 * Module: Led
 *
 * File Name: Led.h
 *
 * Description: Header file for Led Module.
 *
 * Author: Mohamed Tarek
 **************************/
#ifndef LED_H
#define LED_H

// #include "Std_Types.h"

/* Set the led ON/OFF according to its configuration Positive logic or negative logic */
#define STD_HIGH        0x01U       /* Standard HIGH */
#define STD_LOW         0x00U       /* Standard LOW */

#define LED_ON  STD_HIGH
#define LED_OFF STD_LOW

/* Set the LED Port */
#define LED_PORT DioConf_LED1_PORT_NUM

/* Set the LED Pin Number */
#define LED_PIN_NUM DioConf_LED1_CHANNEL_NUM

void RGB_LED_init(void);
void LED_voidSetRed(void);
void LED_voidSetGreen(void);
void LED_voidSetBlue(void);
void LED_voidSetWhite(void);
void LED_voidSetOff(void);

#endif /* LED_H */