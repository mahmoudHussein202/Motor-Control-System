/***************** Includes *********************
************************************************/
#include "../OS/OS.h"
/***************** Externs **********************
************************************************/
extern uint8_t sending_volt_flag;
extern uint8_t test_ECU2_temp;
extern uint8_t test_ECU2_volt;
/**********************************************/
int main(void)
{
	/**************** Initialization ***************/
	RGB_LED_init();
	Can_Driv_init();
	ADC_DRIV_Init();
	systick_init(can_send_Temp);
	/*************** Local Variables ***************/
	float voltage_0 = 0;
	float voltage_1 = 0;
	/**********************************************/
	while(1)
	{
		//[1]check for acknowledge
		can_start_recieving_ack();
		//[2]calculate input temperature 
		voltage_0 = ADC_get_volt(1);
		test_ECU2_temp = (uint8_t)(voltage_0 * 10);
		//[3]calculate input voltage 
		voltage_1 = ADC_get_volt(0);
		test_ECU2_volt = (uint8_t)(voltage_1);
	}
		return(0);
}


