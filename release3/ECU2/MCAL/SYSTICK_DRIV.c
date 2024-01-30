#include "../../MCAL/MCAL.h"
#include "SYSTICK_DRIV.h"
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