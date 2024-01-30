#ifndef _SYSTICK_H_
#define _SYSTICK_H_
#include <stdint.h>
#include <stdbool.h>
typedef void (*p2f)(uint8_t) ; 
void systick_init(p2f send_Temp);
#endif
