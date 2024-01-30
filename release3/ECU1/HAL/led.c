/**************************
 *
 * Module: Led
 *
 * File Name: Led.c
 *
 * Description: Source file for Led Module.
 *
 * Author: Mohamed Tarek
 **************************/
#include "../MCAL/Port.h"
#include "../MCAL/Dio.h"
#include "Led.h"
/* LED Configurations Structure */
static Port_ConfigType  g_LED_Config;
#define STD_HIGH        0x01U       /* Standard HIGH */
#define STD_LOW         0x00U       /* Standard LOW */

#define LED_ON  STD_HIGH
#define LED_OFF STD_LOW

/*******************************/
static void LED_setOn(Dio_ChannelType ChannelId);
static void LED_setOff(Dio_ChannelType ChannelId);
static void LED_refreshOutput(Dio_ChannelType ChannelId); 
static void LED_toggle(Dio_ChannelType ChannelId);
/*******************************/
// example LED_setOn(DioConf_LED1_CHANNEL_ID_INDEX)
void LED_setOn(Dio_ChannelType ChannelId)
{
    Dio_WriteChannel(ChannelId,LED_ON);  /* LED ON */
}

/*******************************/
void LED_setOff(Dio_ChannelType ChannelId)
{
    Dio_WriteChannel(ChannelId,LED_OFF); /* LED OFF */
}

/*******************************/
void LED_refreshOutput(Dio_ChannelType ChannelId)
{
    Dio_LevelType state = Dio_ReadChannel(ChannelId);
    Dio_WriteChannel(ChannelId,state); /* re-write the same value */
}

/*******************************/
void LED_toggle(Dio_ChannelType ChannelId)
{
    Dio_LevelType state = Dio_FlipChannel(ChannelId);
}
/*************PUBLIC APIs******************/
void LED_voidSetRed(void)
{
	LED_setOn(DioConf_LED1_CHANNEL_ID_INDEX);
	LED_setOff(DioConf_LED2_CHANNEL_ID_INDEX);
	LED_setOff(DioConf_LED3_CHANNEL_ID_INDEX);
}
void LED_voidSetGreen(void)
{
	LED_setOff(DioConf_LED1_CHANNEL_ID_INDEX);
	LED_setOff(DioConf_LED2_CHANNEL_ID_INDEX);
	LED_setOn (DioConf_LED3_CHANNEL_ID_INDEX);
}
void LED_voidSetBlue(void)
{
	LED_setOff(DioConf_LED1_CHANNEL_ID_INDEX);
	LED_setOn (DioConf_LED2_CHANNEL_ID_INDEX);
	LED_setOff(DioConf_LED3_CHANNEL_ID_INDEX);
}
void LED_voidSetWhite(void)
{
	LED_setOn(DioConf_LED1_CHANNEL_ID_INDEX);
	LED_setOn(DioConf_LED2_CHANNEL_ID_INDEX);
	LED_setOn (DioConf_LED3_CHANNEL_ID_INDEX);
}
void LED_voidSetOff(void)
{
	LED_setOff(DioConf_LED1_CHANNEL_ID_INDEX);
	LED_setOff(DioConf_LED2_CHANNEL_ID_INDEX);
	LED_setOff (DioConf_LED3_CHANNEL_ID_INDEX);
}
void RGB_LED_init(void)
{
	Port_Init(&Port_ConfigSet[0]); 
	Dio_Init(&Dio_Configuration); 
}
/*******************************/
