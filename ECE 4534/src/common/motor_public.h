#ifndef __MOTOR_PUBLIC_H
#define __MOTOR_PUBLIC_H

//typedefs
#include "FreeRTOS.h"
#include "message.h"
#include "motor.h"

//function to add a value to the queue
void motor_SendValToMessageQ(message_t* val);

//function to add a value to the queue from and ISR
void motor_SendValToMessageQFromISR(message_t* val);

#endif
