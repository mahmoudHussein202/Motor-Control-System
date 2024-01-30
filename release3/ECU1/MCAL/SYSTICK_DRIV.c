#include "../MCAL/MCAL.h"
#include "SYSTICK_DRIV.h".h"
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
    //[1] enable clock
    systick_ctl_reg|= ST_CTRL_CLK_SRC | ST_CTRL_ENABLE;
    //[2] set handler and enable int.
    IntRegister(15, SysTick_Handler);
    systick_ctl_reg|= ST_CTRL_INTEN;
    //[3] enable systick
    systick_ctl_reg|= ST_CTRL_INTEN;
    //[4] set period
    systick_data_reg = counts -1 ; 
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