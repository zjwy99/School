#ifndef _NAVIGATION_H
#define _NAVIGATION_H

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "system_config.h"
#include "system_definitions.h"
#include "queue.h"
#include "timers.h"

typedef enum{
            WAIT_FOR_START = 0x0F,
            WAIT_FOR_DATA_REQUESTS = 0x10,
            SEND_TO_SERVER = 0x11,
            LOAD_FROM_SERVER = 0x12,
            EXEC_PATH
} NAVIGATION_STATE;

typedef enum{
    UP = 4,
    DOWN = 6,
    RIGHT = 5,
    LEFT = 7
} DIRECTION;

/**
 * 
 */
typedef struct
{
    QueueHandle_t navQueue_h;
    NAVIGATION_STATE state;
    TimerHandle_t pathTimer_h;
    TimerHandle_t mapSendTimer_h;
    uint8_t curx, cury;
    uint8_t nextx, nexty;
    uint8_t destx, desty;
    uint8_t path[512];
    int16_t steps;
    int16_t curStep;
    DIRECTION curDir;
    DIRECTION nextDir;
    bool turned;
    bool peek;
} NAVIGATION_DATA;

/**
 * Setup the queue
 */
void NAVIGATION_Initialize ( void );


/**
 * Wait for queue data and process incoming commands and data
 */
void NAVIGATION_Tasks( void );


#endif /* _NAVIGATION_H */