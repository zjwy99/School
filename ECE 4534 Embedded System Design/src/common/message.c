#include "message.h"

message_t message_factory(MESSAGE_COMMAND c, MESSAGE_DATA_T t, MESSAGE_DATA d){
    message_t retVal = { .cmd = c, .type = t, .data = d };
    
    return retVal;
}