#ifndef _CAN_DRIV_H_
#define _CAN_DRIV_H_
#include "stdint.h"

typedef enum 
{
    RTN_OK,
    RTN_ERROR
}can_state_t;

void Can_Driv_init(void);
void Can_tx_msg_config(uint8_t ID, uint8_t msg_size);
void Can_rx_msg_config(uint8_t ID, uint8_t msg_size);
void can_send_msg(uint8_t data);
uint32_t can_recevie_msg_sync(void);
uint32_t can_recevie_msg_Async(void);
//------------------------------------
void Can_tx_check_voltage_config(void);
void Can_rx_check_voltage(void);
void can_volt_msg(uint8_t data);
uint32_t can_recevie_volt_Async(void);

#endif
