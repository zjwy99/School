#!/usr/bin/python3

import pika
import sys

creds = pika.PlainCredentials ('cooperra', 'cooperrapassword')
params = pika.ConnectionParameters (virtual_host='netappTeam01', credentials=creds, host='localhost')
connection = pika.BlockingConnection (params)
channel = connection.channel()
channel.exchange_declare (exchange='pi_utilization', type='direct')

result = channel.queue_declare (exclusive=True)
qname = result.method.queue

channel.queue_bind (exchange='pi_utilization', queue=qname, routing_key='host1')
channel.queue_bind (exchange='pi_utilization', queue=qname, routing_key='host2')

def callback (ch, method, properties, body):
    print (body)

channel.basic_consume (callback, queue=qname, no_ack=True)
channel.start_consuming ()
