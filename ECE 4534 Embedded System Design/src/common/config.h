#ifndef __CONFIG_H
#define __CONFIG_H

#include <stdint.h>
#include <stdbool.h>

typedef struct{
    bool enable_testing;
    uint32_t test_number;
} config_vals;

void set_enable_testing( bool );

void set_test_number( uint32_t );

bool is_enable_testing();

uint32_t get_test_number();

#endif