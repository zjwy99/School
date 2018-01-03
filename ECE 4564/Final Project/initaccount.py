import pika
import sys
import json
import pickle
import json
import uuid

class Client (object):

    def __init__(self):
        self.connection = pika.BlockingConnection(pika.ConnectionParameters(host='localhost'))

        self.channel = self.connection.channel()

        result = self.channel.queue_declare(exclusive=True)
        self.callback_queue = result.method.queue

        self.channel.basic_consume(self.on_response, no_ack=True,
                                   queue=self.callback_queue)

    def on_response(self, ch, method, props, body):
        if self.corr_id == props.correlation_id:
            self.response = body

    def call(self, n):
        tojson = json.dumps(n)
        tosend = pickle.dumps(tojson)
        self.response = None
        self.corr_id = str(uuid.uuid4())
        if (n["type"] == "register" or n["type"] == "pay" or n["type"] == "login" or n["type"] == "checkout"):
            self.channel.basic_publish(exchange='',
                                   routing_key='accounts',
                                   properties=pika.BasicProperties(
                                         reply_to = self.callback_queue,
                                         correlation_id = self.corr_id,
                                         ),
                                   body=tosend)
        else:
            self.channel.basic_publish(exchange='',
                                   routing_key='recipe',
                                   properties=pika.BasicProperties(
                                         reply_to = self.callback_queue,
                                         correlation_id = self.corr_id,
                                         ),
                                   body=tosend)
            
        while self.response is None:
            self.connection.process_data_events()
        return pickle.loads(self.response)

myclient = Client()
response = myclient.call({"type" : "register", "email" : "netappsteam01@vt.edu", "password" : "finalproject"})
print(" [.] Got %r" % response)
response = myclient.call({"type" : "login", "email" : "netappsteam01@vt.edu", "password" : "finlproject"})
print(" [.] Got %r" % response)
