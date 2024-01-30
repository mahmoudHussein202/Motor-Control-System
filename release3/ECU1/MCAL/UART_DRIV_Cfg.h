/*********************************************************************
 * ******************** UART Configurations.h ************************
 * @brief this file contain the configurations of the UART Driver
 * ENG: Mahmoud Hussein
*/
#ifndef _UART_CONFIG_H_
#define _UART_CONFIG_H_
#include "UART_DRIV.h"
/*-----------------------   UART Selection -----------------------------
CTL_CLK_UART0 , CTL_CLK_UART1 , CTL_CLK_UART2, CTL_CLK_UART3, CTL_CLK_UART4
CTL_CLK_UART6, CTL_CLK_UART6, CTL_CLK_UART7
*/
#define UART_select   CTL_CLK_UART0 
/*----------------------   UART BaudRate ------------------------------
9600, 384600
*/
#define UART_BaudRate  9600
/*----------------------   Data Length  ------------------------------
UART_data_len_8, UART_data_len_7, UART_data_len_6, UART_data_len_5
*/
#define Data_length UART_data_len_8
/*----------------------   Stop Bits no.   ------------------------------
UART_STOP_ONE, UART_STOP_ONE
*/
#define stop_bits_no UART_STOP_ONE
/*--------------------- Parity Select   ------------------------------------
UART_PAR_NONE, UART_PAR_EVEN, UART_PAR_ODD, UART_PAR_ONE, UART_PAR_ZERO
*/
#define parity_select UART_PAR_NONE
/************************************************************************/
#endif
