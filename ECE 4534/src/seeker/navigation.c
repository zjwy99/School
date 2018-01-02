#include "navigation.h"
#include "navigation_public.h"
#include "message.h"
#include "debug.h"
#include "motor_public.h"
#include "wifly_public.h"
#include "pathfinding.h"
#include "json_parse.h"
#include <stdio.h>

NAVIGATION_DATA navigationData;

//from navigation_public.h
void nav_SendValToMessageQ(message_t *val){
    if(xQueueSend( navigationData.navQueue_h, val, 0U) == errQUEUE_FULL){
        dbgError(ERROR_SENDING_TO_QUEUE);
    }
}

void nav_SendValToMessageQFromISR(message_t *val){
    //static to minimize stack usage
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    if(xQueueSendFromISR( navigationData.navQueue_h, val, &xHigherPriorityTaskWoken) == errQUEUE_FULL){
        dbgError(ERROR_SENDING_TO_QUEUE);
    }
    //check xHigherPriorityTaskWoken
    if(xHigherPriorityTaskWoken){
        portEND_SWITCHING_ISR( xHigherPriorityTaskWoken );
    }
}

void sendMotorCommand(MESSAGE_COMMAND type){
    static message_t data = {MOTOR_BACKWARD, 75, {18}};
    static wifly_message_t sendMsg;
    sendMsg.size = sprintf(
        sendMsg.buffer, 
        "{\"DEBUG\":\" sendMotorCommand(%u) called\"}",
        type
    );
    wifly_SendValToMessageQ(&sendMsg);
    data.cmd = type;
    if( type == MOTOR_LEFT || type == MOTOR_RIGHT ){
        data.data.val_ui32 = 90;
    } else {
        data.data.val_ui32 = 18;
    }
    motor_SendValToMessageQ(&data);
}

bool executeNextMove(){
     if(navigationData.curDir == navigationData.nextDir){
         sendMotorCommand(MOTOR_FORWARD);
         return true;
     } 
     //else 
     //execute turn
     switch(navigationData.nextDir - navigationData.curDir){
         case 1: //RIGHT
         case -3:
             sendMotorCommand(MOTOR_LEFT);
             navigationData.turned = true;
             break;
         case 0: //forward (ignore)
             break;
         case 2:
         case -2: //BACKWARDS (do not set turned and return true)
             sendMotorCommand(MOTOR_BACKWARD);
             return true;
         case -1:
         case 3: //LEFT
             sendMotorCommand(MOTOR_RIGHT);
             navigationData.turned = true;
             break;
     }
     
    return false;
}

/**
 * Send commands to the motor to move the rover to the next position in the path
 * also determines the next position
 */
void determineNextMoves(){
    //get message count and put into json
    static wifly_message_t sendMsg;
   
    //first check for reset
    if(navigationData.curStep > navigationData.steps || navigationData.steps == 1){
        return;
    }
    //else move current position
    //get next points
    navigationData.nextx = navigationData.path[navigationData.curStep * 2];
    navigationData.nexty = navigationData.path[navigationData.curStep * 2 + 1];
    //get new direction
    if(navigationData.cury > navigationData.nexty){
        //next dir is up
        navigationData.nextDir = UP;
    }else if(navigationData.cury < navigationData.nexty){
        //next is down
        navigationData.nextDir = DOWN;
    }else if(navigationData.curx > navigationData.nextx){
        //next is to the right
        navigationData.nextDir = RIGHT;
    }else if(navigationData.curx < navigationData.nextx){
        //next is to the left
        navigationData.nextDir = LEFT;
    }
}

/**
 * called when the motor has completed its movement
 * Moves the rover's internal state to the next point
 */
bool pathMoveToNextPos(){
    //get message count and put into json
    static wifly_message_t sendMsg;
    //check if there is a turn queued
    if(navigationData.turned){
        navigationData.curDir = navigationData.nextDir;
        navigationData.turned = false;
    }else{
        //else move current position
        //get next points
        navigationData.curx = navigationData.nextx;
        navigationData.cury = navigationData.nexty;
        //else do nothing with the directions
        //debug
        sendMsg.size = sprintf(
                sendMsg.buffer, 
                "{\"DEBUG\":\"Step %i, moved to (%i,%i) @ %i\"}",
                navigationData.curStep,
                navigationData.curx,
                navigationData.cury,
                navigationData.curDir
            );
        wifly_SendValToMessageQ(&sendMsg);
        //increment step
        navigationData.curStep++;
    }
    //last check for reset
    if(navigationData.curStep >= navigationData.steps ){
        //check if more moves to wait for
        //reset nav
        navigationData.state = WAIT_FOR_START;
        //reset path
        navigationData.steps = findPath(
                default_map,
                navigationData.curx, navigationData.cury,
                navigationData.destx, navigationData.desty,
                navigationData.path
            );
        sendMsg.size = sprintf(
            sendMsg.buffer, 
            "{\"DEBUG\":\"Step %i, DONE\"}",
            navigationData.curStep
        );
        wifly_SendValToMessageQ(&sendMsg);
        navigationData.state = WAIT_FOR_START;
        return true;
    } else {
        return false;
    }
}

