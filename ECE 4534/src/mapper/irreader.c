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

uint32_t getDistance()
{
    return (uint32_t)ir_readerData.parsedData;
}

void ADC_send(TimerHandle_t xTimer )
{
    wifly_message_t dbgData;
    message_t sendData;
    MESSAGE_DATA dataUnion;
    
    
    dataUnion.val_float = ir_readerData.parsedData;
    sendData = message_factory(IR_MESSAGE, FLOAT, dataUnion);
    json_from_message_t(&sendData, &dbgData);
    wifly_SendValToMessageQFromISR(&dbgData);
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
    ir_readerData.dataReady = false;
    ir_readerData.objDetected = false;
    ir_readerData.parsedData = 0;
    
    //create the timer callback
    ir_readerData.stepperTimer_h = xTimerCreate(
            "IR Timer",
            1000 / portTICK_PERIOD_MS,
            pdTRUE,
            (void *) 0,
            ADC_send
        );
    //check
     if( ir_readerData.stepperTimer_h == NULL ){
        dbgError( ERROR_STARING_TIMER );
    }else{ //start
        //if( xTimerStart(ir_readerData.stepperTimer_h, 0) != pdPASS ){
        //    dbgError( ERROR_STARING_TIMER );
        //}
    }

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
   
 
    /* Must read results before clearing persistent interrupt flag. */
    for (i=0; i < ADC_NUM_SAMPLE_PER_AVERAGE; i++) {
        sensorVal += PLIB_ADC_ResultGetByIndex(ADC_ID_1, i);
    }
 
    sensorVal = sensorVal / ADC_NUM_SAMPLE_PER_AVERAGE;
    ir_readerData.parsedData = 2076/(float)(sensorVal*(3*200/(float)1023) - 11);
    ir_readerData.dataReady = true;
    ir_readerData.objDetected = true;
    

    
   
 
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