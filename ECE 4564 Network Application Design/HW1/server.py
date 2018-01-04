#!/usr/bin/env python

import wolframalpha
import socket
import sys
import hashlib
import pickle

# Variables for server
host = ''
port = 54321
backlog = 5
size = 1024

# Wolfram Alpha set up
app_id = "2AYVQ4-869A86X3LQ"
wolfclient = wolframalpha.Client(app_id)

def getmd5 (message):
    m=hashlib.md5()
    m.update(message.encode('utf-8'))
    return m.hexdigest()

def makemessage (message):
    return (message, getmd5(message))

# Have server listen to clients
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.bind((host, port))
s.listen(backlog)

# Infinite loop
while 1:
    client, address = s.accept()
    sdata = client.recv(size)
    
    data = pickle.loads(sdata)
    try:
        if (data[1] != getmd5(data[0])):
            print ("ERROR IN MESSAGE SENDING")
            raise

        # Every time server gets input, send that input to Wolfram Alpha
        print("Received tuple: {}".format(data))
        print ("Received Question: {}".format(data[0]))
        res = wolfclient.query(data[0])
        answer = next(res.results).text
        mesanswer = makemessage(answer)
    except:
        mesanswer = makemessage("Unable to answer question.  Please try again")
    print("Answer tuple: {}".format(mesanswer))

    if mesanswer:
        sendanswer = pickle.dumps(mesanswer)
        client.send(sendanswer)
    client.close()
