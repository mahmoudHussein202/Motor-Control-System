#include "stdio.h"
#include "OS.h"
#include "../SERVICES/CAN/Can.h"
#include "../MCAL/UART_DRIV.h"
#include "../HAL/Led.h"
#include "../HAL/Button.h"
#include "../SERVICES/EEPROM/eeprom_s.h"
/******************************************
 * ********* Global Variables **********
*****************************************/
uint8_t Glob_FaultState_Counter = 0 ; 
uint8_t Glob_CommunicationLost_Counter = 0 ; 
uint8_t Glob_Overheated_Counter = 0 ; 
uint8_t get_voltage_counter = 0;
IRQ_Handling mainIRQs[MAX_IRQ_HANDLING] ; 
char stateToString[MAX_SysState_Count][80] = {"Fault State.\r\n", "Communication Lost.\r\n" , "Motor Overheated.\r\n" , "Normal.\r\n", "GET VOLTAGE ERROR.\r\n", "Please check the overheating problem and press sw1 and sw2.\r\n","Please check the the voltage and press sw1 and sw2.\r\n"};
Packet PacketToPc ; 
extern uint32_t ECU2_current_temp;
extern uint32_t getKnownVoltage;
extern uint32_t test_ECU2_temp;

/**
 * @brief This Function is used to send current system state and temperature to PC
 * 
 * @return ReturnState E_OK if function executed correctly.
 */
ReturnState APP_SendSystemState(void)
{
  ReturnState retVal= E_OK ; 
  char tempBuffer[3] = {0}; 
  UART_Send_string("- System State : ");
  UART_Send_string(stateToString[PacketToPc.copySystemState]);
  UART_Send_string("\n- Average Temperature : ");
  sprintf(tempBuffer, "%d", PacketToPc.avgTemperature); 
  UART_Send_string(tempBuffer);  
	UART_Send_string("\n---------------------------------\n");  

  return retVal; 
}
/**
 * @brief This Function is responsible for evaluate the current situation of temperature 
 * 
 * @param copy_u8Temperature current value of temperature 
 * @return ReturnState will return E_OK if exist trivial value or temperature was normal.
 *                          return E_NOT_OK if exists abnormal condition
 */
ReturnState APP_CheckTemperature(uint8_t copy_u8Temperature)
{
 
  #define ERROR_IN_TEMP_READINGS_MARGIN 6
  ReturnState retVal = E_NOT_OK ; 
  static uint8_t TempOverBoundries[ERROR_IN_TEMP_READINGS_MARGIN] = {0};
  static uint8_t TempOverBoundries_Counter = 0 ; 
  if(copy_u8Temperature >= MIN_AVG_TEMPERATURE && copy_u8Temperature <= MAX_AVG_TEMPERATURE)
  {
    // Clear Flags 
    for (uint8_t i = 0; i < ERROR_IN_TEMP_READINGS_MARGIN; i++)
    {
      TempOverBoundries[i] = 0 ; 
    }
    TempOverBoundries_Counter = 0 ;
    mainIRQs[IRQ_OVERHEATED] = 0 ; 
    retVal = E_OK ;
  }
  else
  {
    TempOverBoundries[TempOverBoundries_Counter] = 1 ;
    TempOverBoundries_Counter++ ; 
    if(TempOverBoundries_Counter >= ERROR_IN_TEMP_READINGS_MARGIN || mainIRQs[IRQ_OVERHEATED] == 1)
    {
      // Will Send Remote Frame 
      retVal = E_NOT_OK ; 
      // Set sytem to overheted 
      mainIRQs[IRQ_OVERHEATED] = 1 ;
      TempOverBoundries_Counter = 0 ; 
    }
    else
    {
      retVal = E_OK ; 
    }
  }
  return retVal ; 
}
/**
 * @brief This Function is used to handle DTCs and save it at ROM
 * 
 * @return ReturnState 
 */
ReturnState DTC_Handle(void)
{
  ReturnState retVal;
  if(Glob_CommunicationLost_Counter >= DTC_SAVE_AT_REPEATATION)
  {
    // SAVED IN EEPROM AT ADDRESS DTC_COMM_LOST
		while(mainIRQs[IRQ_COMM_LOST]!=0){
		/* waiting for APP_InterruptBased_GetCANmsg to interrupt the system and reset irq com.lost flag*/
			LED_voidSetRed();
		}
    retVal = E_OK ; 
  }
  else
  {
    // Never Happens 
    retVal = E_NOT_OK; 
  }

  if(Glob_FaultState_Counter >= DTC_SAVE_AT_REPEATATION)
  {
    // SAVED IN EEPROM AT ADDRESS DTC_FAULT_STATE
		// hault to force the user to reset
		uint32_t fault = 'f';
		EEPROMProgram(&fault,0x450,8);
		while(1){LED_voidSetBlue(); }
    retVal = E_OK ;        
  }
  else
  {
    // Never Happens 
    retVal = E_NOT_OK; 
  }
  if(Glob_Overheated_Counter >= DTC_SAVE_AT_REPEATATION)
  {
    // SAVED IN EEPROM AT ADDRESS DTC_OVERHEATED
		// send a signal to shut down the motor
		// hault the system to force user to reset
		uint32_t fault = 'h';
		EEPROMProgram(&fault,0x450,8);
		while(1)
		{
			LED_voidSetWhite();
		}
    retVal = E_OK ; 
  }
  else
  {
    // Never Happens 
    retVal = E_NOT_OK; 
  }
  return retVal ; 
}
/*           INTERRUPTS       */
/**
 * @brief Called every 500ms 
 * 
 */
