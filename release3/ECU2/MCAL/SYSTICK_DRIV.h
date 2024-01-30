#ifndef _SYSTICK_H_
#define _SYSTICK_H_
#include <stdint.h>
#include <stdbool.h>
#include "SYSTICK_cfg.h"
#include "SYSTICK_DRIV.h"
#include "SYSTICK_reg.h"
typedef void (*p2f)(uint8_t) ; 
typedef struct 
{
    uint8_t systick_clk_src;
    uint8_t interrupt_enable;
    p2f systick_handler;  
}systick_config_t;
void systick_init(p2f send_Temp);
#endif