void mapSendCallback( TimerHandle_t xTimer ){
    static uint32_t pointsSent = 0;
    //get point and put into json
    static wifly_message_t sendMsg;
    static message_t point;
    static MESSAGE_DATA Y;
    static char tempPoints[BUFFER_SIZE];
    //check for reset
    if(pointsSent >= MAP_X*MAP_Y){
        //stop the timer
        xTimerStop(navigationData.mapSendTimer_h, 0);
        //send stop to server
        //make point
        Y.val_bool = navigationData.peek;
        point = message_factory(SERVER_DONE_SENDING_MAP, 0, Y );
        json_from_message_t(&point, &sendMsg);
        //send the message
        wifly_SendValToMessageQ(&sendMsg);
        increment_seq_num();
        //reset count
        pointsSent = 0;
        return;
    }
    //send 100 points together
    int count = 0;
    //build string of points
    for(; count < 100; count++,pointsSent++){
        tempPoints[count] = '0'+*((uint8_t*)default_map + pointsSent );
    }
    //make message
    json_from_string(PIC_GET_MAP, pointsSent, tempPoints, &sendMsg);
    //send the message
    wifly_SendValToMessageQ(&sendMsg);
    increment_seq_num();
}

void NAVIGATION_Initialize ( void )
{
    //init debug if needed
    if( !isDebugInit() )
        debugInit();
    
    dbgOutputLoc( NAVIGATION_TASK_INIT );
 
    //create the queue
    navigationData.navQueue_h = xQueueCreate( 5, sizeof( message_t ) );
    navigationData.state = WAIT_FOR_START;
    
    //check queue create
    if( navigationData.navQueue_h == NULL) //fail
        dbgError( ERROR_CREATING_QUEUE );
    
    
    //map sending timer
    navigationData.mapSendTimer_h = xTimerCreate(
            "Map Send Timer",
            50 / portTICK_PERIOD_MS,
            pdTRUE,
            (void *) 0,
            mapSendCallback
        );
    if( navigationData.mapSendTimer_h == NULL ){
        dbgError( ERROR_STARING_TIMER );
    }
    
    navigationData.curx = navigationData.cury = 0;
    navigationData.destx = navigationData.desty = 0;
    navigationData.curDir = UP;
    navigationData.turned = false;
}

void sendPath(){
    static wifly_message_t sendToWiflyData;
    int i = 0;
    for(; i < navigationData.steps; i++){
        //get the points
        uint8_t 
            x = navigationData.path[i*2],
            y = navigationData.path[i*2+1]
        ;
        sendToWiflyData.size = sprintf(
                sendToWiflyData.buffer,
                "{\"DEBUG\":\"%i: (%i,%i)\"}",
                i,x,y
            );
        wifly_SendValToMessageQ(&sendToWiflyData);          
    }
}

void updateMap( message_t* point){
    default_map[ point->type/MAP_Y ][ point->type%MAP_X ] = point->data.val_bool;
}

void updateMapBlock( message_t* block ){
    uint16_t remaining, curEntry, index;
    uint16_t data = 0;
    for( curEntry = 0, remaining = 1; curEntry < 25; curEntry++, remaining++ ){
        //type is the last index used + 1
        index = block->type - remaining;
        if(index >= MAP_X*MAP_Y)
            break;
        data = (block->data.val_ui32 >> curEntry) & 1;
        default_map[ index/MAP_Y ][ index%MAP_X ] = data;
    }
}

void setDest( message_t* point){
    static wifly_message_t sendMsg;
    //index = [x*map_x + y]
    navigationData.destx = point->type;
    navigationData.desty = point->data.val_ui8;
    //send each path to server
    sendMsg.size = sprintf(
            sendMsg.buffer,
            "{\"DEBUG\":\"New Dest: (%i,%i)\"}", 
            navigationData.destx,
            navigationData.desty
        );
    wifly_SendValToMessageQ(&sendMsg);
}

void setCurPos( message_t* point){
    static wifly_message_t sendMsg;
    navigationData.curx = point->type;
    navigationData.cury = point->data.val_ui8;
    //send each path to server
    sendMsg.size = sprintf(
            sendMsg.buffer,
            "{\"DEBUG\":\"New Cur Pos: (%i,%i)\"}", 
            navigationData.curx,
            navigationData.cury
        );
    wifly_SendValToMessageQ(&sendMsg);
}

void sendCurPos(){
    static wifly_message_t sendMsg;
    static message_t point;
    static MESSAGE_DATA Y;
    //make point
    Y.val_ui32 = navigationData.cury;
    point = message_factory(SERVER_MAP_POINT, navigationData.curx, Y );
    json_from_message_t(&point, &sendMsg);
    //send the message
    wifly_SendValToMessageQ(&sendMsg);
    increment_seq_num();
}

