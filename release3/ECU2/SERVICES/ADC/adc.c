/* TM4C123G Tiva C Series ADC Example */

/* This Program measures analog voltage from analog Channel zero */
/* Also sends measured voltage value to computer virtual terminal using UART */
#include "../../MCAL/MCAL.h"
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
    /* Enable Clock to ADC0 and GPIO pins */
    SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R4;   /* Enable Clock to GPIOE or PE3/AN0 */
    SYSCTL_RCGCADC_R |= SYSCTL_RCGCADC_R0;    /* AD0 clock enable */
    
    /* Initialize PE3 and PE2 for AIN0 input */
    GPIO_PORTE_AFSEL_R |= GPIO_PIN_3 | GPIO_PIN_2;       /* enable alternate function */
    GPIO_PORTE_DEN_R &= ~(GPIO_PIN_3 | GPIO_PIN_2);      /* disable digital function */
    GPIO_PORTE_AMSEL_R |= GPIO_PIN_3 | GPIO_PIN_2;       /* enable analog function */
   
    /* Initialize sample sequencer 3 */
    ADC0_ACTSS_R &= ~ADC_ACTSS_ASEN3;        /* disable SS3 during configuration */
    ADC0_EMUX_R &= ~ADC_EMUX_EM3_M;    /* software trigger conversion */
             
    ADC0_SSCTL3_R |= ADC_SSCTL3_END0 | ADC_SSCTL3_IE0;        /* take one sample at a time, set flag at 1st sample */
    ADC0_ACTSS_R |= ADC_ACTSS_ASEN3;         /* enable ADC0 sequencer 3 */
}
/*
- @brief: get input voltage of the selected channel
- @param: channel_num :  ADC_select_CH0, ADC_select_CH1
- @return: input voltage on the selected channel
*/
float ADC_get_volt(uint8_t channel_num)
{
  #define ADC_SSMUX3_MUX1 0x00000001  // AIN1 (Analog Input 1) for ADC Sample Sequencer 3
  #define ADC_SSMUX3_MUX0 0x00000000  // AIN0 (Analog Input 0) for ADC Sample Sequencer 3
    if (channel_num == ADC_select_CH1)
    {
        ADC0_SSMUX3_R = ADC_SSMUX3_MUX1;          /* get input from channel 1 */
        ADC0_PSSI_R |= ADC_PSSI_SS3;              /* Enable SS3 conversion or start sampling data from AN0 */
        while((ADC0_RIS_R & ADC_RIS_INR3) == 0) ; /* Wait until sample conversion completed */
        adc_value = ADC0_SSFIFO3_R;               /* read ADC conversion result from SS3 FIFO */
        ADC0_ISC_R = ADC_ISC_IN3;                 /* clear conversion clear flag bit */
        adc_1_voltage = (adc_value * 0.0008);     /* convert digital value back into voltage */
        return adc_1_voltage;
    }
    else
    {
        ADC0_SSMUX3_R = ADC_SSMUX3_MUX0;          /* get input from channel 0 */
        ADC0_PSSI_R |= ADC_PSSI_SS3;              /* Enable SS3 conversion or start sampling data from AN0 */
        while((ADC0_RIS_R & ADC_RIS_INR3) == 0) ; /* Wait until sample conversion completed */
        adc_value = ADC0_SSFIFO3_R;               /* read ADC conversion result from SS3 FIFO */
        ADC0_ISC_R = ADC_ISC_IN3;                 /* clear conversion clear flag bit */
        adc_0_voltage = (adc_value * 0.0008);     /* convert digital value back into voltage */
        return adc_0_voltage;
    }
}
