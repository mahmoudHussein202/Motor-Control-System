/********************************
 * ******UART_DRIVER.h**********
 * Eng: Mahmoud Hussein
*******************************/

#ifndef _UART_DRIV_H_
#define _UART_DRIV_H_
#include "UART_DRIV_Cfg.h"
#if UART_select==CTL_CLK_UART0
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
/**************************************************
 * ************* General Macros  ******************
***************************************************/
// Clock Enabling 
#define CTL_CLK_UART0            0xf0001800  // UART 0
#define CTL_CLK_UART1            0xf0001801  // UART 1
#define CTL_CLK_UART2            0xf0001802  // UART 2
#define CTL_CLK_UART3            0xf0001803  // UART 3
#define CTL_CLK_UART4            0xf0001804  // UART 4
#define CTL_CLK_UART5            0xf0001805  // UART 5
#define CTL_CLK_UART6            0xf0001806  // UART 6
#define CTL_CLK_UART7            0xf0001807  // UART 7
// UART[x] Base Addresses
#define UART0_BASE              0x4000C000  // UART0
#define UART1_BASE              0x4000D000  // UART1
#define UART2_BASE              0x4000E000  // UART2
#define UART3_BASE              0x4000F000  // UART3
#define UART4_BASE              0x40010000  // UART4
#define UART5_BASE              0x40011000  // UART5
#define UART6_BASE              0x40012000  // UART6
#define UART7_BASE              0x40013000  // UART7
// bit fields for UART_x_FR register
#define UART_FR_RI               0x00000100  // Ring Indicator
#define UART_FR_TXFE             0x00000080  // UART Transmit FIFO Empty
#define UART_FR_RXFF             0x00000040  // UART Receive FIFO Full
#define UART_FR_TXFF             0x00000020  // UART Transmit FIFO Full
#define UART_FR_RXFE             0x00000010  // UART Receive FIFO Empty
#define UART_FR_BUSY             0x00000008  // UART Busy
#define UART_FR_DCD              0x00000004  // Data Carrier Detect
#define UART_FR_DSR              0x00000002  // Data Set Ready
#define UART_FR_CTS              0x00000001  // Clear To Send
// bit fields for UART_x_LCRH register
#define UART_LCRH_SPS            0x00000080  // UART Stick Parity Select
#define UART_LCRH_WLEN_M         0x00000060  // UART Word Length
#define UART_LCRH_WLEN_5         0x00000000  // 5 bits (default)
#define UART_LCRH_WLEN_6         0x00000020  // 6 bits
#define UART_LCRH_WLEN_7         0x00000040  // 7 bits
#define UART_LCRH_WLEN_8         0x00000060  // 8 bits
#define UART_LCRH_FEN            0x00000010  // UART Enable FIFOs
#define UART_LCRH_STP2           0x00000008  // UART Two Stop Bits Select
#define UART_LCRH_EPS            0x00000004  // UART Even Parity Select
#define UART_LCRH_PEN            0x00000002  // UART Parity Enable
#define UART_LCRH_BRK            0x00000001  // UART Send Break
// bit fields for UART_x_CTL
#define UART_CTL_CTSEN           0x00008000  // Enable Clear To Send
#define UART_CTL_RTSEN           0x00004000  // Enable Request to Send
#define UART_CTL_RTS             0x00000800  // Request to Send
#define UART_CTL_DTR             0x00000400  // Data Terminal Ready
#define UART_CTL_RXE             0x00000200  // UART Receive Enable
#define UART_CTL_TXE             0x00000100  // UART Transmit Enable
#define UART_CTL_LBE             0x00000080  // UART Loop Back Enable
#define UART_CTL_HSE             0x00000020  // High-Speed Enable
#define UART_CTL_EOT             0x00000010  // End of Transmission
#define UART_CTL_SMART           0x00000008  // ISO 7816 Smart Card Support
#define UART_CTL_SIRLP           0x00000004  // UART SIR Low-Power Mode
#define UART_CTL_SIREN           0x00000002  // UART SIR Enable
#define UART_CTL_UARTEN          0x00000001  // UART Enable
//@ref data_length_configuration
#define UART_data_len_8          0x00000060  // 8 bit data
#define UART_data_len_7          0x00000040  // 7 bit data
#define UART_data_len_6          0x00000020  // 6 bit data
#define UART_data_len_5          0x00000000  // 5 bit data
//@ref stop_bit_confiugration
#define UART_STOP_ONE    				 0x00000000  // One stop bit
#define UART_STOP_TWO    				 0x00000008  // Two stop bits
//@ref parity_bit_configuration
#define UART_PAR_NONE    				 0x00000000  // No parity
#define UART_PAR_EVEN    				 0x00000006  // Even parity
#define UART_PAR_ODD     0x00000002  // Odd parity
#define UART_PAR_ONE     0x00000086  // Parity bit is one
#define UART_PAR_ZERO    0x00000082  // Parity bit is zero
/**************************************************
 * ****************** APIs ************************
***************************************************/
void uart_init(void);
void UART_Send_string(char data[]);
#endif