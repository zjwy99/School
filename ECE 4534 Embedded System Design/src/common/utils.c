#include "utils.h"

uint32_t sdbm( uint8_t *str, size_t size ){
    uint32_t hash = 0;

    int i;
    for( i = 0; i < size; i++ )
        hash = str[i] + ( hash << 6 ) + ( hash << 16 ) - hash;
    
    return hash;
}