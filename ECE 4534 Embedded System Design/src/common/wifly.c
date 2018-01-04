#include "wifly.h"
#include "wifly_public.h"
#include "debug.h"
#include "message.h"
#include "navigation_public.h"
#include "json_parse.h"
#include "config.h"
#include "test_public.h"
#include "lidar_public.h"
#include "motor_public.h"
#include <stdio.h>

WIFLY_DATA wiflyData;

//from wifly_public.h
//externally facing queue that directs straight to transmit ISR
void wifly_SendValToMessageQ(wifly_message_t* val){
    if(xQueueSend( wiflyData.wiflyQueue_h, val, 0U) == errQUEUE_FULL){
        dbgError(ERROR_SENDING_TO_QUEUE);
    }
    //start sending data
    wiflyData.message_count++;
    PLIB_INT_SourceEnable(INT_ID_0, WIFLY_TX_INT_SRC );
}

void wifly_SendValToMessageQFromISR(wifly_message_t* val){
    //static to minimize stack usage
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    if(xQueueSendFromISR( wiflyData.wiflyQueue_h, val, &xHigherPriorityTaskWoken) == errQUEUE_FULL){
        dbgError(ERROR_SENDING_TO_QUEUE);
    }
    //check xHigherPriorityTaskWoken
    if(xHigherPriorityTaskWoken){
        portEND_SWITCHING_ISR( xHigherPriorityTaskWoken );
    }
    //start sending data
    wiflyData.message_count++;
    PLIB_INT_SourceEnable(INT_ID_0, WIFLY_TX_INT_SRC );
}

//Internal queue for processing received messages
void _wifly_internal_SendRecvVal(wifly_message_t* val){
    if(xQueueSend( wiflyData.receiveQueue_h, val, 0U) == errQUEUE_FULL){
        dbgError(ERROR_SENDING_TO_QUEUE);
    }
}

void _wifly_internal_SendRecvValFromISR(wifly_message_t* val){
    //static to minimize stack usage
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    if(xQueueSendFromISR( wiflyData.receiveQueue_h, val, &xHigherPriorityTaskWoken) == errQUEUE_FULL){
        dbgError(ERROR_SENDING_TO_QUEUE);
    }
    //check xHigherPriorityTaskWoken
    if(xHigherPriorityTaskWoken){
        portEND_SWITCHING_ISR( xHigherPriorityTaskWoken );
    }
}

digest_result digest_metadata(uint32_t* checksum, uint32_t* calculatedChecksum, uint32_t* seqNumber){
    //check checksums
    if( *checksum != *calculatedChecksum ){
        //output to wifi
        //dbgToServer( JSON_PROCESSING_ERROR,  );
        return BAD_CHECKSUM;
    }
    //else
    //check seq numbers
    if( *seqNumber != wiflyData.seq_number ){
        //output to wifly
        //dbgToServer( JSON_PROCESSING_ERROR );
        //set to new number
        return BAD_SEQ_NUMBER;
    }
    //else
    return META_GOOD;
}

uint32_t get_seq_num(){
    return wiflyData.seq_number;
}

void increment_seq_num(){
    wiflyData.seq_number++;
}

bool wifly_MessageQFull(){
    return uxQueueSpacesAvailable( wiflyData.wiflyQueue_h ) == 0;
}

void rateTimerCallback( TimerHandle_t xTimer ){
    //get message count and put into json
    static message_t sendData;
    static MESSAGE_DATA data;
    static wifly_message_t sendMsg;
    
    data.val_ui32 = wiflyData.message_count/5;
    sendData = message_factory(PIC_MESSAGE_RATE, UINT32, data);
    //get JSON
    json_from_message_t( &sendData, &sendMsg );
    //send
    wifly_SendValToMessageQ( &sendMsg );
    increment_seq_num();
    //reset
    wiflyData.message_count = 0;
    
}

