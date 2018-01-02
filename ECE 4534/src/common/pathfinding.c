#include "pathfinding.h"


/**
 * Pathfinding using a simplified Dijkstra breadth-first-search.
 * 
 * Inspired from https://stackoverflow.com/questions/38554515/most-memory-efficient-algorithm-for-finding-a-path-on-a-grid
 *  - changed everything to use statically allocated arrays, ect instead of calloc, realloc, malloc
 * 
 */
int16_t findPath(
        uint8_t grid[][MAP_X], 
        uint8_t x1, uint8_t y1, 
        uint8_t x2, uint8_t y2, 
        uint8_t* path) {
    int8_t dir[MAP_Y][MAP_X] = {0};                                 // allocate direction matrix: X*Y
    dir[y2][x2] = 5;                                                // mark starting cell as visited (search backwards)
    int8_t queue[MAX_QUEUE_DEPTH];                                  // allocate fifo queue: 152 bytes
    queue[0] = x2; queue[1] = y2;                                   // put starting cell in queue (search backwards)
    uint8_t qRead = 0, qWrite = 2;                                  // queue pointers
    int8_t qCurSize = 1, qNextSize = 0;                             // queue size per distance
    int16_t distance = 0;                                           // distance to current cell
    int8_t dx[4] = {0, 1, 0, -1};                                   // up, right, down, left
    while (qRead != qWrite && !dir[y1][x1]) {                       // until queue empty (fail) or target reached
        int8_t x = queue[qRead++], y = queue[qRead++];              // take oldest cell from queue
        qRead %= MAX_QUEUE_DEPTH;                                   // wrap-around queue pointer
        int8_t i;
        for (i = 0; i < 4; i++) {                                   // check 4 neighbouring cells
            int8_t nx = x + dx[i], ny = y + dx[3 - i];              // coordinates of neighbouring cell
            if (nx >= 0 && nx < MAP_X && ny >= 0 && ny < MAP_Y      // coordinates not off-grid &
               && !grid[ny][nx] && !dir[ny][nx] ) {                 // traversable unvisited cell
                dir[ny][nx] = i + 1;                                // store direction 1-4
                queue[qWrite++] = nx; queue[qWrite++] = ny;         // put cell in queue
                qWrite %= MAX_QUEUE_DEPTH;                          // wrap-around queue pointer
                ++qNextSize;                                        // increment queue size for next distance
            }
        }
        if (!--qCurSize || dir[y1][x1]) {                           // current distance done or target reached
            qCurSize = qNextSize;                                   // switch to distance + 1
            qNextSize = 0;
            ++distance;
        }
    }
    if (!dir[y1][x1]) distance = -1;                                // no path found
    else {                                                          // path found
        path[0] = x1; path[1] = y1;                                 // starting position (forward)
        int16_t i;
        for (i = 1; i <= distance; i++) {                           // retrace steps
            int8_t d = dir[y1][x1] - 1;                             // direction of previous step 0-3
            x1 -= dx[d]; y1 -= dx[3 - d];                           // go back to previous position
            path[i * 2] = x1; path[i * 2 + 1] = y1;                 // add cell to path
        }
    }
    return distance + 1;                                            // return number of cells in path
}

int16_t findClosestUnknown(
        uint8_t grid[][MAP_X],
        uint8_t x2, uint8_t y2,
        uint8_t *path) 
{
    int8_t dir[MAP_Y][MAP_X] = {0};                                 // allocate direction matrix: X*Y
    dir[y2][x2] = 5;                                                // mark starting cell as visited (search backwards)
    int8_t queue[MAX_QUEUE_DEPTH];                                  // allocate fifo queue: 152 bytes
    queue[0] = x2; queue[1] = y2;                                   // put starting cell in queue (search backwards)
    uint8_t qRead = 0, qWrite = 2;                                  // queue pointers
    int8_t qCurSize = 1, qNextSize = 0;                             // queue size per distance
    int16_t distance = 0;                                           // distance to current cell
    int8_t dx[4] = {0, 1, 0, -1};                                   // up, right, down, left
    while (qRead != qWrite) {                                                            // until queue empty (fail) or target reached
        int8_t x = queue[qRead++], y = queue[qRead++];              // take oldest cell from queue
        qRead %= MAX_QUEUE_DEPTH;                                   // wrap-around queue pointer
        int8_t i;
        for (i = 0; i < 4; i++) {                                   // check 4 neighbouring cells
            int8_t nx = x + dx[i], ny = y + dx[3 - i];              // coordinates of neighbouring cell
            if (nx >= 0 && nx < MAP_X && ny >= 0 && ny < MAP_Y      // coordinates not off-grid &
               && grid[ny][nx]!=1 && !dir[ny][nx] ) {               // traversable unvisited cell
                if(grid[ny][nx]==2){
                    if(findPath(grid, x2, y2, nx, ny, path) > 0){   //if here then return
                        return ny*MAP_Y+nx;
                    }
                }else{
                    queue[qWrite++] = nx; queue[qWrite++] = ny;     // put cell in queue
                    qWrite %= MAX_QUEUE_DEPTH;                      // wrap-around queue pointer
                    ++qNextSize;                                    // increment queue size for next distance
                }
                dir[ny][nx] = i + 1;                                // store direction 1-4 (mark visited)
            }
        }
        if (!--qCurSize) {                                          // current distance done or target reached
            qCurSize = qNextSize;                                   // switch to distance + 1
            qNextSize = 0;
            ++distance;
        }
    } 
    return -1;
}