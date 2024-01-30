/*
 * sysclk.c
 *
 *  Created on: Jan 13, 2024
 *      Author: Abnaby
 */

#ifndef LAYERED_ARCH_MCAL_SYSCLK_C_
#define LAYERED_ARCH_MCAL_SYSCLK_C_

#include "sysclk.h"
#include <stdint.h>
#include <stdbool.h>

ReturnState sysclk_setSystemClk(void)
{
    IntMasterDisable();
    ReturnState retVal ;
    retVal = E_OK ;
    (void)retVal;
    uint32_t Loc_u32ClkSpeed = 0 ;

#if   CPU_CLK_VAL == SYS_CLK_8_MHZ
    SysCtlClockSet(SYSCTL_SYSDIV_2 | SYSCTL_USE_OSC | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);
    Loc_u32ClkSpeed = SysCtlClockGet() ;
    if(Loc_u32ClkSpeed != 8000000)
        retVal = E_NOT_OK ;
#else
    SysCtlClockSet(SYSCTL_SYSDIV_1 | SYSCTL_USE_OSC | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);
    Loc_u32ClkSpeed = SysCtlClockGet() ;
    (void)retVal;
    if(Loc_u32ClkSpeed != 16000000)
        retVal = E_NOT_OK ;
#endif
    IntMasterEnable();
    return E_OK;
}
ReturnState sysclk_setPeripheralEnable(uint32_t ui32GPIOPeripheral)
{
    IntMasterDisable();
    ReturnState retVal ;
    retVal = E_OK ;
    (void)retVal;
    SysCtlPeripheralEnable(ui32GPIOPeripheral);
    IntMasterEnable();
    return E_OK;
}

ReturnState sysclk_setPeripheralDisable(uint32_t ui32GPIOPeripheral)
{
    IntMasterDisable();
    ReturnState retVal ;
    retVal = E_OK ;
    (void)retVal;
    SysCtlPeripheralDisable(ui32GPIOPeripheral);
    IntMasterEnable();
    return E_OK;
}

#endif /* LAYERED_ARCH_MCAL_SYSCLK_C_ */
