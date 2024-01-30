#include"test.h"
#include "../systick/systick.h"
#include "../sysclk/sysclk.h"
#include "../can/CAN.h"
#include "../uart/uart_config.h"

extern uint8_t Glob_FaultState_Counter  ; 
extern uint8_t Glob_CommunicationLost_Counter ; 
extern uint8_t Glob_Overheated_Counter  ; 
extern IRQ_Handling mainIRQs[MAX_IRQ_HANDLING] ; 
extern char stateToString[MAX_SysState_Count][30] ;
extern Packet PacketToPc ;
uint32_t ECU2_current_temp;
/****************************************
*******************test******************
**************************************/
uint32_t test_ECU2_temp = 25;
uint32_t getKnownVoltage = 80 ; 
//********************************//
int main(void)
{
    // init
    sysclk_setSystemClk();
    systick_init(APP_InterruptBased_KeepAliveMsg , APP_InerruptBased_SendToPc , APP_InterruptBased_CheckCommunication);
    CAN_xInit();
    UART_INIT();
    while(1)
    { 

			/*FOR IRQ_RECEIVED_TEMP_FROM_CAN Ready To Check Temperature     */
      receving_temp_ECU2(ECU2_current_temp,getKnownVoltage);
      /*      NORMAL    */ 
      normal_condition_check();
      // Call DTC_Handle 
      DTC_Error_check();
		}
}
