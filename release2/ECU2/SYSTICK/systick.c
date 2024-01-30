#include <stdint.h>
#include <stdbool.h>
#include "driverlib/systick.h"
#include "driverlib/sysctl.h"
#include "driverlib/timer.h"
#include "systick.h"
#define test_ECU2_Functionality 0
void SysTick_Handler(void);
#define NULL (void*)0
static p2f ECU2_Send_temp = NULL ; 
static p2f Glob_p2SendToPc = NULL ; 
static p2f Glob_p2CheckCom = NULL ; 
extern uint8_t sending_volt_flag;
/***********************************
************* test  **************
*********************************/
extern uint32_t test_ECU2_temp;
extern uint8_t test_ECU2_volt;

void systick_init(p2f send_Temp)
{
        ECU2_Send_temp = send_Temp ; 
    
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
			// 500 ms
        
    }
    if (counter%2==0)
    {
			// 1 sec
			ECU2_Send_temp(test_ECU2_temp);
    }
    if(counter%10==0)
    {
      // 5 sec 
			  counter = 0; 
    }


 }