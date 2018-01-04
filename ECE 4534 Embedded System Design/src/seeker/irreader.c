// *****************************************************************************
// *****************************************************************************
// Section: Included Files 
// *****************************************************************************
// *****************************************************************************

#include "irreader.h"
#include "irreader_public.h"
#include "wifly_public.h"
#include "message.h"
#include "debug.h"
#include "queue.h"
#include "wifly_public.h"
#include <stdio.h>
// *****************************************************************************
/* Application Data

  Summary:
    Holds application data

  Description:
    This structure holds the application's data.

  Remarks:
    This structure should be initialized by the APP_Initialize function.
    
    Application strings and buffers are be defined outside this structure.
*/

IR_READER_DATA ir_readerData;


// *****************************************************************************
// *****************************************************************************
// Section: Application Local Functions
// *****************************************************************************
// *****************************************************************************

//from ir_reader_public
void irReaderSendValToMessageQ(message_t* val){
    if(xQueueSend( ir_readerData.irQueue, val, 0U) == errQUEUE_FULL){
        dbgError(ERROR_SENDING_TO_QUEUE);
    }
}

void irReaderSendValToMessageQFromISR(message_t* val){
    //static to minimize stack usage
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    if(xQueueSendFromISR( ir_readerData.irQueue, val, &xHigherPriorityTaskWoken) == errQUEUE_FULL){
        dbgError(ERROR_SENDING_TO_QUEUE);
    }
    //check xHigherPriorityTaskWoken
    if(xHigherPriorityTaskWoken){
        portEND_SWITCHING_ISR( xHigherPriorityTaskWoken );
    }
}

/*******************************************************************************
  Function:
    void IR_READER_Initialize ( void )

  Remarks:
    See prototype in ir_reader.h.
 */

void IRREADER_Initialize ( void )
{
    dbgOutputLoc(IR_TASK_INIT);
    
    /* Initialize data */
    ir_readerData.objDetected = false;
    ir_readerData.parsedData = 0;

    //create the queue
    ir_readerData.irQueue = xQueueCreate( QUEUE_LENGTH, sizeof( uint16_t ) );
    //check value
    if(ir_readerData.irQueue == NULL)//error
        dbgError(ERROR_CREATING_QUEUE);
            
    // Enable the ADC
    DRV_ADC_Open();
}

void ADC_parse(void)
{
    int i;
    float sensorVal = 0.0;
    wifly_message_t dbgData;
    message_t sendData;
    MESSAGE_DATA dataUnion;
 
    /* Must read results before clearing persistent interrupt flag. */
    for (i=0; i < ADC_NUM_SAMPLE_PER_AVERAGE; i++) {
        sensorVal += PLIB_ADC_ResultGetByIndex(ADC_ID_1, i);
    }
 
    /* Find the average and convert the raw data */
    sensorVal = sensorVal / ADC_NUM_SAMPLE_PER_AVERAGE;
    ir_readerData.parsedData = sensorVal*(5.0/1023.0);
    
    /* If object was just detected within range of sensor */
    if (ir_readerData.parsedData < 5.0 && !ir_readerData.objDetected) {
        /* Set objDetected to true and send stop to Nav */
        ir_readerData.objDetected = true;
        sendData = message_factory(MOTOR_STOP, UINT32, dataUnion);
        nav_SendValToMessageQFromISR(&sendData);
        dbgData.size = sprintf( dbgData.buffer, "{\"DEBUG\": \"ir object detected in range\"}");
        wifly_SendValToMessageQFromISR(&dbgData);
        dbgData.size = sprintf( dbgData.buffer, "{\"DEBUG\": \"IR thread\", \"To Nav queue\": {\"cmd\": %u, \"type\": %u, \"data\": %u}}", sendData.cmd, sendData.type, sendData.data);
        wifly_SendValToMessageQFromISR(&dbgData);
    /* If object has left the detectable range */
    } else if (ir_readerData.parsedData >= 5.0 && ir_readerData.objDetected) {
        /* Set objDetected to false */
        ir_readerData.objDetected = false;
        dbgData.size = sprintf( dbgData.buffer, "{\"DEBUG\": \"ir no object in range\"}");
        wifly_SendValToMessageQFromISR(&dbgData);
    }
 
    /* If the "Stop Conversion on the First ADC Interrupt?" box in MHC is checked,
       the Hardware will disable auto-sampling when the interrupt condition
       occurs (after obtaining the 16th result). Auto-sampling needs to be
       re-enabled every ADC interrupt. */
    PLIB_ADC_SampleAutoStartEnable(ADC_ID_1);
}

/******************************************************************************
  Function:
    void IR_READER_Tasks ( void )

  Remarks:
    See prototype in ir_reader.h.
 */

void IRREADER_Tasks ( void )
{
    DEBUG_CONSTANTS error; //local error var
    message_t recvData;
    wifly_message_t dbgData;
    
    dbgData.size = sprintf( dbgData.buffer, "{\"DEBUG\":\"ir task\"}");
    wifly_SendValToMessageQ( &dbgData );
    
    SAFE(dbgOutputLoc(IR_TASK_ENTER),error);
    while(1){ 
        //blockingly read the queue
        SAFE(dbgOutputLoc(IR_TASK_WAIT_FOR_QUEUE),error);
        
        if(xQueueReceive( ir_readerData.irQueue, &recvData, portMAX_DELAY ) == pdFALSE){
            dbgError(ERROR_RECEIVEING_FROM_QUEUE);
        }
        
        SAFE(dbgOutputLoc(IR_TASK_GOT_DATA_FROM_QUEUE),error);
    }
    SAFE(dbgOutputLoc(IR_TASK_LEAVING),error);
}