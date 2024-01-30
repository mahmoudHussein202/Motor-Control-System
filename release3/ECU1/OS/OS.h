#ifndef _TEST_H_
#define _TEST_H_
/********************************************
 * ************* Includes ****************** 
********************************************/
#include <stdint.h>
#include <stdbool.h>
#include "../MCAL/SYSTICK_DRIV.h"
#include "../SERVICES/SYSCLK/sysclk.h"
/******************************************
 * *********** Macros Values **************
*****************************************/
#define MIN_AVG_TEMPERATURE           0
#define MAX_AVG_TEMPERATURE           25 
#define ECU2_KEEP_ALIVE_ID            0x12
#define ECU2_TEMPERATURE_ID           0x13
#define KNOWN_VOLTAGE_COMP_MIN        0 
#define KNOWN_VOLTAGE_COMP_MAX        2
#define KEEP_ALIVE_ACK_SYMBOL         '^'
#define DTC_SAVE_AT_REPEATATION        3
/******************************************
 * ********* Type defines  **********
*****************************************/
/**
 * @brief Assuming that enum value == its address at eeprom
 * 
 */
typedef enum
{
  DTC_FAULT_STATE  = 0xA0 , 
  DTC_COMM_LOST    = 0xA4 ,
  DTC_OVERHEATED   = 0xA8 , 
}DTCs;

/**
 * @brief IRQ flags, determine the source of the interrupt when raising the corrosponding flag
 * in mainIRQs array
 */
typedef enum
{
  IRQ_RECEIVED_TEMP_FROM_CAN,
  IRQ_ALIVE_ACK,
  IRQ_COMM_LOST,
  IRQ_OVERHEATED,
	IRQ_FAULT,
  // this determine the maximum number of IRQs
  MAX_IRQ_HANDLING
}IRQ_Handling;

/**
 * @brief system states
 */
typedef enum
{
  SYS_FAULT_STATE,
  COMM_LOST,
  MOTOR_OVER_HEATING,
  NORMAL,
	GET_VOLTAGE_ERROR,
	PLEASE_CHECK_OVERHEATING,
	PLEASE_CHECK_FAULTSTATE,
  // this determine the maximum number of states
  MAX_SysState_Count
}SystemState;
/**
 * @brief the packet that'll be sent to pc for system_state
 */
typedef struct 
{
  /* data */
  SystemState copySystemState ; 
  uint8_t  avgTemperature ; 
}Packet;
/******************************************
 * *************** APIs ****************
*****************************************/
void os_init(void);
ReturnState APP_SendSystemState(void);
ReturnState APP_CheckTemperature(uint8_t copy_u8Temperature);
ReturnState DTC_Handle(void);
void APP_InterruptBased_answerOfKeepAlive(void);
void APP_InterruptBased_KeepAliveMsg(void);
void APP_InterruptBased_CheckCommunication(void);
void APP_InerruptBased_SendToPc(void);
void APP_InterruptBased_GetCANmsg(uint32_t tempratute);
/******************************************
 * *********** ECU2 Test Cases ************
*******************************************/
void receving_temp_ECU2( uint32_t  getTemperature);
void normal_condition_check(void);
void DTC_Error_check(void);
#endif