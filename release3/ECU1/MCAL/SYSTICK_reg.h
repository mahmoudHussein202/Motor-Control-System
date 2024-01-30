#ifndef _SYSTICK_REG_H_
#define _SYSTICK_REG_H_
//*****************************************************************************
//  bit fields in the NVIC_ST_CTRL register.
//*****************************************************************************
#define ST_CTRL_COUNT      0x00010000  // Count flag
#define ST_CTRL_CLK_SRC    0x00000004  // Clock Source
#define ST_CTRL_INTEN      0x00000002  // Interrupt enable
#define ST_CTRL_ENABLE     0x00000001  // Counter mode
//*****************************************************************************
// systick control registers
//*****************************************************************************
#define systick_ctl_reg     (*((volatile unsigned long *)(0xE000E010)))
#define systick_data_reg    (*((volatile uint32_t *)(0xE000E014)))
#endif