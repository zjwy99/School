# Server program
# Receives requests and sends back a response for each request
# NEED TO PRINT LOG FILE FOR EACH PORT

from pymongo import MongoClient
import pymongo
import threading
import socket
import sys
import json
import hashlib
import numpy as np
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

def server_recv_message(connection, recv_message):
    while recv_message[-1] != 33:
        recv_message = recv_message + connection.recv(1)
    return recv_message

def parse_json_normal(recv_message_dict):
    recv_checksum = recv_message_dict["checksum"]
    recv_seq = recv_message_dict["seq_num"]
    recv_jpayload = recv_message_dict["payload"]
    recv_payload_dict = recv_jpayload
    recv_operation = recv_payload_dict["cmd"]
    recv_data_type = recv_payload_dict["type"]
    recv_data = recv_payload_dict["data"]
    return recv_message_dict, recv_checksum, recv_seq, recv_jpayload, recv_payload_dict, recv_operation, recv_data_type, recv_data

def parse_json_debug(recv_message_dict):
    recv_msg = recv_message_dict["DEBUG"]
    return recv_msg

def check_seq(log, listen_port, int_seq, recv_seq, miss_count):
    if int_seq == recv_seq:
        #dont need
        log.info('seq number is valid')
        drop = False
    else:
        miss_count = miss_count + abs(int_seq - recv_seq)
        int_seq = recv_seq
        log.info('seq number is invalid')
        drop = True
    return miss_count, drop

def calc_checksum(log, listen_port, theobject, recv_checksum):
    calc_recv_checksum = int(sdbm(theobject))

    if recv_checksum == calc_recv_checksum:
        data_valid = True
        log.info('payload is valid...')
    else:
        data_valid = False
        log.info('payload is invalid...')

    return data_valid

def server_send_message(log, listen_port, connection, cmd, mes_type, mes_data, int_seq):
    # Form payload
    payload = {"cmd":cmd, "type":mes_type, "data":mes_data}
    jpayload = json.dumps(payload)
    # Calculate checksum
    checksum = int(sdbm(jpayload))
    # Form the message
    message = {"payload":payload, "checksum":checksum, "seq_num":int_seq}
    jmessage = '@' + json.dumps(message) + '!'
    # Sends message
    connection.sendall(jmessage.encode())
    log.debug('sent {}'.format(jmessage.encode()))
    # Increment seq num
    int_seq = int_seq + 1
    return int_seq

def database_op(log, listen_port, collection, recv_operation, recv_payload_dict, recv_data_type, recv_data):
    try:
        if recv_operation == 0x10: # store
            insert_result = collection.replace_one({'type':recv_payload_dict['type']},recv_payload_dict,upsert=True)
            mes_type = recv_data_type
            mes_data = recv_data
            opres = 1
            log.debug("stored to database: {}".format(insert_result))
        elif recv_operation == 0x11: # load
            find_result = collection.find_one({"type":recv_data_type})
            if find_result == None:
                opres = -1
                mes_type = 0
                mes_data = 0
                log.debug("error! not found in database...")
            else:
                mes_type = find_result["type"]
                mes_data = find_result["data"]
                opres = 2
                log.debug("found in database: {}".format(find_result))
        elif recv_operation == 0x01:
            opres = mes_data = recv_payload_dict["cmd"]
            mes_type = recv_payload_dict["type"]
            mes_data = recv_payload_dict["data"]
        else:
            log.debug("error! invalid command...")
            opres = -2
            mes_type = 0
            mes_data = 0

        return mes_type, mes_data, opres
    except pymongo.errors.PyMongoError:
        log.debug("error! operation '{}' failed...".format(recv_operation))
        opres = -3
        mes_type = "none"
        mes_data = "none"
        return mes_type, mes_data, opres

