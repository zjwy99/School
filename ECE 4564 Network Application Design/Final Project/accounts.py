from pymongo import MongoClient
import pika
import sys
import json
import getopt
import pickle
import json

class Accounts(object):
    def register(self, ch, method, properties, body):
        print('Register')
        entry = json.loads(pickle.loads(body))
        if (self.posts.find_one({"email" : entry["email"]}) != None):
            ch.basic_publish(exchange='', routing_key=properties.reply_to,
                                properties=pika.BasicProperties(correlation_id = properties.correlation_id),
                                body = pickle.dumps(json.dumps({"reply" : "Already Registered"})))
        else:
            entry.update({"balance" : 0})
            self.posts.insert_one(entry)
            ch.basic_publish(exchange='', routing_key=properties.reply_to,
                                properties=pika.BasicProperties(correlation_id = properties.correlation_id),
                                body = pickle.dumps(json.dumps({"reply":"Registered"})))

    def login(self, ch, method, properties, body):
        print('Login')
        entry = json.loads(pickle.loads(body))
        qu = self.posts.find_one({"email" : entry["email"]})
        if qu != None:
            if qu["password"] == entry["password"]:
                ch.basic_publish(exchange='', routing_key=properties.reply_to,
                                properties=pika.BasicProperties(correlation_id = properties.correlation_id),
                                body = pickle.dumps(json.dumps({"reply" : ":)"})))
                print ("Success")
            else:
                ch.basic_publish(exchange='', routing_key=properties.reply_to,
                                properties=pika.BasicProperties(correlation_id = properties.correlation_id),
                                body = pickle.dumps(json.dumps({"reply":":("})))
                print ("Failed")
        else:
            ch.basic_publish(exchange='', routing_key=properties.reply_to,
                                properties=pika.BasicProperties(correlation_id = properties.correlation_id),
                                body = pickle.dumps(json.dumps({"reply":":("})))
            print ("Failed")

    def pay(self, ch, method, properties, body):
        entry = json.loads(pickle.loads(body))
        qu = self.posts.find_one({"email" : entry["email"]})
        if qu == None:
            ch.basic_publish(exchange='', routing_key=properties.reply_to,
                                properties=pika.BasicProperties(correlation_id = properties.correlation_id),
                                body = pickle.dumps(json.dumps({"reply" : "No account"})))
        else:
            if qu["password"] != entry["password"]:
                ch.basic_publish(exchange='', routing_key=properties.reply_to,
                                properties=pika.BasicProperties(correlation_id = properties.correlation_id),
                                body = pickle.dumps(json.dumps({"reply" : "No account"})))
            else:
                cur = qu["balance"]
                cur += int(entry["price"])
                self.posts.update_one({"email" : entry["email"]}, {"$set" : {"balance" : cur}})
                print ("Balance: {}".format(cur))
                print ('Pay')
                ch.basic_publish(exchange='', routing_key=properties.reply_to,
                                properties=pika.BasicProperties(correlation_id = properties.correlation_id),
                                body = pickle.dumps(json.dumps({"reply" : cur})))

    def checkout(self, ch, method, properties, body):
        entry = json.loads(pickle.loads(body))
        qu = self.posts.find_one({"email" : entry["email"]})
        if qu == None:
            ch.basic_publish(exchange='', routing_key=properties.reply_to,
                                properties=pika.BasicProperties(correlation_id = properties.correlation_id),
                                body = pickle.dumps(json.dumps({"reply" : "No account"})))
        else:
            if qu["password"] != entry["password"]:
                ch.basic_publish(exchange='', routing_key=properties.reply_to,
                                properties=pika.BasicProperties(correlation_id = properties.correlation_id),
                                body = pickle.dumps(json.dumps({"reply" : "No account"})))
            else:
                cur = qu["balance"]
                self.posts.update_one({"email" : entry["email"]}, {"$set" : {"balance" : 0}})
                print ("Payment: {}".format(cur))
                print ('Checkout')
                ch.basic_publish(exchange='', routing_key=properties.reply_to,
                                properties=pika.BasicProperties(correlation_id = properties.correlation_id),
                                body = pickle.dumps(json.dumps({"reply" : cur})))

    def callback (self, ch, method, properties, body):
        print ("Callback")
        mess = pickle.loads (body)
        obj = json.loads(mess)
        if (obj['type'] == "register"):
            self.register(ch, method, properties, body)
        elif (obj['type'] == "pay"):
            self.pay(ch, method, properties, body)
        elif (obj['type'] == "login"):
            self.login(ch, method, properties, body)
        elif (obj['type'] == "checkout"):
            self.checkout(ch, method, properties, body)
        else:
            print ('Not Valid')
        print ()

    def __init__(self):
        self.client = MongoClient ('localhost', 27017)
        self.db = self.client.accdb
        self.coll = self.db.accounts
        self.posts= self.db.posts

        vhost = '/'
        mbservip = sys.argv[1]
        creds = pika.PlainCredentials ('guest', 'guest')
        params = pika.ConnectionParameters (virtual_host = vhost, credentials = creds, host=mbservip)
        self.connection = pika.BlockingConnection (params)
        self.channel = self.connection.channel()
#        self.channel.exchange_declare (exchange = 'smartgroceries', type='direct')
        result = self.channel.queue_declare (queue='accounts', exclusive=True)
        self.qname = result.method.queue

#        self.channel.queue_bind (exchange='smartgroceries', queue=qname, routing_keys='accounts')

        self.channel.basic_consume (self.callback, queue=self.qname, no_ack=True)
        self.channel.start_consuming()


q = Accounts()
