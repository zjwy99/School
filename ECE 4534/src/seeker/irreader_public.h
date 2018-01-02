/**
 * Public functions that allow access to private members of the ir_reader
 */
#ifndef __IR_READER_PUBLIC
#define __IR_READER_PUBLIC

#include "FreeRTOS.h"
#include <stdint.h>

//function to add a value to the queue
void irReaderSendValToMessageQ(message_t* val);

//function to add a value to the queue from and ISR
void irReaderSendValToMessageQFromISR(message_t* val);

#endif
