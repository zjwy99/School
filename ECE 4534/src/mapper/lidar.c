#include "lidar.h"
#include "lidar_public.h"
#include "debug.h"
#include "message.h"
#include "navigation_public.h"
#include <stdio.h>
#include <string.h>
LIDAR_DATA lidarData;

#define DEGREE_PER_TICK 0.70172155688622754491017964071856

uint16_t init[177] = {136, 384, 511, 153, 145, 152, 256, 255, 128, 152, 4704, 151,
                    65281, 384, 511, 0, 1791, 153, 131, 152, 1411, 2047, 385, 384,
                    27540, 131, 153, 131, 152, 387, 153, 146, 152, 129, 1791, 153,
                    131, 152, 1411, 511, 256, 255, 128, 153, 176, 149, 511, 79, 
                    11342, 255, 46262, 147, 511, 0, 255, 9, 16, 17, 292, 65317, 117,
                    511, 11342, 72, 8240, 255, 2352, 84, 1073, 818, 33600, 9542, 96,
                    39, 1616, 81, 38482, 2134, 12375, 97, 98, 100, 101, 41062, 511,
                    12834, 5191, 65353, 74, 255, 2682, 123, 8568, 511, 13347, 66,
                    65348, 9797, 1350, 16448, 1550, 6688, 16451, 255, 820, 17461, 511,
                    1073, 2379, 1356, 1101, 255, 68, 8261, 2119, 10312, 103, 1136,
                    369, 65138, 118, 119, 511, 269, 255, 384, 63489, 511, 398, 256,
                    255, 128, 1034, 267, 153, 132, 152, 388, 153, 1, 152, 153, 80, 152, 
                    153, 70, 152, 153, 81, 144, 153, 112, 152, 153, 113, 144, 59393, 153,
                    1, 152, 153, 80, 152, 153, 70, 152, 153, 81, 144, 153, 112, 152, 153,
                    113, 144, 151, 257};

uint16_t calibration_val[14] = {16640, 153, 19, 152, 267, 0, 513, 16640, 153, 19, 152, 267, 0, 59393};

uint16_t dist_reg[17] = {384, 511, 15505, 256, 255, 128, 256, 153, 0, 152, 153, 19, 152, 153, 30, 144, 267};

uint32_t sp1[2] = {2097220, 9699953};
uint64_t sp2 = 32688;


static const uint8_t half_steps[8] = {
    0x08,
    0x0C,
    0x04,
    0x06,
    0x02,
    0x03,
    0x01,
    0x09
};

static const uint8_t full_steps[4] = {
    0x09,
    0x0C,
    0x06,
    0x03
};

//from lidar_public.h
void lidar_SendValToMessageQ(message_t *val){
    if(xQueueSend( lidarData.lidarQueue_h, val, 0U) == errQUEUE_FULL){
        dbgError(ERROR_SENDING_TO_QUEUE);
    }
}

void lidar_SendValToMessageQFromISR(message_t *val){
    //static to minimize stack usage
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    if(xQueueSendFromISR( lidarData.lidarQueue_h, val, &xHigherPriorityTaskWoken) == errQUEUE_FULL){
        dbgError(ERROR_SENDING_TO_QUEUE);
    }
    //check xHigherPriorityTaskWoken
    if(xHigherPriorityTaskWoken){
        portEND_SWITCHING_ISR( xHigherPriorityTaskWoken );
    }
}

uint16_t getDistance()
{
    return lidarData.distance;
}

/**
 * Write the current step value to the stepper motor input pins
 * A0 -> A -> bit 3
 * A1 -> B -> bit 2
 * A4 -> C -> bit 1
 * A5 -> D -> bit 0
 * @param xTimer -- ignore
 */
