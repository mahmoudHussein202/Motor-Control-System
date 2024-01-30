#ifndef _UART_CONFIG_H_
#define _UART_CONFIG_H_
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"
/*-----------------------   UART Selection -----------------------------
SYSCTL_PERIPH_UART0, SYSCTL_PERIPH_UART1, SYSCTL_PERIPH_UART2, SYSCTL_PERIPH_UART3, 
SYSCTL_PERIPH_UART4, SYSCTL_PERIPH_UART5, SYSCTL_PERIPH_UART6, SYSCTL_PERIPH_UART7
*/
#define UART_select     SYSCTL_PERIPH_UART0 
/*----------------------   UART BaudRate ------------------------------
9600, 384600
*/
#define UART_BaudRate  9600
/*----------------------   Data Length  ------------------------------
UART_CONFIG_WLEN_8, UART_CONFIG_WLEN_7, UART_CONFIG_WLEN_6, UART_CONFIG_WLEN_5
*/
#define Data_length UART_CONFIG_WLEN_8
/*----------------------   Stop Bits no.   ------------------------------
UART_CONFIG_STOP_ONE, UART_CONFIG_STOP_TWO
*/
#define stop_bits_no UART_CONFIG_STOP_ONE
/*--------------------- Parity Select   ------------------------------------
UART_CONFIG_PAR_NONE, UART_CONFIG_PAR_EVEN, UART_CONFIG_PAR_ODD, UART_CONFIG_PAR_ONE, UART_CONFIG_PAR_ZERO
*/
#define parity_select UART_CONFIG_PAR_NONE
/**************************************************************************
 * ***********************************************************************
*/



void UART_INIT();
void UART_Send_string(char *data);
#endif
