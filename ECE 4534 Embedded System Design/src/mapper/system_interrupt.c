// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include "system/common/sys_common.h"
#include "navigation_public.h"
#include "debug.h"
#include "message.h"
#include "wifly.h"
#include "wifly_public.h"
#include "motor.h"
#include "irreader.h"
#include "irreader_public.h"
#include "system_definitions.h"
#include <stdio.h>

// *****************************************************************************
// *****************************************************************************
// Section: System Interrupt Vector Functions
// *****************************************************************************
// *****************************************************************************


void IntHandlerDrvI2CInstance0(void)
{
    DRV_I2C_Tasks(sysObj.drvI2C0);
 
}
     
 
   

 
 
 

 




void IntHandlerDrvUsartInstance0(void)
{
    if( SYS_INT_SourceStatusGet( WIFLY_RX_INT_SRC ) ){
        wifly_rxInterruptHandler();
    }else if( SYS_INT_SourceStatusGet( WIFLY_TX_INT_SRC ) ){
        wifly_txInterruptHandler();
    }
}

void IntHandlerDrvUsartInstance1(void)
{
    //check tx int
    if(SYS_INT_SourceStatusGet( DBG_UART_INT_SOURCE_TX ))
        dbg_txInterruptHandler();
}

void IntHandlerDrvAdc(void)
{
    /* Clear ADC Interrupt Flag */
    ADC_parse();
    PLIB_INT_SourceFlagClear(INT_ID_0, INT_SOURCE_ADC_1);
}

void IntHandlerDrvTmrInstance0(void)
{
    PLIB_INT_SourceFlagClear(INT_ID_0,INT_SOURCE_TIMER_2);
}
void IntHandlerDrvTmrInstance1(void)
{
    update_tick();
    PLIB_INT_SourceFlagClear(INT_ID_0,INT_SOURCE_TIMER_3);
}
 
 /*******************************************************************************
 End of File
*/