void stepperCallback( TimerHandle_t xTimer ){
    static uint32_t step = 0;
    static uint32_t tick = 0;
    static wifly_message_t dbgData;
    static message_t sendData = {SERVER_LIDAR_READING, 0, {0}};
    PORTS_DATA_TYPE write_val = SYS_PORTS_Read(PORTS_ID_0, PORT_CHANNEL_A);
    uint8_t A, B, C, D;
    A = !!(full_steps[step] & 8);
    B = !!(full_steps[step] & 4);
    C = !!(full_steps[step] & 2);
    D = !!(full_steps[step] & 1);
    
    //assign values to the proper bits which keeping unrelated values
    write_val ^= ( -A ^ write_val ) & ( 1 /*<<0*/);
    write_val ^= ( -B ^ write_val ) & ( 1 << 1 );
    write_val ^= ( -C ^ write_val ) & ( 1 << 4 );
    write_val ^= ( -D ^ write_val ) & ( 1 << 5 );
    
    SYS_PORTS_Write(PORTS_ID_0, PORT_CHANNEL_A, write_val);

    if( lidarData.scanDir ){
        if( tick == 513 ){
            lidarData.scanDir = 0;
        }else{
            tick++;
        }
        if( step == 3 ){
            step = 0;
        }
        else{
            step++;
        }
    }
    else{
        if( tick == 0 ){
            lidarData.scanDir = 1;
        }else{
                tick--;
        }
        if( step == 0 ){
            step = 3;
        }
        else{
            step--;
        }
    }
    // send the angle
    sendData.data.val_ui32 = getDistance();
    sendData.type = tick;
    json_from_message_t(&sendData, &dbgData);
    wifly_SendValToMessageQ( &dbgData );
    increment_seq_num();
 }

