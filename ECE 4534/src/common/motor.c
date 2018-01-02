#include "motor.h"
#include "motor_public.h"
#include "wifly_public.h"
#include "debug.h"
#include "navigation_public.h"
#include <stdio.h>

MOTOR_DATA motorData;

//from motor_public.h
void motor_SendValToMessageQ(message_t* val){
    if(xQueueSend( motorData.motorQueue_h, val, 0U) != pdPASS){
        dbgError(ERROR_SENDING_TO_QUEUE);
    }
}

void motor_SendValToMessageQFromISR(message_t* val){
    //static to minimize stack usage
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    if(xQueueSendFromISR( motorData.motorQueue_h, val, &xHigherPriorityTaskWoken) == errQUEUE_FULL){
        dbgError(ERROR_SENDING_TO_QUEUE);
    }
    //check xHigherPriorityTaskWoken
    if(xHigherPriorityTaskWoken){
        portEND_SWITCHING_ISR( xHigherPriorityTaskWoken );
    }
}

/* From motor.h */
int get_cycle(int speed)
{
    if(speed <= 0){
        return 0;
    } else if (speed >= 100) {
        return MOTOR_MAX_PULSE_WIDTH;
    } else {
        return (speed/100.)*MOTOR_MAX_PULSE_WIDTH;
    }
}

void reset_counter_reg(void)
{
    PLIB_TMR_Counter16BitClear(TMR_ID_2);
    PLIB_TMR_Counter16BitClear(TMR_ID_3);
    motorData.current_tick = 0;
}

void save_current_state(void)
{
    motorData.saved_motor1_direction = motorData.motor1_direction;
    motorData.saved_motor2_direction = motorData.motor2_direction;
    motorData.saved_speed = motorData.speed;
    motorData.saved_current_tick = motorData.current_tick;
    motorData.saved_target_tick = motorData.target_tick;
    motorData.saved_state = motorData.state;
}

void load_saved_state(void)
{
    motorData.motor1_direction = motorData.saved_motor1_direction;
    motorData.motor2_direction = motorData.saved_motor2_direction;
    motorData.speed = motorData.saved_speed;
    motorData.current_tick = motorData.saved_current_tick;
    motorData.target_tick = motorData.saved_target_tick;
    motorData.state = motorData.saved_state;
    
    /* Set motors speed and direction */
    motor1_set(motorData.speed, motorData.motor1_direction);
    motor2_set(motorData.speed, motorData.motor2_direction);
}

void motor1_set_pulse(int speed)
{
    int cycle;
    cycle = get_cycle(speed);
    PLIB_OC_PulseWidth16BitSet(_OCMP1_BASE_ADDRESS, cycle);
}

void motor2_set_pulse(int speed)
{
    int cycle;
    cycle = get_cycle(speed);
    PLIB_OC_PulseWidth16BitSet(_OCMP2_BASE_ADDRESS, cycle);
}

void motor1_set(int speed, MOTOR_DIRECTION direction)
{
    motor1_set_pulse(speed);
    SYS_PORTS_PinWrite(PORTS_ID_0, MOTOR1_DIRECTION_CHANNEL, MOTOR1_DIRECTION_BIT, direction);
    motorData.motor1_direction = direction;
    motorData.speed = speed;
}

void motor2_set(int speed, MOTOR_DIRECTION direction)
{
    motor2_set_pulse(speed);
    SYS_PORTS_PinWrite(PORTS_ID_0, MOTOR2_DIRECTION_CHANNEL, MOTOR2_DIRECTION_BIT, direction);
    motorData.motor2_direction = direction;
    motorData.speed = speed;
}

void rover_stop(void)
{
    motor1_set(0, motorData.motor1_direction);
    motor2_set(0, motorData.motor2_direction);
}

/* 
 * Steven's
 * 
 * ----- MOTOR_BACKWARD -----
 * motor1_set(0.97*speed, direction);
 * motor2_set(speed, direction);
 * 
 * ----- MOTOR_ONWARD -----
 * motor1_set(speed, direction);
 * motor2_set(0.98*speed, direction);
 * 
 */

/*
 * Nicky's
 * 
 * ----- MOTOR_BACKWARD -----
 * motor1_set(0.98*speed, direction);
 * motor2_set(speed, direction);
 * 
 * ----- MOTOR_ONWARD -----
 * motor1_set(0.97*speed, direction);
 * motor2_set(speed, direction);
 */

/*
 * Zach's
 * 
 * ----- MOTOR_BACKWARD -----
 * motor1_set(speed, direction);
 * motor2_set(0.85*speed, direction);
 * 
 * ----- MOTOR_ONWARD -----
 * motor1_set(speed, direction);
 * motor2_set(0.96*speed, direction);
 */
