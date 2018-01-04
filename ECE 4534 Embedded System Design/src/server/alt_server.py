
# Receives requests and sends back a response for each request
# NEED TO PRINT LOG FILE FOR EACH PORT

from pymongo import MongoClient
import pymongo
import threading
import socket
import sys
import json
import numpy as np
import logging
import logging.handlers
from concurrent.futures import ThreadPoolExecutor
from PyQt5.QtCore import *
from PyQt5.QtWidgets import QApplication
import pickle
import time
from ctypes import *

# Settings Module
import server_constants as S

def sdbm(L):
    h = np.uint32(0)
    six = np.uint32(6)
    sixteen = np.uint32(16)
    for c in L:
        h = np.uint32(ord(c) + (h << six) + (h << sixteen) - h)
    return h

class PickleDgramHandler(logging.handlers.DatagramHandler):

    def send(self, s):
        if self.sock is None:
            self.createSocket()
        self.sock.sendto(s, (self.host, self.port))

    def emit(self, record):
        try:
            s = pickle.dumps(record.getMessage())
            self.send(s)
        except (KeyboardInterrupt, SystemExit):
            raise
        except:
            self.handleError(record)

class Server(QObject):
    def __init__(self, host, port):
        super(Server, self).__init__()
        self.host = host
        self.port = port
        self.sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        self.sock.bind((self.host, self.port))
        self.log = logging.getLogger(str(self.port))
        self.log.setLevel(logging.DEBUG)
        fmt = logging.Formatter('%(levelname)s:%(name)s:%(message)s')
        debugHandler = logging.StreamHandler()
        debugHandler.setLevel(logging.DEBUG)
        debugHandler.setFormatter(fmt)

        #make a UDP handler
        infoHandler = PickleDgramHandler(self.host, self.port+1)
        print('UDP port opened on {}'.format(self.port+1))
        infoHandler.setLevel(logging.INFO)
        infoHandler.setFormatter(fmt)
        self.log.addHandler(debugHandler)
        self.log.addHandler(infoHandler)
        #
        self.int_seq = 0
        # Connects to the database
        client = MongoClient()
        db = client.embedded_database
        self.collection = db.milestone2
        #
        self.client = None
        #
        self.map = [0 for i in range(100)]

    def listen(self):
        self.sock.listen(5)
        while True:
            self.log.debug('Listening...')
            self.client, address = self.sock.accept()
            self.log.debug('accepted')
            self.client.setblocking(True)
            self.client.settimeout(15)
            self.listenToClient()
            self.client.shutdown(0)
            self.client.close()
            self.client = None

    def listenToClient( self ):
        while True:
            try:
                #check for begin delim
                data = self.client.recv(1)
                if data:
                    if data == S.BEGIN_MSG_DELIM: #process the rest of the message
                        #get full message
                        message_s = self.readUntilEnd( data )
                        self.log.debug('got {}'.format(message_s))
                        #make json
                        message = json.loads(message_s[1:-1])
                        #check message type
                        if 'DEBUG' in message:
                            self.processDebugMessage(message)
                        else: #normal message
                            #process for correctness
                            valid = self.validateMessage(message)
                            #increment
                            self.int_seq = self.int_seq + 1
                            if valid or message['payload']['cmd'] == S.SERVER_GET_CONFIG:
                                #process commands/debugging
                                self.processMessage(message)
                    # else: ignore
                else:
                    self.log.info('disconnect')
                    break
            except json.JSONDecodeError:
                #bad json
                self.log.critical('Unable to parse received string into JSON')
            except socket.timeout:
                #bad json
                self.log.critical('Timeout')
                break

    def database_op(self, recv_operation, recv_payload_dict):
        opres = recv_operation
        mes_type = recv_payload_dict['type']
        mes_data = S.INVALID
        try:
            if recv_operation == S.SERVER_STORE: # store
                insert_result = self.collection.replace_one({'type':mes_type},recv_payload_dict,upsert=True)
                mes_data = recv_payload_dict['data']
                self.log.debug("stored to database: {}".format(insert_result))
            elif recv_operation == S.SERVER_LOAD: # load
                find_result = self.collection.find_one({"type":mes_type})
                if find_result == None:
                    self.log.debug("error! not found in database...")
                else:
                    mes_type = find_result["type"]
                    mes_data = find_result["data"]
                    self.log.debug("found in database: {}".format(find_result))
            else:
                self.log.debug("error! invalid command...")
        except pymongo.errors.PyMongoError:
            self.log.debug("error! operation '{}' failed...".format(opres))
        return mes_type, mes_data, opres

    def processDebugMessage(self, message):
        #just print
        self.log.info(message['DEBUG'])

    def processMessage(self, message):
        #get payload
        payload = message['payload']
        cmd = payload['cmd']
        #process different requests
        if cmd == S.SERVER_STORE or cmd == S.SERVER_LOAD:
            res_type, res_data, res_cmd = self.database_op(cmd, payload)
            self.sendMessage(client, res_cmd, res_type, res_data)
            self.int_seq = self.int_seq + 1
        elif cmd == S.SERVER_GET_CONFIG:
            res_type, res_data, res_cmd = self.database_op(S.SERVER_LOAD, {'type':'enable_testing','data':'0x0'})
            self.sendMessage(S.PIC_CONFIG_ENABLE_TESTING, S.UINT32, res_data)
            self.int_seq = self.int_seq + 1
            # Sends test case number if testing is enabled
            if res_data == 1:
                # Loads from database and sends
                res_type, res_data, res_cmd = self.database_op(S.SERVER_LOAD, {'type':'test#','data':'0x0'})
                self.sendMessage(S.PIC_CONFIG_TEST_NUMBER, S.UINT32, res_data)
                self.int_seq = self.int_seq + 1
        elif cmd == S.PIC_MESSAGE_RATE:
            #just print out the reate
            self.log.info('Message Rate (PIC): {}'.format(payload['data']))
        elif cmd == S.SERVER_MAP_POINT:
            index = payload['type']
            value = payload['data']
            self.log.info('{}->{}'.format(index,value))
            #add to map array
            self.map[index] = value
        elif cmd == S.SERVER_DONE_SENDING_MAP:
            if payload['data'] == 0:
                res = self.collection.replace_one({'type':'map'},{'type':'map','map':self.map},upsert=True)
            self.log.info('MAP_{}: {}'.format(payload['data'],self.map))
        elif cmd == S.SERVER_GET_MAP:
            #load map from db
            find_result = self.collection.find_one({'type':'map'})
            if find_result != None:
                self.map = find_result['map']
            self.sendMap()
        elif cmd == S.PIC_GET_MAP:
            #get string and iterate through it
            lastIndex = payload['type']
            count = 100
            for point in payload['data']:
                curIndex = lastIndex-count
                self.map[curIndex] = int(point)
                count = count - 1
        elif cmd == S.SERVER_LIDAR_READING:
            #convert to float
            data = payload['data']
            self.log.info('tick:{},cm:{}'.format(payload['type'],data))
        elif cmd == S.IR_MESSAGE:
            data = payload['data']
            data = cast(pointer(c_uint32(data)), POINTER(c_float)).contents.value
            self.log.info('cm:{}'.format(data))
        else:
            self.log.warning('invalid cmd')

    def sendMap(self):
        #for each block of 32 bits send the block
        count = 0
        data = 0
        for i in self.map:
            #send 32 at a time
            #build the integer
            data += i
            count += 1
            #if time to send
            if count % 25 == 0 or count >= len(self.map):
                self.sendMessage( S.PIC_MAP_POINT_BLOCK, count, data )
                self.int_seq+=1
                time.sleep(0.045)
                data = 0
            else:
                data <<= 1

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

    def sendMessage(self, cmd, mes_type, mes_data):
        # Form payload
        payload = {"cmd":cmd, "type":mes_type, "data":mes_data}
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

    #@pyqtSlot()
    def receiveBroadcast(self, message):
        print(message)
        if self.client != None:
            try:
                self.log.info('got broadcast')
                if message['type'] == -1 or message['type'] == self.port:
                    self.log.info('sent broadcast')
                    message = message['data']
                    self.sendMessage(message['cmd'], message['type'], message['data'])
                    self.int_seq = self.int_seq + 1
            except:
                self.log.critical('Error processing broadcast')
        else:
            self.log.info('got broadcast, but not connected')

