#ifndef __JSON_PARSE_H
#define __JSON_PARSE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "message.h"
#include "wifly.h"

#define TOKS 20
#define EXPECTED_TOKENS 13

int32_t get_values( 
        char *json, 
        uint32_t *checksum, 
        uint32_t *calculated_checksum, 
        uint32_t *seq_num, 
        MESSAGE_COMMAND *cmd, 
        MESSAGE_DATA_T *type, 
        MESSAGE_DATA *value 
);

void json_from_message_t( message_t*, wifly_message_t* );

void json_from_string( MESSAGE_COMMAND, MESSAGE_DATA_T, char*, wifly_message_t*);

#endif