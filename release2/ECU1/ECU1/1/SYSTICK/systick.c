#include <stdint.h>
#include <stdbool.h>
#include "driverlib/systick.h"
#include "driverlib/sysctl.h"
#include "driverlib/timer.h"
#include "systick.h"
#include "../OS/OS.h"
#define test_ECU2_Functionality 0
void SysTick_Handler(void);
#define NULL (void*)0
static p2f Glob_p2KeepAliveCallback = NULL ; 
static p2f Glob_p2SendToPc = NULL ; 
static p2f Glob_p2CheckCom = NULL ; 
/***********************************
************* test  **************
*********************************/
extern uint32_t test_ECU2_temp;

void systick_init(p2f p2KeepAliveCallback, p2f p2SendToPc , p2f p2CheckConnections)
{
        Glob_p2KeepAliveCallback = p2KeepAliveCallback ; 
        Glob_p2SendToPc = p2SendToPc ; 
        Glob_p2CheckCom = p2CheckConnections ; 
    
    SysTickEnable();
    SysTickIntRegister(SysTick_Handler);
    SysTickIntEnable();
    SysTickPeriodSet(15999999/2);
}

// this routine will execute after every one second

void SysTick_Handler(void)
{
    static int counter = 0;
    counter++ ; 
    if (counter >= 1)
    {
        Glob_p2KeepAliveCallback();
				#if test_ECU2_Functionality ==1
					APP_InterruptBased_GetCANmsg('^');
				#endif
    }
    if (counter%2==0)
    {
        Glob_p2SendToPc();
				#if test_ECU2_Functionality ==1
					APP_InterruptBased_GetCANmsg(test_ECU2_temp);
				#endif
    }
    if(counter%10==0)
    {
        // 5 sec 
			  counter = 0; 
        Glob_p2CheckCom();
    }


 }