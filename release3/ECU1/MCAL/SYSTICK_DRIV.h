#ifndef _SYSTICK_H_
#define _SYSTICK_H_
#include "stdint.h"
#include "systick_reg.h"
#include "systick_cfg.h"
typedef void (*p2f)(void) ; 
typedef struct 
{
    uint8_t systick_clk_src;
    uint8_t interrupt_enable;
    p2f systick_handler;  
}systick_config_t;
void systick_init(p2f p2KeepAliveCallback, p2f p2SendToPc , p2f p2CheckConnections);
#endif