void WIFLY_Initialize ( void )
{
    //init debug if needed
    if( !isDebugInit() )
        debugInit();
    
    dbgOutputLoc( WIFLY_TASK_INIT );
    
    //create the queue for transmitting
    wiflyData.wiflyQueue_h = xQueueCreate( 75, sizeof( wifly_message_t ) );
    wiflyData.receiveQueue_h = xQueueCreate( 75, sizeof( wifly_message_t ));
    
    //check queue create
    if( wiflyData.wiflyQueue_h == NULL){ //fail
        dbgError( ERROR_CREATING_QUEUE );
    }
    if( wiflyData.receiveQueue_h == NULL){ //fail
        dbgError( ERROR_CREATING_QUEUE );
    }
    //create rate timer
    wiflyData.rateTimer_h = xTimerCreate(
            "Rate Timer",
            ONE_SEC*5,
            pdTRUE,
            (void *) 0,
            rateTimerCallback
        );
    if( wiflyData.rateTimer_h == NULL ){
        dbgError( ERROR_STARING_TIMER );
    }else{ //start
        if( xTimerStart( wiflyData.rateTimer_h, 0) != pdPASS ){
            dbgError( ERROR_STARING_TIMER );
        }
    }
    //message count
    wiflyData.message_count = 0;
    
    //open uart
    PLIB_USART_Enable( USART_ID_1 );
    PLIB_USART_TransmitterInterruptModeSelect( USART_ID_1, USART_TRANSMIT_FIFO_NOT_FULL );
    PLIB_USART_ReceiverInterruptModeSelect( USART_ID_1, USART_RECEIVE_FIFO_ONE_CHAR );
    //disable tx int
    PLIB_INT_SourceDisable(INT_ID_0, WIFLY_TX_INT_SRC );
    
    //other data
    wiflyData.seq_number = 0;
}


/******************************************************************************
  Function:
    void WIFLY_Tasks ( void )

  Remarks:
    See prototype in wifly.h.
 */