void rover_move_straight(int speed, MOTOR_DIRECTION direction, uint32_t distance)
{
    /* Set distance to travel */
    motorData.target_tick = distance*MOTOR_TICKS_PER_CM;
    
    /* Reset counter registers */
    reset_counter_reg();
    
    if(direction == MOTOR_BACKWARD) {
        /* Set motors speed and direction */
        motor1_set(0.97*speed, direction);
        motor2_set(speed, direction);
    } 
    else if(direction == MOTOR_ONWARD) {
        /* Set motors speed and direction */
        motor1_set(speed, direction);
        motor2_set(0.98*speed, direction);
    }
}

void rover_turn_cw(int speed, float degree)
{   
    /* Set distance to travel, arc length formula */
    motorData.target_tick = MOTOR_PI*ROVER_DIAMETER*MOTOR_TICKS_PER_CM*(degree/360.);
    
    /* Reset counter registers */
    reset_counter_reg();
    
    /* Set motors speed and direction */
    motor1_set(speed, MOTOR_REVERSE);
    motor2_set(speed, MOTOR_ONWARD);
}

void rover_turn_ccw(int speed, float degree)
{
    /* Set distance to travel, arc length formula */
    motorData.target_tick = MOTOR_PI*ROVER_DIAMETER*MOTOR_TICKS_PER_CM*(degree/360.);
    
    /* Reset counter registers */
    reset_counter_reg();
    
    /* Set motors speed and direction */
    motor1_set(speed, MOTOR_ONWARD);
    motor2_set(speed, MOTOR_REVERSE);
}

void update_tick(void)
{
    motorData.current_tick = motorData.current_tick + 30;
    motorData.total_tick = motorData.total_tick + 30;
}


void MOTOR_Initialize ( void )
{
    dbgOutputLoc( MOTOR_TASK_INIT );
    
    /* Initialize data */
    motorData.total_tick = 0;
    motorData.current_tick = 0;
    motorData.target_tick = 0;
    motorData.motor1_direction = MOTOR_ONWARD;
    motorData.motor2_direction = MOTOR_ONWARD;
    motorData.state = MOTOR_SERVICE_TASKS;
    
    /* Create the queue and check if created */
    motorData.motorQueue_h = xQueueCreate( 5, sizeof( message_t ) );
    if( motorData.motorQueue_h == NULL) dbgError( ERROR_CREATING_QUEUE );
    
    /* Start output compare modules */
    DRV_OC0_Start();
    DRV_OC1_Start();
    
    /* Start timers */
    DRV_TMR0_Start();
    DRV_TMR1_Start();
}


/******************************************************************************
  Function:
    void MOTOR_Tasks ( void )

  Remarks:
    See prototype in motor.h.
 */

