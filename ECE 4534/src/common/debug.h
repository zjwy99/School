/**
 * 
 */

#ifndef __DEBUG_FUNCTIONS_H
#define __DEBUG_FUNCTIONS_H

//includes
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "system_config.h"
#include "system_definitions.h"

#define DBG_UART_INT_SOURCE_TX INT_SOURCE_USART_4_TRANSMIT

typedef enum{
            UNINITIALIZED_ERROR = 0x01,
            FATAL_ERROR = 0x02,
            SUCCESS = 0x03,
            ERROR_CREATING_QUEUE = 0x4,
            ERROR_SENDING_TO_QUEUE = 0x5,
            ERROR_RECEIVEING_FROM_QUEUE = 0x6,
            ERROR_STARING_TIMER = 0x7,
            ERROR_OPENING_ADC = 0x8,
            ERROR_OPENING_UART = 0x9,
            BUFFER_OVERFLOW_PROTECTION = 0xa,
            JSON_PROCESSING_ERROR = 0xb
    
} DEBUG_CONSTANTS;

typedef enum DEBUG_LOCATIONS{
    NO_LOCATION = 0x0,
            SYS_TASK_ENTER = 0x1,
            SYS_TASK_PRE_WHILE = 0x2,
            
            IR_TASK_ENTER = 0x3,
            IR_TASK_INIT = 0x4,
            IR_TASK_WAIT_FOR_QUEUE = 0x5,
            IR_TASK_GOT_DATA_FROM_QUEUE = 0x6,
            IR_TASK_SEND_TO_UART = 0x7,
            IR_TASK_SEND_TO_GPIO = 0x8,
            IR_TASK_LEAVING = 0x9,
            
            DEBUG_FATAL_ERROR = 0xa,
            
            TIMER_ISR_ENTER = 0xb,
            TIMER_ISR_WAIT_FOR_ADC = 0x10,
            TIMER_ISR_GET_ADC = 0xc,
            TIMER_ISR_BEFORE_SEND_DATA = 0xd,
            TIMER_ISR_AFTER_SEND_DATA = 0xe,
            TIMER_ISR_LEAVING = 0xf,
            
            NAVIGATION_TASK_ENTER = 0x12,
            NAVIGATION_TASK_BEFORE_WHILE = 0x13,
            NAVIGATION_TASK_INIT = 0x14,
            NAVIGATION_TASK_WAIT_FOR_QUEUE = 0x15,
            NAVIGATION_TASK_GOT_DATA_FROM_QUEUE = 0x16,
            NAVIGATION_TASK_PROCESS_DATA = 0x17,
            NAVIGATION_TASK_LEAVING = 0x18,
            
            MOTOR_TASK_ENTER = 0x19,
            MOTOR_TASK_BEFORE_WHILE = 0x1a,
            MOTOR_TASK_INIT = 0x1b,
            MOTOR_TASK_WAIT_FOR_QUEUE = 0x1c,
            MOTOR_TASK_GOT_DATA_FROM_QUEUE = 0x1d,
            MOTOR_TASK_PROCESS_DATA = 0x1e,
            MOTOR_TASK_LEAVING = 0x1f,
            
            WIFLY_TASK_ENTER = 0x20,
            WIFLY_TASK_BEFORE_WHILE = 0x21,
            WIFLY_TASK_INIT = 0x22,
            WIFLY_TASK_WAIT_FOR_QUEUE = 0x23,
            WIFLY_TASK_GOT_DATA_FROM_QUEUE = 0x24,
            WIFLY_TASK_PROCESS_DATA = 0x25,
            WIFLY_TASK_LEAVING = 0x26,
            
            MAP_MGR_TASK_ENTER = 0x27,
            MAP_MGR_TASK_BEFORE_WHILE = 0x28,
            MAP_MGR_TASK_INIT = 0x29,
            MAP_MGR_TASK_WAIT_FOR_QUEUE = 0x2a,
            MAP_MGR_TASK_GOT_DATA_FROM_QUEUE = 0x2b,
            MAP_MGR_TASK_PROCESS_DATA = 0x2c,
            MAP_MGR_TASK_LEAVING = 0x2e,
            
            LIDAR_TASK_ENTER = 0x2f,
            LIDAR_TASK_BEFORE_WHILE = 0x30,
            LIDAR_TASK_INIT = 0x31,
            LIDAR_TASK_WAIT_FOR_QUEUE = 0x32,
            LIDAR_TASK_GOT_DATA_FROM_QUEUE = 0x33,
            LIDAR_TASK_PROCESS_DATA = 0x34,
            LIDAR_TASK_LEAVING = 0x35,
            
            NAV_GOT_START = 0x36,
            NAV_GOT_REPLY = 0x37,
            
            DEBUG_INIT = 0x11,
            
            WIFLY_TX_INT = 0x38,
            WIFLY_RX_INT = 0x39,
            
            WIFLY_GOT_VALUES = 0x3a,
            WIFLY_CHECKED_META = 0x3b,
            WIFLY_BAD_CHECKSUM = 0x3c,
            WIFLY_BAD_SEQ_NUM = 0x3d,
            
            WIFLY_TX_BEGIN = 0x3e,
            WIFLY_TX_SENDING = 0x3f,
            WIFLY_TX_END = 0x40
            
} DEBUG_LOCATION;

typedef struct{
    DRV_HANDLE uartHandle;
    bool initialized;
    uint8_t sendData;
} DEBUG_DATA;

/**
 * dbgUARTVal
 * - Takes in a uint8_t and outputs it to USART0
 * @param val - value to output
 * @returns success
 */
DEBUG_CONSTANTS dbgUARTVal( const uint8_t val);

/**
 * dbgOutputVal
 * - Takes in a uint8_t and outputs it on the Max32's
 * pins:
 *  From MSB->LSB {85 -> 78}
 */
DEBUG_CONSTANTS dbgOutputVal( const uint8_t val);


/**
 * Meant for outputting the current location of the code as defined in the
 * DEBUG_LOCATION struct
 * @param loc
 * @return success
 */
DEBUG_CONSTANTS dbgOutputLoc( DEBUG_LOCATION loc);

void dbgError( DEBUG_CONSTANTS );

void debugInit();

bool isDebugInit();

void dbg_txInterruptHandler();

#define SAFE(x, error){\
    if( (error = x) != SUCCESS ){\
        dbgError(error);\
    }\
}

#endif
