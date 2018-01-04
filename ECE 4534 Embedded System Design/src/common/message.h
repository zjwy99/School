#ifndef __MESSAGE_H    /* Guard against multiple inclusion */
#define __MESSAGE_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

//types of supported commands
typedef enum{
    FAKE,                               //0x0
            TEST,                       //0x01
            IR_MESSAGE,                 //0x02
            IR_MESSAGE_FROM_ISR,        //0x03
            MOTOR_MESSAGE,              //0x04
            MAP_MESSAGE,                //0x05
            LIDAR_MESSAGE,              //0x06
            WIFLY_MESSAGE,              //0x07
            WIFLY_MESSAGE_START,        //0x08
            WIFLY_MESSAGE_SEND_TO_DB,   //0x09
            WIFLY_MESSAGE_READ_FROM_DB, //0x0a
            
            START,                      //0x0b
            STOP,                        //0x0c
            
            WIFLY_RECV,                 //0x0d
            NAV_RECV,                   //0x0e
            NAV_SEND,                   //0x0f
               
            SERVER_STORE,               //0x10
            SERVER_LOAD,                //ox11
            SERVER_GET_CONFIG,          //0x12
            PIC_CONFIG_ENABLE_TESTING,  //0x13
            PIC_CONFIG_TEST_NUMBER,     //0x14
            
            PIC_MESSAGE_RATE,           //0x15
            PIC_CHANGE_TEST_PERIOD,     //0x16
            PIC_NAV_START,              //0x17
            PIC_NAV_SET_DEST,           //0x18
            PIC_NAV_SET_START,          //0x19
            PIC_GET_MAP,                //0x1a
            PIC_GET_PATH,               //0x1b
            
            SERVER_MAP_POINT,           //0x1c -> update from PIC
            PIC_MAP_POINT,              //0x1d -> update from server
            PIC_NAV_STOP,               //0x1e
            PIC_GET_CUR_POS,            //0x1f
            SERVER_CUR_POS,             //0x20
            PIC_GET_POINT,              //0x21
            SERVER_GET_POINT,           //0x22
            
            MESSAGE_RATE_ENABLE,        //0x23
            SERVER_DONE_SENDING_MAP,    //0x24
            PIC_PEEK_MAP,               //0x25
            
            SERVER_GET_MAP,             //0x26
            PIC_MAP_POINT_BLOCK,        //0x27
            PIC_CHANGE_LIDAR_DIR,       //0x28
            SERVER_LIDAR_READING,       //0x29
            PIC_START_STOP_LIDAR,       //0x2a
            
            MOTOR_FORWARD,              //0x2b
            MOTOR_BACKWARD,             //0x2c
            MOTOR_LEFT,                 //0x2d
            MOTOR_RIGHT,                //0x2e
            MOTOR_STOP,                 //0x2f
            
            MANUAL_FORWARD,             //0x30
            MANUAL_BACKWARD,            //0x31
            MANUAL_LEFT,                //0x32
            MANUAL_RIGHT                //0x33
            
} MESSAGE_COMMAND;

//supported data types  
typedef enum{
    UINT32 = 0,
            UINT16 =1,
            UINT8=2,
            INT32=3,
            INT16=4,
            INT8=5,
            FLOAT=6,
            BOOL=7
} MESSAGE_DATA_T;

//supported data
typedef union{
        uint32_t val_ui32;
        uint16_t val_ui16;
        uint8_t  val_ui8;
        int32_t  val_i32;
        int16_t  val_i16;
        int8_t   val_i8;
        bool     val_bool;
        float    val_float;
} MESSAGE_DATA;

//message def
typedef struct{
    MESSAGE_COMMAND cmd;
    MESSAGE_DATA_T type;
    MESSAGE_DATA data;
} message_t;

message_t message_factory( MESSAGE_COMMAND, MESSAGE_DATA_T, MESSAGE_DATA );

#endif