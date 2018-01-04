#ifndef _LIDAR_H
#define _LIDAR_H

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
#include "driver/i2c/drv_i2c.h"
#include "driver/i2c/drv_i2c_compatibility.h"
#include "driver/i2c/src/drv_i2c_local.h"

const I2C_SLAVE_ADDRESS_VALUE appSlaveAddress = 0x52;


/* Application Data
 */

typedef struct
{
    QueueHandle_t lidarQueue_h;
    TimerHandle_t stepperTimer_h;
    bool scanDir;
    bool scanning;
    
    TimerHandle_t i2cTimer;
    DRV_HANDLE                              handleI2C0;
    DRV_I2C_BUFFER_HANDLE                   I2CBufferHandle;
    DRV_I2C_BUFFER_EVENT                    I2CBufferEvent;
    DRV_I2C_BUFFER_EVENT operationStatus;
    SYS_STATUS          i2c_status;
     unsigned char buffer_2bytes[2];
    unsigned char buffer_1byte[1];
    unsigned char buffer_6bytes[6];
    int int_buff[100][2];
    uint8_t stop_variable, calibration_data, read_temp;
    bool stop_init, finish_init, calibrate, read_dist;
    uint16_t data;
    DRV_I2C_OBJ             *dObj;
    uint16_t count, count2, calibration_count, read_count;
    int sensorValue;
    uint16_t distance;
    bool f;
    
} LIDAR_DATA;



/**
*/

void LIDAR_Initialize ( void );


/**
 */

void LIDAR_Tasks( void );

#endif /* _LIDAR_H */