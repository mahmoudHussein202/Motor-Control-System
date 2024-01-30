#ifndef _SYSTICK_H_
#define _SYSTICK_H_
#include <stdint.h>
#include <stdbool.h>
typedef void (*p2f)(void) ; 
void systick_init(p2f p2KeepAliveCallback, p2f p2SendToPc , p2f p2CheckConnections);
#endif