void WIFLY_Tasks ( void )
{
    //ERROR var
    DEBUG_CONSTANTS error;
    
    SAFE(dbgOutputLoc( WIFLY_TASK_ENTER ), error);
    SAFE(dbgOutputLoc( WIFLY_TASK_BEFORE_WHILE ), error);
    
    //local data
    wifly_message_t recvData, dbgData;
    message_t sendData;
    MESSAGE_DATA dataUnion;
    
    //data for parsing JSON
    message_t parseData;
    uint32_t checksum, calculatedChecksum, seqNumber;
    
    //request config values
    sendData = message_factory(SERVER_GET_CONFIG, UINT32, dataUnion);
    json_from_message_t( &sendData, &dbgData );
    wifly_SendValToMessageQ( &dbgData );
    increment_seq_num();
    while( 1 ){
        //wait for queue
        SAFE(dbgOutputLoc( WIFLY_TASK_WAIT_FOR_QUEUE ), error);
        if( xQueueReceive( wiflyData.receiveQueue_h, &recvData, portMAX_DELAY) == pdFALSE )
            dbgError(ERROR_RECEIVEING_FROM_QUEUE);
        
        SAFE( dbgOutputLoc( WIFLY_TASK_GOT_DATA_FROM_QUEUE ), error);
        
        bool success = true;
        //wifly_SendValToMessageQ( &recvData );
        //parse data from json
        //make sure that json format is correct
        if( get_values( recvData.buffer, &checksum, &calculatedChecksum, &seqNumber, &parseData.cmd, &parseData.type, &parseData.data) != EXPECTED_TOKENS ){
            dbgData.size = sprintf(
                    dbgData.buffer,
                    "{\"DEBUG\":\"Error parsing JSON, unexpected or missing data, or bad format\"}"
                );
            wifly_SendValToMessageQ( &dbgData );
            success = false;
            //dbgToServer( JSON_PROCESSING_ERROR, 0 );
        }
        SAFE( dbgOutputLoc( WIFLY_GOT_VALUES ), error);
        //else we are good
        //check metadata for errors
        switch( digest_metadata( &checksum, &calculatedChecksum, &seqNumber ) ){
            case BAD_CHECKSUM:
                dbgData.size = sprintf(
                    dbgData.buffer,
                    "{\"DEBUG\":\"Error parsing JSON, bad checksum. got: %u, calculated: %u\"}",
                    checksum, 
                    calculatedChecksum 
                );
                SAFE( dbgOutputLoc( WIFLY_BAD_CHECKSUM ), error);
                success = false;
                wifly_SendValToMessageQ( &dbgData );
                break;
            case BAD_SEQ_NUMBER:
                dbgData.size = sprintf(
                    dbgData.buffer,
                    "{\"DEBUG\":\"Error parsing JSON, seq number. Expected %u\"}",
                    wiflyData.seq_number
                );
                SAFE( dbgOutputLoc( WIFLY_BAD_SEQ_NUM ), error);
                success = false;
                //handle new seq_number
                wiflyData.seq_number = seqNumber;
                wifly_SendValToMessageQ( &dbgData );
                break;
            default:
                break;
        }
        SAFE( dbgOutputLoc( WIFLY_CHECKED_META ), error);
        if(success){
            dbgData.size = sprintf(
                dbgData.buffer,
                "{\"DEBUG\":\"SUCCESS; Got message with command: %x\"}",
                    parseData.cmd
            );
            wifly_SendValToMessageQ( &dbgData );
        }
        //always
        wiflyData.seq_number++;
        wiflyData.message_count++;
        //use values to do stuff
        //process
        switch( parseData.cmd ){
            //check config values
            case PIC_CONFIG_ENABLE_TESTING:
                set_enable_testing(parseData.data.val_bool);
                dbgData.size = sprintf(
                        dbgData.buffer,
                        "{\"DEBUG\":\"Got Testing Enable: %u\"}",
                        parseData.data.val_bool
                    );
                wifly_SendValToMessageQ( &dbgData );
                break;
            case PIC_CONFIG_TEST_NUMBER:
                set_test_number(parseData.data.val_ui32);
                dbgData.size = sprintf(
                        dbgData.buffer,
                        "{\"DEBUG\":\"Got Testing Number: %u\"}",
                        parseData.data.val_ui32
                    );
                wifly_SendValToMessageQ( &dbgData );
                break;
            case PIC_CHANGE_TEST_PERIOD:
                changeTestPeriod(parseData.data.val_ui32);
                break;
            case MESSAGE_RATE_ENABLE:
                if( parseData.data.val_bool ){
                    if( xTimerStart( wiflyData.rateTimer_h, 0) != pdPASS ){
                        dbgError( ERROR_STARING_TIMER );
                    }
                }else{
                    xTimerStop( wiflyData.rateTimer_h,0);
                }
                break;
            case PIC_NAV_START:
            case PIC_NAV_STOP:
            case PIC_NAV_SET_DEST:
            case PIC_NAV_SET_START:
            case PIC_GET_PATH:
            case PIC_GET_MAP:
            case PIC_PEEK_MAP:
            case PIC_MAP_POINT:
            case PIC_GET_CUR_POS:
            case PIC_GET_POINT:
            case PIC_MAP_POINT_BLOCK:
            case SERVER_GET_MAP:
                //forward to navigation module
                nav_SendValToMessageQ(&parseData);
                break;
            case PIC_CHANGE_LIDAR_DIR:
            case PIC_START_STOP_LIDAR:
                //forward to lidar
                lidar_SendValToMessageQ(&parseData);
                break;
            case MANUAL_FORWARD:
            case MANUAL_BACKWARD:
            case MANUAL_LEFT:
            case MANUAL_RIGHT:
                //forward to the motor
                motor_SendValToMessageQ(&parseData);
                break;
        }
        
        SAFE( dbgOutputLoc( WIFLY_TASK_PROCESS_DATA ), error);
        
    }
    SAFE(dbgOutputLoc( WIFLY_TASK_LEAVING ), error);
}

