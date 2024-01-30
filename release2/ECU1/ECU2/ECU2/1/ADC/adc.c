/* TM4C123G Tiva C Series ADC Example */

/* This Program measures analog voltage from analog Channel zero */
/* Also sends measured voltage value to computer virtual terminal using UART */
#include "tm4c123gh6pm.h"
#include "stdint.h"
#include "stdbool.h"
#include "adc.h"
/*************Macros************/
#define ADC_select_CH0  0
#define ADC_select_CH1  1
/*****************************/
unsigned int adc_value;
float adc_1_voltage ;
float adc_0_voltage ;

void ADC_DRIV_Init()
{
    /* Enable Clock to ADC0 and GPIO pins*/
    SYSCTL->RCGCGPIO |= (1<<4);   /* Enable Clock to GPIOE or PE3/AN0 */
    SYSCTL->RCGCADC |= (1<<0);    /* AD0 clock enable*/
    
    /* initialize PE3 and PE2for AIN0 input  */
    GPIOE->AFSEL |= (1<<3);       /* enable alternate function */
	  GPIOE->AFSEL |= (1<<2);
    GPIOE->DEN &= ~(1<<3);        /* disable digital function */
		GPIOE->DEN &= ~(1<<2); 
    GPIOE->AMSEL |= (1<<3);       /* enable analog function */
		GPIOE->AMSEL |= (1<<2);
   
    /* initialize sample sequencer3 */
    ADC0->ACTSS &= ~(1<<3);        /* disable SS3 during configuration */
    ADC0->EMUX &= ~0xF000;    /* software trigger conversion */
             
    ADC0->SSCTL3 |= (1<<1)|(1<<2);        /* take one sample at a time, set flag at 1st sample */
    ADC0->ACTSS |= (1<<3);         /* enable ADC0 sequencer 3 */
}
/*
- @brief: get input voltage of the selected channel
- @param: channel_num :  ADC_select_CH0, ADC_select_CH1
- @return: input voltage on the selected channel
*/
float ADC_get_volt(uint8_t channel_num)
{
    if (channel_num == ADC_select_CH1)
    {
        ADC0->SSMUX3 = 1;					    /* get input from channel 1 */
        ADC0->PSSI |= (1<<3);                   /* Enable SS3 conversion or start sampling data from AN0 */
        while((ADC0->RIS & 8) == 0) ;           /* Wait untill sample conversion completed*/
        adc_value = ADC0->SSFIFO3;              /* read adc coversion result from SS3 FIFO*/
        ADC0->ISC = 8;                          /* clear coversion clear flag bit*/	  
				adc_1_voltage = (adc_value * 0.0008);	/* convert digital value back into voltage */
        return adc_1_voltage;
    }
    else
    {
        ADC0->SSMUX3 = 0;					    /* get input from channel 0 */
        ADC0->PSSI |= (1<<3);                   /* Enable SS3 conversion or start sampling data from AN0 */
        while((ADC0->RIS & 8) == 0) ;           /* Wait untill sample conversion completed*/
        adc_value = ADC0->SSFIFO3;              /* read adc coversion result from SS3 FIFO*/
        ADC0->ISC = 8;                          /* clear coversion clear flag bit*/
		adc_0_voltage = (adc_value * 0.0008);   /* convert digital value back into voltage */
        return adc_0_voltage;
    }

}
