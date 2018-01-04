#ifndef __WIFLY_PUBLIC_H
#define __WIFLY_PUBLIC_H

#include "wifly.h" //get BUFFER_SIZE, wifly_message_t

//function to add a value to the queue
void wifly_SendValToMessageQ(wifly_message_t* val);

//function to add a value to the queue from and ISR
void wifly_SendValToMessageQFromISR(wifly_message_t* val);

uint32_t get_seq_num();

void increment_seq_num();

bool wifly_MessageQFull();
#endif