class ServerSignals(QObject):
    sendToServers = pyqtSignal(dict)
    empty = pyqtSignal()

class AdminServer(Server):

    def __init__(self, address, port):
        super(AdminServer, self).__init__(address, port)
        self.signals = ServerSignals()

    def listen(self):
        self.sock.listen(5)
        while True:
            self.log.debug('Listening...')
            self.client, address = self.sock.accept()
            self.log.debug('accepted')
            self.client.setblocking(True)
            self.listenToClient()
            self.client.shutdown(0)
            self.client.close()
            self.client = None
            self.int_seq = 0

    def processMessage(self, message):
        #get payload
        payload = message['payload']
        cmd = payload['cmd']
        #process different requests
        self.log.debug('CMD = {}'.format(cmd))
        if cmd == S.SERVER_STORE or cmd == S.SERVER_LOAD:
            res_type, res_data, res_cmd = self.database_op(cmd, payload)
            self.sendMessage(res_cmd, res_type, res_data)
            self.int_seq = self.int_seq + 1
        else: 
            self.log.debug('Emitting')
            self.signals.sendToServers.emit(payload)
            self.sendMessage(cmd, payload['type'], payload['data'])
            self.int_seq = self.int_seq + 1

if __name__ == '__main__':
    if len(sys.argv) == 2:
        address = sys.argv[1]
    else:
        print("Error, need address!")

    app = QApplication(sys.argv)
    #spin up the four servers
    servers = [Server(address, port) for port in [S.PORTA, S.PORTB, S.PORTC]]
    admin = AdminServer(address, S.PORT_CTRL)
    #connect signals
    for s in servers:
        admin.signals.sendToServers.connect(s.receiveBroadcast)
    #start them
    executor = ThreadPoolExecutor(max_workers = 4)
    #add admin to servers list to make our lives easy
    servers.append(admin)
    server_futures = [executor.submit(s.listen) for s in servers]
    app.exec_()
    executor.shutdown(True)
    exit(0)