void sendPoint( message_t* point){
    static wifly_message_t sendMsg;
    static message_t data;
    static MESSAGE_DATA Y;
    static uint32_t index;
    index = (point->data.val_ui32 * MAP_Y) + point->type;
    //make point
    Y.val_ui32 = default_map[ point->data.val_ui32 ][ point->type ];
    data = message_factory(SERVER_MAP_POINT, index, Y );
    json_from_message_t(&data, &sendMsg);
    //send the message
    wifly_SendValToMessageQ(&sendMsg);
    increment_seq_num();
}

void stopNav(){
    static wifly_message_t sendMsg;
    //recalc path
    navigationData.steps = findPath(
            default_map,
            navigationData.curx, navigationData.cury,
            navigationData.destx, navigationData.desty,
            navigationData.path
        );
    navigationData.curStep = 0;
    //send steps
    sendMsg.size = sprintf(
            sendMsg.buffer,
            "{\"DEBUG\":\"Steps: %i\"}", 
            navigationData.steps
        );
    wifly_SendValToMessageQ(&sendMsg);
    //reset nav
    navigationData.state = WAIT_FOR_START;
}

void stopMotor(message_t * msg){
    static wifly_message_t sendMsg;
    motor_SendValToMessageQ(msg);
    stopNav();
    //send done
    sendMsg.size = sprintf(
        sendMsg.buffer, 
        "{\"DEBUG\":\"MOTOR STOP\"}"
    );
    wifly_SendValToMessageQ(&sendMsg);
}

void NAVIGATION_Tasks ( void )
{
    //ERROR var
    DEBUG_CONSTANTS error;
    
    SAFE(dbgOutputLoc( NAVIGATION_TASK_ENTER ), error);
    
    //local data
    message_t recvData;
    message_t sendData;
    wifly_message_t sendToWiflyData;
    MESSAGE_DATA dataUnion;
        
    SAFE(dbgOutputLoc( NAVIGATION_TASK_BEFORE_WHILE ), error);
    
    while( 1 ){
        //wait for queue
        SAFE(dbgOutputLoc( NAVIGATION_TASK_WAIT_FOR_QUEUE ), error);
        
        if( xQueueReceive( navigationData.navQueue_h, &recvData, portMAX_DELAY) == pdFALSE )
            dbgError(ERROR_RECEIVEING_FROM_QUEUE);
        
        SAFE( dbgOutputLoc( NAVIGATION_TASK_GOT_DATA_FROM_QUEUE ), error);
        SAFE( dbgUARTVal(recvData.cmd), error);
        //process
        SAFE( dbgOutputLoc( NAVIGATION_TASK_PROCESS_DATA ), error);
        
        //always do (async commands like start stop)
        switch( recvData.cmd ){
            case PIC_NAV_SET_DEST:
                setDest(&recvData);
                stopNav();
                continue;
            case PIC_NAV_SET_START:
                setCurPos(&recvData);
                stopNav();
                continue;
            case PIC_GET_PATH:
                sendPath();
                break;
            case PIC_GET_MAP:
                if( xTimerStart( navigationData.mapSendTimer_h, 0) != pdPASS ){
                    dbgError( ERROR_STARING_TIMER );
                }
                navigationData.peek = false;
                break;
            case PIC_PEEK_MAP:
                if( xTimerStart( navigationData.mapSendTimer_h, 0) != pdPASS ){
                    dbgError( ERROR_STARING_TIMER );
                }
                navigationData.peek = true;
                break;
            case PIC_NAV_STOP:
                stopNav();
                continue;
            case MOTOR_STOP:
                stopMotor(&recvData);
                break;
            case PIC_MAP_POINT:
                //interpret data and change location in the map
                updateMap(&recvData);
                stopNav();
                break;
            case PIC_MAP_POINT_BLOCK:
                updateMapBlock(&recvData);
                stopNav();
                break;
            case PIC_GET_CUR_POS:
                sendCurPos();
                break;
            case PIC_GET_POINT:
                sendPoint(&recvData);
                break;
            case SERVER_GET_MAP:
                //forward to server
                json_from_message_t( &recvData, &sendToWiflyData );
                wifly_SendValToMessageQ(&sendToWiflyData);
                increment_seq_num();
                break;
            default:
                break;
        }
        //state machine
        switch( navigationData.state ){
            case WAIT_FOR_START:
                if( recvData.cmd == PIC_NAV_START){
                    navigationData.state = EXEC_PATH;
                    //skip the first step (it is the current position)
                    navigationData.curStep = 1;
                    //first step
                    determineNextMoves();
                    //execute first move
                    executeNextMove();
                    //sendMotorCommand(MOTOR_RIGHT);
                    break;
                }
                //else do nothing
                break;
            case EXEC_PATH:
                if( recvData.cmd == MOTOR_MESSAGE ){
                    if(!pathMoveToNextPos()){
                        determineNextMoves();
                        executeNextMove();
                    }
//                    navigationData.state = WAIT_FOR_START;
                }
                break;
        }
    }
    SAFE(dbgOutputLoc( NAVIGATION_TASK_LEAVING ), error);
}
