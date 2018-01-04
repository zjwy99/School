#ifndef __LIDAR_PUBLIC_H
#define __LIDAR_PUBLIC_H

//typedefs
#include "FreeRTOS.h"
#include "message.h"

//function to add a value to the queue
void lidar_SendValToMessageQ(message_t *val);

//function to add a value to the queue from and ISR
void lidar_SendValToMessageQFromISR(message_t *val);

#endif
