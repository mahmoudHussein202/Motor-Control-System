#include "eeprom_s.h"
uint32_t ui32EEPROMInit;
uint32_t pui32Data[2];
uint32_t pui32Read[2];

void eeprom_init()
{ 
//[1] Enable the EEPROM module.
SysCtlPeripheralEnable(SYSCTL_PERIPH_EEPROM0);
//[2] Wait for the EEPROM module to be ready.
while(!SysCtlPeripheralReady(SYSCTL_PERIPH_EEPROM0)){}
//[3] Wait for the EEPROM Initialization to complete
ui32EEPROMInit = EEPROMInit();
// Check if the EEPROM Initialization returned an error
// and inform the application
if(ui32EEPROMInit != EEPROM_INIT_OK)
{
    while(1){}
}
}
