#include "json_parse.h"
#include "jsmn.h"
#include "utils.h"
#include "wifly_public.h"
#include <stdio.h>

static char *test = "{\"checksum\":77665588,\"seq_num\":0,\"payload\":{\"cmd\":12,\"type\":6,\"value\":12.1}}";

static int jsoneq(const char *json, jsmntok_t *tok, const char *s) {
    if (tok->type == JSMN_STRING && (int) strlen(s) == tok->end - tok->start &&
            strncmp(json + tok->start, s, tok->end - tok->start) == 0) {
        return 0;
    }
    return -1;
}

int32_t get_values( 
        char *json, 
        uint32_t *checksum, 
        uint32_t *calculated_checksum, 
        uint32_t *seq_num, 
        MESSAGE_COMMAND *cmd, 
        MESSAGE_DATA_T *type, 
        MESSAGE_DATA *value )
{
    int i;
    jsmn_parser parse;
    jsmn_init(&parse);

    jsmntok_t t[TOKS];
    int r = jsmn_parse(&parse, json, strlen(json), t, TOKS);

    if( r < 0 ){
        return r;
    }

    //check for top level to be object
    if ( r != 13 || t[0].type != JSMN_OBJECT ){
        return r;
    }
    
    //now iterate over all of the keys
    for( i = 1; i < r; i++ ){
        if( jsoneq( json, &t[i], "checksum" ) == 0 ){
            *checksum = strtoul( json + t[i+1].start, NULL, 0);
            i++;
        }
        else if( jsoneq( json, &t[i], "seq_num" ) == 0 ){
            *seq_num = strtoul( json + t[i+1].start, NULL, 0);
            i++;
        }
        //object json
        else if( jsoneq( json, &t[i], "payload" ) == 0 ){
            int j;
            if( t[i+1].type != JSMN_OBJECT ){
                continue;
            }
            //checksum
            *calculated_checksum = sdbm((uint8_t*)json + t[i+1].start, t[i+1].end - t[i+1].start);
            i++;
        }
        else if( jsoneq( json, &t[i], "cmd" ) == 0 ){
            *cmd = strtoul( json + t[i+1].start, NULL, 0);
            i++;
        }
        else if( jsoneq( json, &t[i], "type" ) == 0 ){
            *type = strtoul( json + t[i+1].start, NULL, 0);
            i++;
        }
        else if( jsoneq( json, &t[i], "data" ) == 0 ){
            value->val_ui32 = strtoul( json + t[i+1].start, NULL, 0);
            i++;
        }
    }

    return r;
}

void json_from_message_t(message_t* message, wifly_message_t* json){
    //temp buffer
    static char temp[BUFFER_SIZE];
    static size_t temp_size;
    temp_size = sprintf(temp, "{\"cmd\": %u, \"type\": %u, \"data\": %u}", message->cmd, message->type, message->data.val_ui32);
    
    //get checksum
    uint32_t check = sdbm(temp, temp_size);
    
    //make final message
    json->size = sprintf(json->buffer, "{\"checksum\": %u, \"seq_num\": %u, \"payload\": %.*s}", check, get_seq_num(), temp_size, temp);
}

void json_from_string(MESSAGE_COMMAND cmd, MESSAGE_DATA_T type, char* data, wifly_message_t* json){
    //temp buffer
    static char temp[BUFFER_SIZE];
    static size_t temp_size;
    temp_size = snprintf(temp, BUFFER_SIZE, "{\"cmd\": %u, \"type\": %u, \"data\": \"%s\"}", cmd, type, data);
    
    //get checksum
    uint32_t check = sdbm(temp, temp_size);
    
    //make final message
    json->size = snprintf(json->buffer, BUFFER_SIZE, "{\"checksum\": %u, \"seq_num\": %u, \"payload\": %.*s}", check, get_seq_num(), temp_size, temp);
}
