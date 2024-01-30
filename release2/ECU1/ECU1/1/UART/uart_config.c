#include <stdint.h>
#include <stdbool.h>
#include "tm4c123gh6pm.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/sysctl.h"
#include "driverlib/interrupt.h"
#include "driverlib/can.h"
#include "driverlib/gpio.h"
#include "driverlib/uart.h"
#include "uart_config.h"
#include "driverlib/pin_map.h"
#include "inc/hw_ints.h"

#if UART_select==SYSCTL_PERIPH_UART0
#define Uart_base UART0_BASE
#elif UART_select==SYSCTL_PERIPH_UART2
#define Uart_base UART2_BASE
#elif UART_select==SYSCTL_PERIPH_UART3
#define Uart_base UART3_BASE
#elif UART_select==SYSCTL_PERIPH_UART4
#define Uart_base UART4_BASE
#elif UART_select==SYSCTL_PERIPH_UART5
#define Uart_base UART5_BASE
#elif UART_select==SYSCTL_PERIPH_UART6
#define Uart_base UART6_BASE
#elif UART_select==SYSCTL_PERIPH_UART7
#define Uart_base UART7_BASE
#endif
void UART_INIT(void)
{
     // enable UART
    SysCtlPeripheralEnable(UART_select);
		SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
		GPIOPinConfigure(1);
		GPIOPinConfigure(0x00000401);
		GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);
    // wait UART to be ready
    while(!SysCtlPeripheralReady(UART_select));
    // initialize the UART
    UARTConfigSetExpClk(Uart_base,SysCtlClockGet(),UART_BaudRate,(Data_length | stop_bits_no |parity_select));
}
void UART_Send_string(char data[])
{
    for (int i=0; data[i]!= '\0'; i++)
    {
        UARTCharPut(Uart_base,data[i]);
		}  
}
	
