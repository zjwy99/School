#include "config.h"

static config_vals config;

void set_enable_testing( bool val ){
    config.enable_testing = val;
}

void set_test_number( uint32_t val ){
    config.test_number = val;
}

bool is_enable_testing(){
    return config.enable_testing;
}

uint32_t get_test_number(){
    return config.test_number;
}