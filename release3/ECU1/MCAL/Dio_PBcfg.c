/**************************
 *
 * File Name: Dio_PBcfg.c
 *
 * Description: Source file for Post Build Configurations.
 *
 * Author: Youssef Sherif
 *
 ***************************/

#include "Dio.h"

/* PB structure used with Dio_Init API */
const Dio_ConfigType Dio_Configuration = {
                                             
				             DioConf_SW1_PORT_NUM,DioConf_SW1_CHANNEL_NUM,    // Portf , Pin 4
				             DioConf_SW2_PORT_NUM,DioConf_SW2_CHANNEL_NUM,    // Portf , Pin 0
										 DioConf_LED1_PORT_NUM,DioConf_LED1_CHANNEL_NUM,    // Portf , Pin 1
										 DioConf_LED1_PORT_NUM,DioConf_LED2_CHANNEL_NUM,    // Portf , Pin 2
										 DioConf_LED1_PORT_NUM,DioConf_LED3_CHANNEL_NUM,    // Portf , Pin 3
};
