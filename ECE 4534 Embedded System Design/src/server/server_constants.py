#!/usr/bin/env python3

# Settings Module
# Server Settings

PORTA = 10000
PORTB = 20000
PORTC = 30000
PORT_CTRL = 40000

# Get From PIC
SERVER_STORE = 0x10
SERVER_LOAD = 0x11
SERVER_GET_CONFIG = 0x12
SERVER_MAP_POINT = 0x1c
PIC_MESSAGE_RATE = 0x15
SERVER_DONE_SENDING_MAP = 0x24
SERVER_GET_MAP = 0x26	
SERVER_LIDAR_READING = 0x29

IR_MESSAGE = 0x02

# Send to PIC
PIC_CONFIG_ENABLE_TESTING = 0x13
PIC_CONFIG_TEST_NUMBER = 0x14
PIC_MAP_POINT = 0x1d
PIC_MAP_POINT_BLOCK = 0x27
PIC_GET_MAP = 0x1a
FAKE = 0x00
TEST = 0x01

# Test Cases
TEST1 = 1
TEST2 = 2
TEST3 = 3
TEST4 = 4
TEST5 = 5
TEST6 = 6

# Data Types
UINT32 = 0
UINT16 = 1
UINT8 = 2
INT32 = 3
INT16 = 4
INT8 = 5
FLOAT = 6
BOOL = 7

# Message Delimiters
BEGIN_MSG_DELIM = b'@'
END_MSG_DELIM = b'!'

# Message size
SIZE = 256

# invlid data
INVALID = -256
