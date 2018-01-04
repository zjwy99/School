#!/usr/bin/python3

import pika
import sys

creds = pika.PlainCredentials ('cooperra', 'cooperrapassword')
params = pika.ConnectionParameters (virtual_host='netappTeam01', credentials=creds, host='localhost')
connection = pika.BlockingConnection (params)
channel = connection.channel()
channel.exchange_declare (exchange='pi_utilization', type='direct')

message = "Hello World"

channel.basic_publish (exchange='pi_utilization', routing_key='host1', body=message)
connection.close()
