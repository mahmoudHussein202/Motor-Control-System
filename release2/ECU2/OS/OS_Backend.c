/****************** Includes ********************
*************************************************/
#include "stdint.h"
#include "OS.h"
/***************************************************
****************** Global Variables ****************
***************************************************/
uint8_t sending_volt_flag;
uint8_t test_ECU2_temp;
uint8_t test_ECU2_volt;
/**************************************************/

void can_start_recieving_ack()
{
	uint32_t msg_rec = 0;
	Can_rx_msg_config(0x0,8);
	msg_rec = can_recevie_msg_Async();
	if (msg_rec == '^')
	{
		Can_tx_msg_config(0x12,8);
		can_send_msg('^');
	}
	else if(msg_rec == 'c')
	{
		sending_volt_flag = 1;
		RGB_BLUE();
	}
}
void can_send_Temp(uint8_t Temp)
{
	Can_tx_msg_config(0x13,8);
	can_send_msg(Temp);
	for(int i = 0 ; i < 1200; i++){}
	can_send_msg(test_ECU2_volt*40);	
}