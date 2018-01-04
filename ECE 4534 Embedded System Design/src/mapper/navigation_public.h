#ifndef __NAVIGATION_PUBLIC_H
#define __NAVIGATION_PUBLIC_H

//typedefs
#include "FreeRTOS.h"
#include "message.h"

//function to add a value to the queue
void nav_SendValToMessageQ(message_t *val);

//function to add a value to the queue from and ISR
void nav_SendValToMessageQFromISR(message_t *val);

#endif