void MOTOR_Tasks ( void )
{
    //ERROR var
    DEBUG_CONSTANTS error;
    
    SAFE(dbgOutputLoc( MOTOR_TASK_ENTER ), error);
    SAFE(dbgOutputLoc( MOTOR_TASK_BEFORE_WHILE ), error);
    
    //local data
    message_t recvData;
    message_t sendData;
    MESSAGE_DATA dataUnion;
    wifly_message_t dbgData;
    bool queueEmpty = true;
    
    dbgData.size = sprintf( dbgData.buffer, "{\"DEBUG\":\"motor task\"}");
    wifly_SendValToMessageQ( &dbgData );
    
    while( 1 ){
  
        SAFE(dbgOutputLoc( MOTOR_TASK_WAIT_FOR_QUEUE ), error);

        if( xQueueReceive( motorData.motorQueue_h, &recvData, 0) != pdPASS ) {
            //dbgError(ERROR_RECEIVEING_FROM_QUEUE);
            queueEmpty = true;
        } else {
            queueEmpty = false;
            
            SAFE( dbgOutputLoc( MOTOR_TASK_GOT_DATA_FROM_QUEUE ), error);

            SAFE( dbgOutputLoc( MOTOR_TASK_PROCESS_DATA ), error);
            
            dbgData.size = sprintf( dbgData.buffer, "{\"DEBUG\": \"Motor thread\", \"From Motor queue\": {\"cmd\": %u, \"type\": %u, \"data\": %u}}", recvData.cmd, recvData.type, recvData.data);
            wifly_SendValToMessageQ( &dbgData );
        }
        
        switch(motorData.state)
        {
            case MOTOR_SERVICE_TASKS:
                /* Match received command to correct movement function */
                if (queueEmpty) continue;
                else if (recvData.cmd == MOTOR_FORWARD) {
                    rover_move_straight(recvData.type, MOTOR_ONWARD, recvData.data.val_ui32);
                    motorData.state = MOTOR_MOVEMENT;
                }
                else if (recvData.cmd == MOTOR_BACKWARD) {
                    rover_move_straight(recvData.type, MOTOR_BACKWARD, recvData.data.val_ui32);
                    motorData.state = MOTOR_MOVEMENT;
                }
                else if (recvData.cmd == MOTOR_LEFT) {
                    rover_turn_ccw(recvData.type, recvData.data.val_ui32);
                    motorData.state = MOTOR_MOVEMENT;
                }
                else if (recvData.cmd == MOTOR_RIGHT) {
                    rover_turn_cw(recvData.type, recvData.data.val_ui32);
                    motorData.state = MOTOR_MOVEMENT;
                }
                else if (recvData.cmd == MOTOR_STOP) {
                    rover_stop();
                    reset_counter_reg();
                }
                else if (recvData.cmd == MANUAL_FORWARD) {
                    rover_move_straight(recvData.type, MOTOR_ONWARD, recvData.data.val_ui32);
                    motorData.state = MOTOR_MOVEMENT;
                }
                else if (recvData.cmd == MANUAL_BACKWARD) {
                    rover_move_straight(recvData.type, MOTOR_BACKWARD, recvData.data.val_ui32);
                    motorData.state = MOTOR_MOVEMENT;
                }
                else if (recvData.cmd == MANUAL_LEFT) {
                    rover_turn_ccw(recvData.type, recvData.data.val_ui32);
                    motorData.state = MOTOR_MOVEMENT;
                }
                else if (recvData.cmd == MANUAL_RIGHT) {
                    rover_turn_cw(recvData.type, recvData.data.val_ui32);
                    motorData.state = MOTOR_MOVEMENT;
                }
                else continue;
                break;
                
            case MOTOR_MOVEMENT:
                /* Wait until current movement finishes or stop command comes through */
                if (!queueEmpty && recvData.cmd == MOTOR_STOP) {
                    rover_stop();
                    dataUnion.val_float = motorData.current_tick / MOTOR_TICKS_PER_CM;
                    reset_counter_reg();
                    sendData = message_factory(MOTOR_MESSAGE, FLOAT, dataUnion);
                    motorData.state = MOTOR_SERVICE_TASKS;
                    dbgData.size = sprintf( dbgData.buffer, "{\"DEBUG\": \"motor abrupt stop\"}");
                    nav_SendValToMessageQFromISR( &sendData );
                    wifly_SendValToMessageQ( &dbgData );
                } 
                else if (!queueEmpty && recvData.cmd == MANUAL_FORWARD) {
                    save_current_state();
                    rover_stop();
                    reset_counter_reg();
                    rover_move_straight(recvData.type, MOTOR_ONWARD, recvData.data.val_ui32);
                    motorData.state = MOTOR_MANUAL;
                }
                else if (!queueEmpty && recvData.cmd == MANUAL_BACKWARD) {
                    save_current_state();
                    rover_stop();
                    reset_counter_reg();
                    rover_move_straight(recvData.type, MOTOR_BACKWARD, recvData.data.val_ui32);
                    motorData.state = MOTOR_MANUAL;
                }
                else if (!queueEmpty && recvData.cmd == MANUAL_LEFT) {
                    save_current_state();
                    rover_stop();
                    reset_counter_reg();
                    rover_turn_ccw(recvData.type, recvData.data.val_ui32);
                    motorData.state = MOTOR_MANUAL;
                }
                else if (!queueEmpty && recvData.cmd == MANUAL_RIGHT) {
                    save_current_state();
                    rover_stop();
                    reset_counter_reg();
                    rover_turn_cw(recvData.type, recvData.data.val_ui32);
                    motorData.state = MOTOR_MANUAL;
                }
                else if (motorData.current_tick >= motorData.target_tick) {
                    rover_stop();
                    dataUnion.val_float = motorData.current_tick / MOTOR_TICKS_PER_CM;
                    reset_counter_reg();
                    sendData = message_factory(MOTOR_MESSAGE, FLOAT, dataUnion);
                    motorData.state = MOTOR_SERVICE_TASKS;
                    dbgData.size = sprintf( dbgData.buffer, "{\"DEBUG\": \"motor finished task\"}");
                    nav_SendValToMessageQFromISR( &sendData );
                    wifly_SendValToMessageQ( &dbgData );
                }
                break;
                
            case MOTOR_MANUAL:
                /* Wait until current manual movement finishes */
                if (motorData.current_tick >= motorData.target_tick) {
                    rover_stop();
                    load_saved_state();
                    dbgData.size = sprintf( dbgData.buffer, "{\"DEBUG\": \"motor finished manual control\"}");
                    wifly_SendValToMessageQ( &dbgData );
                }
                break;
        }
    }
    
    SAFE(dbgOutputLoc( MOTOR_TASK_LEAVING ), error);
}
