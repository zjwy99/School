#ifndef __UTILS_H
#define __UTILS_H

#include <stdint.h>
#include <stddef.h>

/**
 * Simple string checksum
 * @param str
 * @return digest of the string
 */
uint32_t sdbm( uint8_t *str, size_t size );

#endif