//send json string
void wifly_txInterruptHandler(){
    dbgOutputLoc(WIFLY_TX_INT);
    //ISR local variables
    //INIT once because static 
    static TX_STATE txState = BEGIN;
    static wifly_message_t valToSend;
    static size_t buffer_pos = 0;
    //context switching
    static BaseType_t xTaskWoken = pdFALSE;
    
    switch( txState ){
        case BEGIN: //waiting to start
            //first get the string
            dbgOutputLoc(WIFLY_TX_BEGIN);
            if( xQueueReceiveFromISR( wiflyData.wiflyQueue_h, &valToSend, &xTaskWoken) == pdFALSE ){
                dbgError(ERROR_RECEIVEING_FROM_QUEUE);
            } else {
                //check xTaskWoken
                if( xTaskWoken != pdFALSE ){
                    portEND_SWITCHING_ISR( xTaskWoken );
                } 
                //next state (send body of message)
                txState = SENDING;
                //send begin delim
                PLIB_USART_TransmitterByteSend( USART_ID_1, BEGIN_MSG_DELIM );
            }
            break;
        case SENDING:
            //send current byte
            //if at the end
            dbgOutputLoc(WIFLY_TX_SENDING);
            if( buffer_pos >= valToSend.size ){
                //go to next stage 
                txState = END;
                break;
            }else{
                //send current byte
                PLIB_USART_TransmitterByteSend( USART_ID_1, valToSend.buffer[buffer_pos] );
                //go to next byte
                buffer_pos++;
            }
            break;
        case END:
            //send the last byte
            dbgOutputLoc(WIFLY_TX_END);
            PLIB_USART_TransmitterByteSend( USART_ID_1, END_MSG_DELIM );
            //set state
            txState = BEGIN;
            buffer_pos = 0;
            //diable int (if no messages to send)
            if( xQueueIsQueueEmptyFromISR( wiflyData.wiflyQueue_h ) != pdFALSE ){ //if no messages are waiting
                PLIB_INT_SourceDisable(INT_ID_0, WIFLY_TX_INT_SRC );
            }
            //else do nothing
            break;
    }
    PLIB_INT_SourceFlagClear(INT_ID_0,WIFLY_TX_INT_SRC);
}

//build message
void wifly_rxInterruptHandler(){
    dbgOutputLoc(WIFLY_RX_INT);
    
    //static local vars
    static wifly_message_t recvMessage;
    static RX_STATE rxState = IDLE;
    static wifly_message_t dbg;
    
    //get byte
    char byte = PLIB_USART_ReceiverByteReceive( USART_ID_1 );
    
    switch( rxState ){
        case IDLE:
            //check begin message delim
            if( byte == BEGIN_MSG_DELIM ){
                rxState = RECEIVING;
            }
            //else do nothing
            break;
        case RECEIVING:
            //check rxPos for buffer overlfow
            if( recvMessage.size >= BUFFER_SIZE){
                dbgError(BUFFER_OVERFLOW_PROTECTION);
            }else if( byte == END_MSG_DELIM ){
                dbg.size = sprintf(
                    dbg.buffer,
                    "{\"DEBUG\":\"Got Message in ISR: %s\"}",
                        recvMessage.buffer
                );
                wifly_SendValToMessageQFromISR( &dbg );
                //send message to wifly thread for processing
                _wifly_internal_SendRecvValFromISR(&recvMessage);
                
                //send back to IDLE
                rxState = IDLE;
                //reset data
                recvMessage.size = 0;
                memset(recvMessage.buffer,0,BUFFER_SIZE);
                break;
            }
            //load and add to buffer
            recvMessage.buffer[recvMessage.size] = byte;
            //increment pos
            recvMessage.size++;
            break;
        default:
            break;
    }
    PLIB_INT_SourceFlagClear(INT_ID_0,WIFLY_RX_INT_SRC);
}