void APP_InterruptBased_KeepAliveMsg(void)
{
    Can_tx_msg_config(ECU2_KEEP_ALIVE_ID,8); 
    can_send_msg('^');
    mainIRQs[IRQ_ALIVE_ACK] =  1 ; 
		Can_rx_msg_config(ECU2_KEEP_ALIVE_ID,8); 
}
/**
 * @brief Will Called Every 5 Sec
 * 
 */
void APP_InterruptBased_CheckCommunication(void)
{
  // في حاله انين بعت كيب اليف هخلي الفلاج بواحد 
  // فلو الفلاج فضل ب واحد لمده 5 ثواني هيجي الانتربت دا فيرفع الفلاج التاني
  if(mainIRQs[IRQ_ALIVE_ACK] == 1)
  {
    // Communication lost
    mainIRQs[IRQ_COMM_LOST] = 1 ;
    LED_voidSetRed();
  }
  else
  {
    mainIRQs[IRQ_ALIVE_ACK] = 0 ; 
    mainIRQs[IRQ_COMM_LOST] = 0 ;
    LED_voidSetGreen();
  }
}
/**
 * @brief Will Call every 1 ses
 * 
 */
void APP_InerruptBased_SendToPc(void)
{
  APP_SendSystemState();
}
void receving_temp_ECU2( uint32_t  getTemperature)
{
  if(mainIRQs[IRQ_RECEIVED_TEMP_FROM_CAN] == 1 )
  {
    //[1] Clear IRQ Flag
    mainIRQs[IRQ_RECEIVED_TEMP_FROM_CAN] = 0 ; 
    //[2] Check Values <- لو رجعتلي حاجه غير اوك يبقي حصل ان 3 مرات برا رينج درجه الحراره 
    ReturnState retVal = APP_CheckTemperature((uint8_t)getTemperature);
    // في اخر مره الفلاج هيتحط فيها ب 1 هيخش هنا 
    // والكونتر اللي في الفانكشن فوق هيتصفر عشان لو فضلت تجيلي درجه حراره غلط يعيد من الاول يحسب 
    if(retVal != E_OK)
    {
      // هيبعت يسأل علي ال KNOW VOLTAGE 
      // Send Remote Frame 
				Can_tx_msg_config(0x14,8);
				can_volt_msg('c');
        //IntMasterDisable();
			
      //CAN_xSendRemoteFrame(ECU2_TEMPERATURE_ID);
      //CAN_xReceive(ECU2_TEMPERATURE_ID , &getKnownVoltage);

      if(KNOWN_VOLTAGE_COMP_MIN  <= getKnownVoltage && KNOWN_VOLTAGE_COMP_MAX >= getKnownVoltage)
      {
        // لو في الرنج السيستم هيتحط في اوفرهيتنج 
        // send signal to Shutdoown motor 
        // Set sys to overheated
        PacketToPc.copySystemState = MOTOR_OVER_HEATING ; 
        // white LED for 10 seconds
        // Clear Glob_FaultState_Counter
        Glob_FaultState_Counter = 0 ; 
        Glob_Overheated_Counter++ ; 
				mainIRQs[IRQ_FAULT] = 0 ; 
				mainIRQs[IRQ_OVERHEATED] = 1 ;
				// Add temp to struct 
				PacketToPc.avgTemperature = getTemperature ; 
        // Save DTC
      }
      else
      {
        // لو النون فولتدج بره الرينج فانا كدا فولت ستيت
        // Fault state 

				
				
        // Stop CAN Communication until DTC is cleared ( DTC will be saved and will stay in fault state until SW1 &2 are pressed on both ECUs)
        
        // Clear Glob_Overheated_Counter
          Glob_FaultState_Counter++;
          Glob_Overheated_Counter = 0 ;
				mainIRQs[IRQ_OVERHEATED] = 0 ; 
				mainIRQs[IRQ_FAULT] = 1 ; 
        // Set Sys to fault
        PacketToPc.copySystemState = SYS_FAULT_STATE ; 
				PacketToPc.avgTemperature = getTemperature ; 
      }
    }
		else if (Glob_FaultState_Counter >=1)
		{
			Glob_FaultState_Counter++;
		}
		else
		{
			// Add temp to struct 
			PacketToPc.avgTemperature = getTemperature ; 
		}
		
		
  }
}