void i2cTimerCallback(TimerHandle_t xTimer)
{
    static wifly_message_t dbgData;
    if (lidarData.finish_init == false)
    {
        if (_DRV_I2C_IsQueueEmpty(lidarData.dObj) == true && lidarData.count < 177 && init[lidarData.count] != 153 && init[lidarData.count] != 152 && init[lidarData.count] != 151 
                && init[lidarData.count] != 149 && init[lidarData.count] != 147 && init[lidarData.count] != 144){
            DRV_I2C_Transmit(lidarData.handleI2C0,appSlaveAddress,&init[lidarData.count], 2, NULL);
            lidarData.count++;
        }
        else if (_DRV_I2C_IsQueueEmpty(lidarData.dObj) == true && init[lidarData.count] == 153){
            lidarData.count++;
            DRV_I2C_Transmit(lidarData.handleI2C0,appSlaveAddress, &init[lidarData.count], 1, NULL);
            lidarData.count++;
        }
        else if (_DRV_I2C_IsQueueEmpty(lidarData.dObj) == true && init[lidarData.count] == 152){
            if (lidarData.stop_init == false){
                lidarData.I2CBufferHandle = DRV_I2C_Receive(lidarData.handleI2C0, appSlaveAddress, &lidarData.stop_variable, 1, NULL);
                lidarData.stop_init = true;
            }
            else 
                lidarData.I2CBufferHandle = DRV_I2C_Receive(lidarData.handleI2C0, appSlaveAddress, &lidarData.data, 1, NULL);
            lidarData.count++;
        }
        else if (_DRV_I2C_IsQueueEmpty(lidarData.dObj) == true && init[lidarData.count] == 151){
            DRV_I2C_Transmit(lidarData.handleI2C0,appSlaveAddress, &sp1[lidarData.count2], 3, NULL);
            lidarData.count++;
            lidarData.count2++;
        }
        else if (_DRV_I2C_IsQueueEmpty(lidarData.dObj) == true && init[lidarData.count] == 149){
            lidarData.I2CBufferHandle = DRV_I2C_Receive(lidarData.handleI2C0, appSlaveAddress, &lidarData.buffer_6bytes, 6, NULL);
            lidarData.count++;
        }
        else if (_DRV_I2C_IsQueueEmpty(lidarData.dObj) == true && init[lidarData.count] == 147){
            DRV_I2C_Transmit(lidarData.handleI2C0,appSlaveAddress, &sp2, 7, NULL);
            lidarData.count++;
        }
        else if (_DRV_I2C_IsQueueEmpty(lidarData.dObj) == true && init[lidarData.count] == 144){
            lidarData.I2CBufferHandle = DRV_I2C_Receive(lidarData.handleI2C0, appSlaveAddress, &lidarData.data, 2, NULL);
            lidarData.count++;
        }
        else
        {
            lidarData.finish_init = true;
            lidarData.calibrate = true;
        }
    }
    if (lidarData.calibrate == true)
    {
        if (_DRV_I2C_IsQueueEmpty(lidarData.dObj) == true && lidarData.calibration_count < 14 && calibration_val[lidarData.calibration_count] != 153 && calibration_val[lidarData.calibration_count] != 152)
        {
            DRV_I2C_Transmit(lidarData.handleI2C0,appSlaveAddress,&calibration_val[lidarData.calibration_count], 2, NULL);
            lidarData.calibration_count++;
        }
        else if (_DRV_I2C_IsQueueEmpty(lidarData.dObj) == true && calibration_val[lidarData.calibration_count] == 153)
        {
            lidarData.calibration_count++;
            DRV_I2C_Transmit(lidarData.handleI2C0,appSlaveAddress, &calibration_val[lidarData.calibration_count], 1, NULL);
            lidarData.calibration_count++;
        }
        else if (_DRV_I2C_IsQueueEmpty(lidarData.dObj) == true && calibration_val[lidarData.calibration_count] == 152)
        {
            lidarData.I2CBufferHandle = DRV_I2C_Receive(lidarData.handleI2C0, appSlaveAddress, &lidarData.calibration_data, 1, NULL);
            lidarData.calibration_count++;
            if (lidarData.calibration_data == 64)
                lidarData.calibration_count -= 3;
        }
        else
        {
            lidarData.calibrate = false;
            lidarData.read_dist = true;
            dbgData.size = sprintf(
                dbgData.buffer,
                "{\"DEBUG\":\"LIDAR CONFIGURED\"}"
            );
            wifly_SendValToMessageQ( &dbgData );
        }
    }
    if (lidarData.read_dist == true)
    {
        if (_DRV_I2C_IsQueueEmpty(lidarData.dObj) == true && lidarData.read_count < 17 && dist_reg[lidarData.read_count] != 153 && dist_reg[lidarData.read_count] != 152 && dist_reg[lidarData.read_count] != 144)
        {
            DRV_I2C_Transmit(lidarData.handleI2C0,appSlaveAddress,&dist_reg[lidarData.read_count], 2, NULL);
            lidarData.read_count++;
        }
        else if (_DRV_I2C_IsQueueEmpty(lidarData.dObj) == true && dist_reg[lidarData.read_count] == 153)
        {
            lidarData.read_count++;
            DRV_I2C_Transmit(lidarData.handleI2C0,appSlaveAddress,&dist_reg[lidarData.read_count], 1, NULL);
            lidarData.read_count++;
        }
        else if (_DRV_I2C_IsQueueEmpty(lidarData.dObj) == true && dist_reg[lidarData.read_count] == 152)
        {
            lidarData.I2CBufferHandle = DRV_I2C_Receive(lidarData.handleI2C0, appSlaveAddress, &lidarData.read_temp, 1, NULL);
            lidarData.read_count++;
            if (lidarData.read_temp == 64)
                lidarData.read_count -= 3;
        }
        else if (_DRV_I2C_IsQueueEmpty(lidarData.dObj) == true && dist_reg[lidarData.read_count] == 144)
        {
            lidarData.I2CBufferHandle = DRV_I2C_Receive(lidarData.handleI2C0, appSlaveAddress, &lidarData.distance, 2, NULL);
            lidarData.read_count++;
        }
        else
        {
            lidarData.distance = (lidarData.distance>>8) | (lidarData.distance<<8);
            //DRV_I2C_Transmit(lidarData.handleI2C0,appSlaveAddress,&lidarData.distance, 2, NULL);
            lidarData.read_count = 0;
        }
    }
}

void LIDAR_Initialize ( void )
{
    //init debug if needed
    if( !isDebugInit() )
        debugInit();
    
    dbgOutputLoc( LIDAR_TASK_INIT );
    
    //create the queue
    lidarData.lidarQueue_h = xQueueCreate( 5, sizeof( message_t ) );
    
    //check queue create
    if( lidarData.lidarQueue_h == NULL) //fail
        dbgError( ERROR_CREATING_QUEUE );
    
    //create the timer callback
    lidarData.stepperTimer_h = xTimerCreate(
            "Stepper Timer",
            60 / portTICK_PERIOD_MS,
            pdTRUE,
            (void *) 0,
            stepperCallback
        );
    //check
    if( lidarData.stepperTimer_h == NULL ){
        dbgError( ERROR_STARING_TIMER );
    }
    
    lidarData.scanDir = false; //right
    lidarData.scanning = false;
    
    
    lidarData.i2cTimer = xTimerCreate(
            "I2C Timer",
            20 / portTICK_PERIOD_MS,
            pdTRUE,
            (void *) 0,
            i2cTimerCallback
    );

    bool appInitialized = true;
    lidarData.stop_init = false;
    lidarData.finish_init = false;
    lidarData.calibrate = false;
    lidarData.f = false;
    lidarData.read_dist = false;
    lidarData.count = 0;
    lidarData.count2 = 0;
    lidarData.calibration_count = 0;
    lidarData.read_count = 0;
    lidarData.handleI2C0 = DRV_HANDLE_INVALID;
    if (lidarData.handleI2C0 == DRV_HANDLE_INVALID)
    {
        lidarData.handleI2C0 = DRV_I2C_Open(0, DRV_IO_INTENT_READ|DRV_IO_INTENT_WRITE);
        appInitialized &= ( DRV_HANDLE_INVALID != lidarData.handleI2C0 );
    }
    lidarData.dObj = (DRV_I2C_OBJ*)sysObj.drvI2C0;
    
}


