#ifndef _MOTOR_H
#define _MOTOR_H

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


/* DEFINE */
#define MOTOR_MAX_PULSE_WIDTH 40000
    
#define MOTOR1_DIRECTION_CHANNEL PORT_CHANNEL_C
#define MOTOR1_DIRECTION_BIT 14
    
#define MOTOR2_DIRECTION_CHANNEL PORT_CHANNEL_G
#define MOTOR2_DIRECTION_BIT 1

#define MOTOR_TICKS_PER_CM 145.89
#define MOTOR_PI 3.14

/* Steven's = 10.4 */
/* Nicky's = 10.7 */
/* Zach's = 10.0 */
#define ROVER_DIAMETER 10.4

/* Motor direction */
typedef enum
{
    MOTOR_REVERSE = 1,
    MOTOR_ONWARD = 0
} MOTOR_DIRECTION;

/* Motor state */
typedef enum
{
    MOTOR_SERVICE_TASKS,
    MOTOR_MOVEMENT,
    MOTOR_MANUAL
} MOTOR_STATE;

/* Motor Data */
typedef struct
{
    /* Movements data */
    MOTOR_DIRECTION motor1_direction;
    MOTOR_DIRECTION motor2_direction;
    int speed;
    /* Motor tick */
    uint32_t total_tick;
    uint32_t current_tick;
    uint32_t target_tick;
    /* Motor state */
    MOTOR_STATE state;
    /* Queue handler*/
    QueueHandle_t motorQueue_h;
    /* Saved state */
    MOTOR_DIRECTION saved_motor1_direction;
    MOTOR_DIRECTION saved_motor2_direction;
    int saved_speed;
    uint32_t saved_current_tick;
    uint32_t saved_target_tick;
    MOTOR_STATE saved_state;
} MOTOR_DATA;

/*----------------------------- Init and main task -----------------------------*/

/* Init function */
void MOTOR_Initialize ( void );

/* Main task */
void MOTOR_Tasks( void );

/*----------------------------- Helper functions -----------------------------*/

/* Returns the number of cycles required to set motor speed from input range 0-100 */
int get_cycle(int speed);

/* Reset counter register */
void reset_counter_reg(void);

/* Save the current motor state */
void save_current_state(void);

/* Save the saved motor state */
void load_saved_state(void);

/*----------------------------- Motor functions -----------------------------*/

/* Set motor1 power % to speed range from 0-100 */
void motor1_set_pulse(int speed);

/* Set motor2 power % to speed range from 0-100 */
void motor2_set_pulse(int speed);

/* Set motor1 power % and direction */
void motor1_set(int speed, MOTOR_DIRECTION direction);

/* Set motor2 power % and direction */
void motor2_set(int speed, MOTOR_DIRECTION direction);

/*----------------------------- Movement functions -----------------------------*/

/* Stop rover */
void rover_stop(void);

/* Move rover in a straight line to a certain distance */
void rover_move_straight(int speed, MOTOR_DIRECTION direction, uint32_t distance);

/* Turn rover in degrees, clockwise */
void rover_turn_cw(int speed, float degree);

/* Turn rover in degrees, counter clockwise */
void rover_turn_ccw(int speed, float degree);

/*----------------------------- ISR function -----------------------------*/

/* Update tick count */
void update_tick(void);

#endif /* _MOTOR_H */