# Client program for testing server

import socket
import sys
import json
import hashlib
import random
import numpy as np
from PyQt5.QtCore import *
from pymongo import MongoClient
import logging

# Settings Module
import server_constants as S

def sdbm(L):
    h = np.uint32(0)
    six = np.uint32(6)
    sixteen = np.uint32(16)
    for c in L:
        h = np.uint32(ord(c) + (h << six) + (h << sixteen) - h)
    return h

class UserClient(QObject):
    def __init__(self,host,port):
        super(UserClient,self).__init__()
        self.host = host
        self.port = port
        self.client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.client.connect((self.host,self.port))
        #if here then good
        self.int_seq = 0
        client = MongoClient()
        db = client.embedded_database
        self.collection = db.milestone2
        #log
        self.log = logging.getLogger(str(self.port))
        self.log.setLevel(logging.DEBUG)
        fmt = logging.Formatter('%(levelname)s:%(message)s')
        debugHandler = logging.StreamHandler()
        debugHandler.setLevel(logging.DEBUG)
        debugHandler.setFormatter(fmt)
        self.log.addHandler(debugHandler)

    def close(self):
        self.client.shutdown(socket.SHUT_RDWR)
        self.client.close()

    def check_seq_number(self, message):
        if self.int_seq == message['seq_num']:
            #dont need
            self.log.info('seq number is valid')
            valid = True
        else:
            #handle desync
            self.log.debug('seq num = {}'.format(self.int_seq))
            self.int_seq = message['seq_num']
            self.log.warning('seq number is invalid')
            valid = False
        return valid

    def check_checksum(self, message):
        calc_recv_checksum = int(sdbm(json.dumps(message['payload'])))

        if message['checksum'] == calc_recv_checksum:
            data_valid = True
            self.log.info('checksum is valid...')
        else:
            data_valid = False
            self.log.warning('checksum is invalid...')

        return data_valid

    def validateMessage(self, message):
        if (self.check_seq_number(message), self.check_checksum(message)) == (True, True):
            return True
        else:
            return False

    def sendMessage(self, cmd, mes_type, mes_data, port=-1):
        # Form payload
        payload = {"cmd":cmd, "type":mes_type, "data":mes_data}
        payload = {"cmd":cmd, "type":port, "data":payload}
        jpayload = json.dumps(payload)
        # Calculate checksum
        checksum = int(sdbm(jpayload))
        # Form the message
        message = {"payload":payload, "checksum":checksum, "seq_num":self.int_seq}
        jmessage = '@' + json.dumps(message) + '!'
        # Sends message
        self.client.sendall(jmessage.encode())
        self.log.debug('sent {}'.format(jmessage.encode()))

    def readUntilEnd( self, recv_message ):
        #read message until ending delimiter
        while recv_message[-1] != ord(S.END_MSG_DELIM):
            recv_message = recv_message + self.client.recv(1)
        return recv_message

    def exec( self, cmd, mes_type, mes_data, port=-1):
        #check if database load
        if cmd == S.SERVER_LOAD:
            return self.collection.find_one({'type':mes_type})
        else:
            self.sendMessage(cmd, mes_type, mes_data, port)
            self.int_seq+=1
            #now wait for response
            while True:
                data = self.client.recv(1)
                if data:
                    if data == S.BEGIN_MSG_DELIM:
                        #get full message
                        message_s = self.readUntilEnd( data )
                        self.log.debug('got {}'.format(message_s))
                        #make json
                        message = json.loads(message_s[1:-1])
                        #validate
                        valid = self.validateMessage(message)
                        #increment
                        self.int_seq += 1
                        if valid:
                            #return json
                            return message
                        else:
                            return None
                else:
                    self.log.critical('disconnect')
                    break
        return None

if __name__ == '__main__':
    if len(sys.argv) == 3:
        server_ip = sys.argv[1]
        server_port = int(sys.argv[2])

        # Create a client
        client = UserClient(server_ip, server_port)

        while True:
            # Read from keyboard
            mes_cmd = int(input("enter a command: "), 16)
            mes_type = input("enter a type: ")
            mes_data = int(input("enter a data: "), 10)

            print('Got: {}'.format(client.exec(mes_cmd,mes_type,mes_data)))
