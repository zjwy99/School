#ifndef __PATHFINDING_H
#define __PATHFINDING_H

#include <stdint.h>

#define MAP_X 10
#define MAP_Y 10

#define MAX_QUEUE_DEPTH 300

uint8_t default_map[MAP_Y][MAP_X] = {0};
//uint8_t default_map[][MAP_X] = {
//        {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
//        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
//        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
//        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,1},
//        {1,0,0,1,1,1,1,0,0,0,0,0,0,1,0,1,0,0,0,1},
//        {1,0,0,1,0,0,1,0,0,0,0,0,1,0,0,1,0,0,0,1},
//        {1,0,0,1,0,0,1,0,0,0,0,1,0,0,0,1,1,1,1,1},
//        {1,0,0,1,1,1,1,0,0,0,1,0,0,0,0,1,0,0,0,1},
//        {1,0,0,0,0,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1},
//        {1,0,0,0,0,0,0,0,1,0,0,0,0,1,0,0,0,0,0,1},
//        {1,0,0,0,0,0,0,1,0,0,0,0,1,0,0,0,0,0,0,1},
//        {1,0,0,0,0,0,1,0,0,0,0,1,0,0,0,0,0,0,0,1},
//        {1,0,0,0,1,1,0,0,0,0,1,0,0,0,0,0,0,0,0,1},
//        {1,0,0,0,1,1,0,0,0,1,0,0,0,0,0,0,0,0,0,1},
//        {1,0,0,0,1,1,0,0,1,0,0,0,0,0,1,1,0,0,0,1},
//        {1,0,0,0,1,1,1,1,0,0,0,0,0,0,1,1,0,0,0,1},
//        {1,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1},
//        {1,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
//        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
//        {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
//    };

/**
 * Populates a given path array with the path from (x1,y1) -> (x2,y2).
 * 
 * @param grid - map
 * @param x1
 * @param y1
 * @param x2
 * @param y2
 * @param path - preallocated array. Must be able to hold (returned size * 2 + 2) bytes
 * @return the size of the path array (# of steps), 0 if none
 */
int16_t findPath( 
        uint8_t grid[][MAP_X], 
        uint8_t x1, uint8_t y1, 
        uint8_t x2, uint8_t y2,
        uint8_t *path
        );

/**
 * Breadth first search to find the closes map point with value '2'
 * @param grid
 * @param x2
 * @param y2
 * @param path
 * @return 
 */
int16_t findClosestUnknown(
        uint8_t grid[][MAP_X],
        uint8_t x2, uint8_t y2,
        uint8_t *path
        );
#endif