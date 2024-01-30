/*********************************************
************ UART_DRIVER.c********************
* @brief this file contain the implemetation of UART driver
* Eng: Mahmoud Hussein
**********************************************/
#include "stdint.h"
#include "UART_DRIV.h"
#include "complex_fun.h"
void uart_init()
{
    uint32_t ui32UARTClk = current_clk();
    uint32_t divisor = 0;
    uint32_t current_baudrate = UART_BaudRate;
    //[1]Enable clock 
    *((volatile uint32_t *)((
        (uint32_t)(0x400fe600 + ((CTL_CLK_UART0 & 0xff00) >> 8)) & 0xF0000000) | 0x02000000 | (((uint32_t)(0x400fe600 + ((CTL_CLK_UART0 & 0xff00) >> 8)) & 0x000FFFFF) << 5) | ((CTL_CLK_UART0 & 0xff) << 2)))=1;
    //[2]wait until tx stops
		for (int i = 0 ; i < 10000; i++);
    //[3]disable uart_fifo
    (*((volatile uint32_t *)(Uart_base + 0x0000002C))) &= ~(UART_LCRH_FEN);
    //[4]disable uart
    (*((volatile uint32_t *)(Uart_base + 0x00000030))) &= ~(UART_CTL_UARTEN | UART_CTL_TXE |UART_CTL_RXE);
    //[5] Is the required baud rate greater than the maximum rate supported
        // without the use of high speed mode?
    if((UART_BaudRate * 16) > ui32UARTClk)
    {
        (*((volatile uint32_t *)(Uart_base + 0x00000030)))|= UART_CTL_HSE;
        current_baudrate /= 2;
    }
    else
    {
        // Disable high speed mode.
        (*((volatile uint32_t *)(Uart_base + 0x00000030))) &= ~(UART_CTL_HSE);
    }
    //[6] Compute the fractional baud rate divider
    divisor = (((ui32UARTClk * 8) / UART_BaudRate) + 1) / 2;
    //[7] set the baudrate
    (*((volatile uint32_t *)(Uart_base + 0x00000024))) = divisor / 64;
    (*((volatile uint32_t *)(Uart_base + 0x00000028))) = divisor % 64;
    //[8] Set parity, data length, and number of stop bits.
    (*((volatile uint32_t *)(Uart_base + 0x0000002C))) = Data_length | stop_bits_no |parity_select;
    //[9] clear flags
    (*((volatile uint32_t *)(Uart_base + 0x00000018)))=0;
    //[10]enable fifo
    (*((volatile uint32_t *)(Uart_base + 0x0000002C)))|= UART_LCRH_FEN;
    //[11] enable tx and rx 
    (*((volatile uint32_t *)(Uart_base + 0x00000030))) |= (UART_CTL_UARTEN | UART_CTL_TXE |UART_CTL_RXE);
}
void UART_Send_string(char data[])
{
    for (int i=0; data[i]!= '\0'; i++)
    {
        //[1] wait available space in buffer
        while((*((volatile uint32_t *)(Uart_base + 0x00000018))) & UART_FR_TXFF){}
        //[2] add the char to data register
        (*((volatile uint32_t *)(Uart_base + 0x00000000))) =data[i];
	}  
}