def test_cases(log, collection, listen_port, connection, recv_message, int_seq, miss_count, case):
    if case == 1: # sends good message
        int_seq = server_send_message(log, listen_port, connection, S.TEST, S.UINT32, 1, int_seq)
        recv_message = connection.recv(1)
        int_seq, miss_count = run_template(log, collection, listen_port, connection, recv_message, int_seq, miss_count)

    elif case == 2: # receives good message
        recv_message = connection.recv(1)
        int_seq, miss_count = run_template(log,collection, listen_port, connection, recv_message, int_seq, miss_count)
        int_seq = server_send_message(log,listen_port, connection, S.TEST, S.UINT32, 2, int_seq)

    elif case == 3: # sends bad checksum
        # Form payload
        payload = {"cmd":S.TEST, "type":S.UINT32, "data":3}
        jpayload = json.dumps(payload)
        # Calculate checksum
        checksum = int(sdbm(jpayload))
        # Form another payload
        payload = {"cmd":S.TEST, "type":S.UINT32, "data":4}
        jpayload = json.dumps(payload)
        # Form the message
        message = {"payload":payload, "checksum":checksum, "seq_num":int_seq}
        jmessage = '@' + json.dumps(message) + '!'
        # Sends message
        connection.sendall(jmessage.encode())
        log.debug('sent {}'.format(jmessage.encode()))
        # Increment seq num
        int_seq = int_seq + 1
        recv_message = connection.recv(1)
        int_seq, miss_count = run_template(log,collection, listen_port, connection, recv_message, int_seq, miss_count)

    elif case == 4: # receives bad checksum
        recv_message = connection.recv(1)
        int_seq, miss_count = run_template(log,collection, listen_port, connection, recv_message, int_seq, miss_count)
        int_seq = server_send_message(log,listen_port, connection, S.TEST, S.UINT32, 4, int_seq)

    elif case == 5: # sends bad seq num
        int_seq = server_send_message(log,listen_port, connection, S.TEST, S.UINT32, 5, int_seq+1)
        recv_message = connection.recv(1)
        int_seq, miss_count = run_template(log,collection, listen_port, connection, recv_message, int_seq, miss_count)

    elif case == 6: # receives bad seq num
        recv_message = connection.recv(1)
        int_seq, miss_count = run_template(log,collection, listen_port, connection, recv_message, int_seq, miss_count)
        int_seq = server_send_message(log,listen_port, connection, S.TEST, S.UINT32, 6, int_seq)

    return int_seq, miss_count

# Parses incomming messages
def run_template(log, collection, listen_port, connection, recv_message, int_seq, miss_count):
    # Receive messages until '!' is found
    recv_message = server_recv_message(connection, recv_message)

    log.debug('received {}'.format(recv_message))

    # parse JSON
    recv_message_dict = json.loads(recv_message[1:-1])

    # Normal Messages
    if "payload" in recv_message_dict:
        recv_message_dict, recv_checksum, recv_seq, recv_jpayload, recv_payload_dict, recv_operation, recv_data_type, recv_data = parse_json_normal(recv_message_dict)

        # Check sequence number
        miss_count, drop = check_seq(log, listen_port, int_seq, recv_seq, miss_count)

        # Increment sequence number
        int_seq = int_seq + 1

        # Calculates checksum
        data_valid = calc_checksum(log, listen_port, json.dumps(recv_jpayload), recv_checksum)

        if data_valid == True and (not drop or recv_operation == S.SERVER_GET_CONFIG):
            if recv_operation == S.SERVER_GET_CONFIG: # Ask for config  values
                # Loads from database and sends
                mes_type, mes_data, opres = database_op(log, listen_port, collection, S.SERVER_LOAD, recv_message_dict, "enable_testing", 0x0)
                int_seq = server_send_message(log, listen_port, connection, S.PIC_CONFIG_ENABLE_TESTING, S.UINT32, mes_data, int_seq)

                # Sends test case number if testing is enabled
                if mes_data == 1:
                    # Loads from database and sends
                    mes_type, mes_data, opres = database_op(log, listen_port, collection, S.SERVER_LOAD, recv_message_dict, "test#", 0x0)
                    int_seq = server_send_message(log, listen_port, connection, S.PIC_CONFIG_TEST_NUMBER, S.UINT32, mes_data, int_seq)
                    # Do test case
                    int_seq, miss_count = test_cases(log, collection, listen_port, connection, recv_message, int_seq, miss_count, mes_data)
            elif recv_operation == S.SERVER_LOAD:
                # Load/Store from/to database and sends
                mes_type, mes_data, opres = database_op(log, listen_port, collection, recv_operation, recv_payload_dict, recv_data_type, recv_data)
                int_seq = server_send_message(log, listen_port, connection, opres, mes_type, mes_data, int_seq)
            elif recv_operation == S.SERVER_STORE:
                # Load/Store from/to database and sends
                mes_type, mes_data, opres = database_op(log, listen_port, collection, recv_operation, recv_payload_dict, recv_data_type, recv_data)
                int_seq = server_send_message(log, listen_port, connection, opres, mes_type, mes_data, int_seq)
            elif recv_operation == S.PIC_MESSAGE_RATE:
                # Output pic message rate to the rate file
                log.info('Rate: {}'.format(recv_data))
        else:
            # Sends the response
            int_seq = server_send_message(log, listen_port, connection, -3, 0, 0, int_seq)
    # Hanlde Debugging Messages (warnings, ect)
    elif "DEBUG" in recv_message_dict:
        recv_dmsg = parse_json_debug(recv_message_dict)
        # now handle the message
        log.info(recv_dmsg) 

    return int_seq, miss_count