/******************************************************************************
  Function:
    void LIDAR_Tasks ( void )

  Remarks:
    See prototype in lidar.h.
 */

void LIDAR_Tasks ( void )
{
    //ERROR var
    DEBUG_CONSTANTS error;
    
    SAFE(dbgOutputLoc( LIDAR_TASK_ENTER ), error);
    SAFE(dbgOutputLoc( LIDAR_TASK_BEFORE_WHILE ), error);
    
    //local data
    message_t recvData;
    message_t sendData;
    MESSAGE_DATA dataUnion;
    wifly_message_t dbgData;
    
    if( xTimerStart(lidarData.i2cTimer, 0) != pdPASS ){
            dbgError( ERROR_STARING_TIMER );
    }
    while( 1 ){
        //wait for queue
        SAFE(dbgOutputLoc( LIDAR_TASK_WAIT_FOR_QUEUE ), error);
        if( xQueueReceive( lidarData.lidarQueue_h, &recvData, portMAX_DELAY) == pdFALSE )
            dbgError(ERROR_RECEIVEING_FROM_QUEUE);
        
        SAFE( dbgOutputLoc( LIDAR_TASK_GOT_DATA_FROM_QUEUE ), error);
        
        //process
        SAFE( dbgOutputLoc( LIDAR_TASK_PROCESS_DATA ), error);

        //TODO: add processing
        switch( recvData.cmd ){
            case TEST:
                dataUnion.val_bool = true;
                sendData = message_factory(LIDAR_MESSAGE, BOOL, dataUnion);
                break;
            case LIDAR_REINIT:
                lidarData.stop_init = false;
                lidarData.finish_init = false;
                lidarData.calibrate = false;
                lidarData.f = false;
                lidarData.read_dist = false;
                lidarData.count = 0;
                lidarData.count2 = 0;
                lidarData.calibration_count = 0;
                lidarData.read_count = 0;
                DRV_I2C_Close(lidarData.handleI2C0);
                lidarData.handleI2C0 = DRV_I2C_Open(0, DRV_IO_INTENT_READ|DRV_IO_INTENT_WRITE);
                break;
            case PIC_CHANGE_LIDAR_DIR:
                lidarData.scanDir = recvData.data.val_bool;
                dbgData.size = sprintf(
                dbgData.buffer,
                "{\"DEBUG\":\"SUCCESS; Changed LIDAR Dir to %u\"}",
                        lidarData.scanDir
                );
                wifly_SendValToMessageQ( &dbgData );
                break;
            case PIC_START_STOP_LIDAR:
                if(lidarData.scanning){
                    //stop 
                    xTimerStop(lidarData.stepperTimer_h, 0);
                    lidarData.scanning = 0;
                }else{
                    if( xTimerStart( lidarData.stepperTimer_h, 0) != pdPASS ){
                        dbgError( ERROR_STARING_TIMER );
                    }
                    lidarData.scanning = 1;
                }
                dbgData.size = sprintf(
                dbgData.buffer,
                "{\"DEBUG\":\"SUCCESS; Changed LIDAR %s\"}",
                        lidarData.scanning?"scanning":"stopped"
                );
                wifly_SendValToMessageQ( &dbgData );
                break;
            default: //do nothing and go back to waiting
                continue;
        }
    }
    SAFE(dbgOutputLoc( LIDAR_TASK_LEAVING ), error);
}
