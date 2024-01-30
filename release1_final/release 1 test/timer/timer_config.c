#include "timer_config.h"
#include "driverlib/timer.h"
#include "driverlib/sysctl.h"
#include "inc/hw_memmap.h"
#include <stdint.h>
#include <stdbool.h>
#if timer_select == SYSCTL_PERIPH_TIMER0
#define timer_base_address   TIMER0_BASE
#endif
#if timer_1_select == SYSCTL_PERIPH_TIMER1
#define timer_1_base_address   TIMER1_BASE
#endif 
uint8_t isr_0 = 0;
void TIMER_0_ISR();
void TIMER_1_ISR();
void timer_0_init()
{
   SysCtlPeripheralEnable(timer_select);
   //while(!SysCtlPeripheralReady(timer_select));
   TimerConfigure(timer_base_address,timer_configuration);
   TimerLoadSet(timer_base_address,TIMER_A,timer_load_value);
   TimerIntEnable(timer_base_address,TIMER_TIMA_TIMEOUT);
   // enable global interrupt
   TimerIntRegister(timer_base_address,TIMER_A,TIMER_0_ISR);
   TimerEnable(timer_base_address,TIMER_A);
}
void timer_1_init()
{
   SysCtlPeripheralEnable(timer_1_select);
   while(!SysCtlPeripheralReady(timer_1_select));
   TimerConfigure(timer_1_base_address,timer_1_configuration);
   TimerLoadSet(timer_1_base_address,TIMER_A,timer_1_load_value);
   TimerIntEnable(timer_1_base_address,TIMER_TIMA_TIMEOUT);
   // enable global interrupt
   TimerIntRegister(timer_1_base_address,TIMER_A,TIMER_1_ISR);
   TimerEnable(timer_1_base_address,TIMER_A);
}

void TIMER_0_ISR()
{
		isr_0 ^= 1<<0;
}
void TIMER_1_ISR()
{

}
