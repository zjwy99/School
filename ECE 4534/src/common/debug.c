/**
 * 
 */

#include "debug.h"

DEBUG_DATA data = {NULL,false,0};

void dbg_txInterruptHandler(){
    PLIB_USART_TransmitterByteSend( USART_ID_4, data.sendData);
    //disable int source
    PLIB_INT_SourceDisable(INT_ID_0, DBG_UART_INT_SOURCE_TX );
}

DEBUG_CONSTANTS dbgUARTVal(const __uint8_t val){
    //check if init
    if(!data.initialized){
        return UNINITIALIZED_ERROR;
    }
    //else
    //start interrupt
    //set data
    data.sendData = val;
    PLIB_INT_SourceEnable(INT_ID_0, DBG_UART_INT_SOURCE_TX );
    //DRV_USART_WriteByte(data.uartHandle, val);
    return SUCCESS;
}

DEBUG_CONSTANTS dbgOutputVal( const __uint8_t data){
    uint8_t seven =  (data>>7) & 0x01;
    uint8_t six   =  (data>>6) & 0x01;
    uint8_t five  =  (data>>5) & 0x01;
    uint8_t four  =  (data>>4) & 0x01;
    uint8_t three =  (data>>3) & 0x01;
    uint8_t two   =  (data>>2) & 0x01;
    uint8_t one =  (data>>1) & 0x01;
    uint8_t zero  =  (data>>0) & 0x01;
    
    SYS_PORTS_PinWrite(PORTS_ID_0, PORT_CHANNEL_G, PORTS_BIT_POS_1, zero);
    SYS_PORTS_PinWrite(PORTS_ID_0, PORT_CHANNEL_G, PORTS_BIT_POS_0, one);
    SYS_PORTS_PinWrite(PORTS_ID_0, PORT_CHANNEL_A, PORTS_BIT_POS_6, two);
    SYS_PORTS_PinWrite(PORTS_ID_0, PORT_CHANNEL_A, PORTS_BIT_POS_7, three);
    SYS_PORTS_PinWrite(PORTS_ID_0, PORT_CHANNEL_G, PORTS_BIT_POS_14, four);
    SYS_PORTS_PinWrite(PORTS_ID_0, PORT_CHANNEL_G, PORTS_BIT_POS_12, five);
    SYS_PORTS_PinWrite(PORTS_ID_0, PORT_CHANNEL_G, PORTS_BIT_POS_13, six);
    SYS_PORTS_PinWrite(PORTS_ID_0, PORT_CHANNEL_A, PORTS_BIT_POS_9, seven);
    return SUCCESS;
}

DEBUG_CONSTANTS dbgOutputLoc( DEBUG_LOCATION data ){
    SYS_PORTS_Write(PORTS_ID_0, PORT_CHANNEL_E, data);
    return SUCCESS;
}

void dbgError( DEBUG_CONSTANTS errorCode){
    dbgOutputLoc(DEBUG_FATAL_ERROR);
    while(1){
        if(dbgUARTVal(errorCode) == UNINITIALIZED_ERROR){
            debugInit();
        }else{
            dbgUARTVal(errorCode);
            dbgOutputVal(errorCode);
        }
    }
}

void debugInit(){
    data.uartHandle = DRV_USART_Open( DRV_USART_INDEX_1, DRV_IO_INTENT_WRITE );
    PLIB_INT_SourceDisable(INT_ID_0, DBG_UART_INT_SOURCE_TX );
    data.initialized = true;
    dbgOutputLoc(DEBUG_INIT);
}

bool isDebugInit(){
    return data.initialized;
}
