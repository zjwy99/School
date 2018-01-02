

#include "test.h"
#include "wifly.h"
#include "wifly_public.h"
#include "json_parse.h"
#include "debug.h"
#include "message.h"
#include "navigation_public.h"
#include "test_public.h"


TEST_DATA testData;

void testTimerCallback( TimerHandle_t xTimer ){
    
    if (is_enable_testing()) 
    {
        switch (get_test_number()) 
        {
            case 2:
            valid();
            increment_seq_num();  
            break;    
                
            case 4:
            badChecksum();
            increment_seq_num();
            break;
            
            case 6:
            badSequence();
            increment_seq_num();
            break;
            
            case 7:
            testQueues();
            break;
            default:
                break;
            
        }
    }
}

void TEST_Initialize(void)
{
     //init debug if needed
    if( !isDebugInit() )
        debugInit();
    
    dbgOutputLoc( WIFLY_TASK_INIT );
    
    //create the queue for transmitting
    testData.testQueue_h = xQueueCreate( 10, sizeof(char) );
     //check queue create
    if( testData.testQueue_h == NULL){ //fail
        dbgError( ERROR_CREATING_QUEUE );
    }
     //create rate timer
    testData.testTimer_h = xTimerCreate(
            "Rate Timer",
            ONE_SEC*6,
            pdTRUE,
            (void *) 0,
            testTimerCallback
        );
    if( testData.testTimer_h == NULL ){
        dbgError( ERROR_STARING_TIMER );
    }else{ //start
        if( xTimerStart( testData.testTimer_h, 0) != pdPASS ){
            dbgError( ERROR_STARING_TIMER );
        }
    }
    
    set_enable_testing(false);
    set_test_number(2);
    
}

void changeTestPeriod(const size_t newPeriodms){
    static wifly_message_t dbgData;
    if( xTimerChangePeriod( 
        testData.testTimer_h, 
        newPeriodms/portTICK_PERIOD_MS,
        1000) == pdPASS ){
        dbgData.size = sprintf(
            dbgData.buffer,
            "{\"DEBUG\":\"Set new test period to %ums\"}",
            newPeriodms
        );
        wifly_SendValToMessageQ( &dbgData );
    } else {
        dbgData.size = sprintf(
            dbgData.buffer,
            "{\"DEBUG\":\"Failed to set new test period to %ums\"}",
            newPeriodms
        );
        wifly_SendValToMessageQ( &dbgData );
    }
    
}

void badChecksum()
{
    wifly_message_t toSend;
    MESSAGE_DATA m;
    m.val_bool = true;
    message_t temp = message_factory(0x01, 7, m);
            
    static char temp2[BUFFER_SIZE];
    static size_t temp_size;
    temp_size = sprintf(temp2, "{\"cmd\": %u, \"type\": %u, \"data\": %f}", temp.cmd, temp.type, temp.data.val_float);
    //make final message
    toSend.size = sprintf(toSend.buffer, "{\"checksum\": %u, \"seq_num\": %u, \"payload\": %.*s}", 212, get_seq_num(), temp_size, temp2);
    
    //toSend.size = sprintf(toSend.buffer, "{\"checksum\":77665588,\"seq_num\":0,\"payload\":{\"cmd\":12,\"type\":6,\"value\":12.1}}");
    wifly_SendValToMessageQ(&toSend);

    
}

void badSequence()
{
    increment_seq_num();
    increment_seq_num();
    wifly_message_t toSend;
    MESSAGE_DATA m;
    m.val_bool = true;
    message_t temp = message_factory(0x01, 7, m);
    
    json_from_message_t(&temp, &toSend);
    wifly_SendValToMessageQ(&toSend);
    
}

void valid()
{
    wifly_message_t toSend;
    MESSAGE_DATA m;
    m.val_bool = true;
    message_t temp = message_factory(0x01, 7, m);
    
    json_from_message_t(&temp, &toSend);
    wifly_SendValToMessageQ(&toSend);
}

void testQueues(){
    MESSAGE_DATA m;
    m.val_bool = true;
    message_t temp = message_factory(TEST, 7, m);
    nav_SendValToMessageQ(&temp);
}

void TEST_Tasks ( void )
{
    
}

