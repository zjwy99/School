#ifndef __MAP_MGR_PUBLIC_H
#define __MAP_MGR_PUBLIC_H

//typedefs
#include "FreeRTOS.h"
#include "message.h"

//function to add a value to the queue
void map_mgr_SendValToMessageQ(message_t val);

//function to add a value to the queue from and ISR
void map_mgr_SendValToMessageQFromISR(message_t val);

#endif
