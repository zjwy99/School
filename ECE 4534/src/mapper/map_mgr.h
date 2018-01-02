#ifndef _MAP_MGR_H
#define _MAP_MGR_H

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

/* Application Data
 */

typedef struct
{
    QueueHandle_t map_mgrQueue_h;
} MAP_MGR_DATA;


/**
*/

void MAP_MGR_Initialize ( void );


/**
 */

void MAP_MGR_Tasks( void );


#endif /* _MAP_MGR_H */