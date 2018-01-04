#include "map_mgr.h"
#include "map_mgr_public.h"
#include "debug.h"
#include "message.h"
#include "navigation_public.h"

MAP_MGR_DATA map_mgrData;

//from map_mgr_public.h
void map_mgr_SendValToMessageQ(message_t val){
    if(xQueueSend( map_mgrData.map_mgrQueue_h, &val, 0U) == errQUEUE_FULL){
        dbgError(ERROR_SENDING_TO_QUEUE);
    }
}

void map_mgr_SendValToMessageQFromISR(message_t val){
    //static to minimize stack usage
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    if(xQueueSendFromISR( map_mgrData.map_mgrQueue_h, &val, &xHigherPriorityTaskWoken) == errQUEUE_FULL){
        dbgError(ERROR_SENDING_TO_QUEUE);
    }
    //check xHigherPriorityTaskWoken
    if(xHigherPriorityTaskWoken){
        portEND_SWITCHING_ISR( xHigherPriorityTaskWoken );
    }
}

void MAP_MGR_Initialize ( void )
{
    //init debug if needed
    if( !isDebugInit() )
        debugInit();
    
    dbgOutputLoc( MAP_MGR_TASK_INIT );
    
    //create the queue
    map_mgrData.map_mgrQueue_h = xQueueCreate( 5, sizeof( message_t ) );
    
    //check queue create
    if( map_mgrData.map_mgrQueue_h == NULL) //fail
        dbgError( ERROR_CREATING_QUEUE );
}


/******************************************************************************
  Function:
    void MAP_MGR_Tasks ( void )

  Remarks:
    See prototype in map_mgr.h.
 */

void MAP_MGR_Tasks ( void )
{
    //ERROR var
    DEBUG_CONSTANTS error;
    
    SAFE(dbgOutputLoc( MAP_MGR_TASK_ENTER ), error);
    SAFE(dbgOutputLoc( MAP_MGR_TASK_BEFORE_WHILE ), error);
    
    //local data
    message_t recvData;
    message_t sendData;
    MESSAGE_DATA dataUnion;
    while( 1 ){
        //wait for queue
        SAFE(dbgOutputLoc( MAP_MGR_TASK_WAIT_FOR_QUEUE ), error);
        if( xQueueReceive( map_mgrData.map_mgrQueue_h, &recvData, portMAX_DELAY) == pdFALSE )
            dbgError(ERROR_RECEIVEING_FROM_QUEUE);
        
        SAFE( dbgOutputLoc( MAP_MGR_TASK_GOT_DATA_FROM_QUEUE ), error);
        
        //process
        SAFE( dbgOutputLoc( MAP_MGR_TASK_PROCESS_DATA ), error);

        //TODO: add processing
        switch( recvData.cmd ){
            case LIDAR_MESSAGE:
                dataUnion.val_bool = true;
                sendData = message_factory(MAP_MESSAGE, BOOL, dataUnion);
                //send to nav module
                nav_SendValToMessageQ(&sendData);
                break;
            default: //do nothing and go back to waiting
                continue;
        }
        
    }
    SAFE(dbgOutputLoc( MAP_MGR_TASK_LEAVING ), error);
}
