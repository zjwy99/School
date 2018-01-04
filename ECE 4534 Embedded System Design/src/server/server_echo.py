#!/usr/bin/env python3
import socket
import json
from sdbm import *

host = ''
port = 2000
backlog = 5
size = 1024

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.bind((host, port))
s.listen(backlog)

print('server started and listening')

try:
    while True:
        client, address = s.accept()
#        while True:
        data = client.recv(size)
        print('received data: ', data)
        data = {'cmd':0,'type':4,'value':22}
        checksum = sdbm(json.dumps(data))
        data = json.dumps({'checksum':int(checksum),'seq_num':0,'payload':data})
        data = '@' + data + '!'
        data = data.encode('ascii')
        print('sent data: ', data)
        client.send(data)
        #enter into while loop
        while True:
            data = client.recv(size)
            print('received data: ', data)

except KeyboardInterrupt:
    client.close()
    s.shutdown(socket.SHUT_RDWR)
    s.close()

client.close()