class Server:
    def __init__(self, ip=None):
        if ip == None:
            self.server_ip = socket.gethostname()
        else:
            self.server_ip = ip
        self.print_lock = threading.Lock()
        print("server ip: {}".format(socket.gethostbyname(self.server_ip)))

    def bind_sock(self, log, sock, listen_port):
        sock.bind((self.server_ip, listen_port))
        sock.listen(5)
        log.debug('waiting for a connection..')

    def routine(self, listen_port):
        # Initialize variables
        int_seq = 0
        miss_count = 0
        log = logging.getLogger(str(listen_port))
        log.setLevel(logging.DEBUG)
        fmt = logging.Formatter('%(levelname)s:%(name)s:%(message)s')
        debugHandler = logging.StreamHandler()
        debugHandler.setLevel(logging.DEBUG)
        debugHandler.setFormatter(fmt)
        infoHandler = logging.FileHandler('server_{}_.log'.format(listen_port))
        infoHandler.setLevel(logging.INFO)
        infoHandler.setFormatter(fmt)
        log.addHandler(debugHandler)
        log.addHandler(infoHandler)

        # Connects to the database
        client = MongoClient()
        db = client.embedded_database
        collection = db.milestone2

        # Creates a socket and binds it to a port
        sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.bind_sock(log, sock, listen_port)
        # Infinitely connect to incomming connections (allows easy disconnect/connect)
        while True:
            (connection, client_address) = sock.accept()
            log.info('connection from {}'.format(client_address))
            #connection.settimeout(1)
            connection.setblocking(True)

            flag = True
            while flag:
                try:
                    # Get first message
                    recv_message = connection.recv(1)
                    if recv_message == '' :
                        flag = False

                    # Run template routine
                    if recv_message == S.BEGIN_MSG_DELIM:
                        self.print_lock.acquire(blocking = True)

                        int_seq, miss_count = run_template(log, collection, listen_port, connection, recv_message, int_seq, miss_count)

                        self.print_lock.release()
                    #else ignore incomming
                except ConnectionAbortedError:
                    log.debug('Discon 1')
                    flag = False
                except socket.error as e:
                    log.debug('Discon 2')
                    flag = False
                except socket.timeout as e:
                    log.debug('Discon 3')
                    flag = False
            log.info('disconnected from {}'.format(client_address))
            connection.close()

    def start_threads(self):
        t1 = threading.Thread(target = self.routine, args = (S.PORTA,))
        t2 = threading.Thread(target = self.routine, args = (S.PORTB,))
        t3 = threading.Thread(target = self.routine, args = (S.PORTC,))
        t4 = threading.Thread(target = self.routine, args = (S.PORT_CTRL,))
        t1.start()
        t2.start()
        t3.start()
        t4.start()

try:
    if len(sys.argv) == 2:
        s = Server(sys.argv[1])
    else:
        s = Server()
    s.start_threads()
except KeyboardInterrupt:
    exit()