void normal_condition_check()
{
  // هنا انا بطمن ان مفيش اوفر هيتنج ولا مفيش فولت ستيت ولا كومينكيشن لوست 
  // هنا عندي 3 كونديشن 
  // الاول ان مفيش اوفر هيتنج 
  // التاني عباره عن حاجتين 
  // عشان ممكن يكون ال IRQ_ALIVE_ACK = 1 بس لسه فيه كومينكيشن 
  if(mainIRQs[IRQ_OVERHEATED] == 0 && mainIRQs[IRQ_FAULT] == 0  &&
                                    ((mainIRQs[IRQ_ALIVE_ACK] == 0 && mainIRQs[IRQ_COMM_LOST] == 0) || 
                                    (mainIRQs[IRQ_ALIVE_ACK] == 1 && mainIRQs[IRQ_COMM_LOST] == 0) ))
  {
        // Normal Temperature 
        PacketToPc.copySystemState = NORMAL ; 
        // green LED on

        // Clear Comm Lost 
        Glob_CommunicationLost_Counter = 0 ; 
      }
      else
      {
        /*          FOR IRQ_ALIVE_ACK and IRQ_COMM_LOST         */
        if(mainIRQs[IRQ_ALIVE_ACK] == 1 && mainIRQs[IRQ_COMM_LOST] == 1)
        {
          // communication lost
          PacketToPc.copySystemState = COMM_LOST ;
          Glob_CommunicationLost_Counter++; 
          // Blinking Blue led for 10 seconds waiting on communication to be restored
          // المفروض هيحصل ريستور لما ابعت ريموت فريم تاني لما يحصل انتربت بعد 500 ملي 
          // في الانتربت دا بكلير الفلاجز دي 
          // بشيلهم هنا بيجي كل 5 ثواني 
          // APP_InterruptBased_CheckCommunication
        }
				
  }
}

void DTC_Error_check()
{
   if(Glob_CommunicationLost_Counter >= DTC_SAVE_AT_REPEATATION || Glob_FaultState_Counter >= DTC_SAVE_AT_REPEATATION || Glob_Overheated_Counter >= DTC_SAVE_AT_REPEATATION)
      {
        DTC_Handle();
      }
}
void os_init()
{
	eeprom_init();
	uint32_t check_last_state = 0;
	uint32_t check_finished = 0;
	EEPROMRead(&check_last_state,0x450,8);
	switch(check_last_state)
	{
		case 0:
			break;
		case 'h':
		{
			LED_voidSetWhite();
			uint16_t counter = 0;
			PacketToPc.copySystemState = PLEASE_CHECK_OVERHEATING;
			uint8_t button1_state = BUTTON_RELEASED;
			uint8_t button2_state = BUTTON_RELEASED;
			while(1)
			{
				LED_voidSetWhite();
				counter++;
				if ( counter == 10000)
				{
					counter = 0;
					APP_SendSystemState();
				}		
				Button1_Current_State();
				Button2_Current_State();
				button1_state = BUTTON_getState1();
				button2_state = BUTTON_getState2();
				if(button1_state == BUTTON_PRESSED && button2_state == BUTTON_PRESSED)
				{
					EEPROMProgram(&check_finished,0x450,8);
					break;
				}
			}
			break;
		}
				case 'f':
		{
			uint16_t counter = 0;
			LED_voidSetBlue();
			PacketToPc.copySystemState = PLEASE_CHECK_FAULTSTATE;
			uint8_t button1_state = BUTTON_RELEASED;
			uint8_t button2_state = BUTTON_RELEASED;
			while(1)
			{
				LED_voidSetBlue();
				counter++;
				if ( counter == 10000)
				{
					counter = 0;
					APP_SendSystemState();
				}		
				Button1_Current_State();
				Button2_Current_State();
				button1_state = BUTTON_getState1();
				button2_state = BUTTON_getState2();
				if(button1_state == BUTTON_PRESSED && button2_state == BUTTON_PRESSED)
				{
					EEPROMProgram(&check_finished,0x450,8);
					break;
				}
			}
			break;
		}
	}
	
}
/*******************************************************
**************** ECU2 Function Dependent ***************
********************************************************
 * @brief Will Call when ECU2 send temp usually every 1 sec 
 * 
 */

void APP_InterruptBased_GetCANmsg(uint32_t recevied_data)
{
  ReturnState retVal = E_NOT_OK;
  uint32_t getTemperature = recevied_data ; 
  // Temperature Each Sec 
  // [removed in test] CAN_xReceive(ECU2_TEMPERATURE_ID , &getTemperature);
  if (getTemperature == KEEP_ALIVE_ACK_SYMBOL)
  {
    mainIRQs[IRQ_COMM_LOST]=0;
    mainIRQs[IRQ_ALIVE_ACK]=0;
    LED_voidSetGreen();
  }
  else if (getTemperature<33)
  {
			ECU2_current_temp = getTemperature;
      mainIRQs[IRQ_RECEIVED_TEMP_FROM_CAN] = 1 ; 
		    mainIRQs[IRQ_COMM_LOST]=0;
    mainIRQs[IRQ_ALIVE_ACK]=0;
    LED_voidSetGreen();
  }
	else 
	{
		getKnownVoltage = getTemperature/40;
		
	}
}
