#ifndef _WIFLY_H
#define _WIFLY_H

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
#include "message.h"
#include "timers.h"

//one secone
#define ONE_SEC         ( 1000 / portTICK_PERIOD_MS )

/* Application Data
 */

#define WIFLY_TX_INT_SRC INT_SOURCE_USART_1_TRANSMIT
#define WIFLY_RX_INT_SRC INT_SOURCE_USART_1_RECEIVE

#define BEGIN_MSG_DELIM '@'
#define END_MSG_DELIM '!'

#define BUFFER_SIZE 256

//typedefs
typedef struct{
    size_t size;
    char buffer[BUFFER_SIZE];
} wifly_message_t;

typedef enum{
    IDLE = 0x01,
    RECEIVING = 0x11
} RX_STATE;

typedef enum{
    BEGIN,
    SENDING,
    END
} TX_STATE;

typedef struct
{
    QueueHandle_t wiflyQueue_h;
    QueueHandle_t receiveQueue_h;
    TimerHandle_t rateTimer_h;
    size_t message_count;
    size_t seq_number;
} WIFLY_DATA;

typedef enum{
    META_GOOD,
    BAD_CHECKSUM,
    BAD_SEQ_NUMBER
} digest_result;


/**
*/

void WIFLY_Initialize ( void );


/**
 */

void WIFLY_Tasks( void );

void wifly_txInterruptHandler();

void wifly_rxInterruptHandler();

digest_result digest_metadata( uint32_t *checksum, uint32_t *calculatedChecksum, uint32_t *seqNumber);

void _wifly_internal_SendRecvVal(wifly_message_t* val);

//function to add a value to the queue from and ISR
void _wifly_internal_SendRecvValFromISR(wifly_message_t* val);

#endif /* _WIFLY_H */