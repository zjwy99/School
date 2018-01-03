#!flask/bin/python
import pika
import sys
import json
import pickle
import json
import uuid
from flask import Flask
from flask import request
from flask_uuid import FlaskUUID

app = Flask(__name__)
flask_uuid = FlaskUUID()
flask_uuid.init_app(app)

@app.route('/')
def index():
    return "Shopping with Big Brother"

@app.route('/get_login', methods=['GET'])
def user_login():
    username = request.args.get('username')
    password = request.args.get('password')
    print(username)
    print(password)
    myclient = Client()
    toresponse = myclient.call({"type" : "login", "email" : username, "password" : password})
    response = json.loads(toresponse)
    print(response['reply'])
    return response['reply']
    

@app.route('/get_recipe', methods=['GET'])
def item_pickup():
    username = request.args.get('username')
    password = request.args.get('password')
    item = request.args.get('minor')
    myclient = Client()
    recipe_response = json.loads(myclient.call({"type" : "recipe", "id" : item}))
    #cost = 100
    cost = recipe_response[0]
    print(cost)
    recipe = recipe_response[1]
    recipe = recipe[0]
    print(recipe)                              
    tototal = myclient.call({"type" : "pay", "email" : username, "password" : password, "price" : cost})
    total = json.loads(tototal)
    print(total['reply'])
    print(recipe['title'])
    print(recipe['image'])
    #return json.dumps({"pay_response" : total['reply'], "title" : 'Cranberry Orange Relish', "image" : 'https://spoonacular.com/recipeImages/Cranberry-Orange-Relish-640400.jpg' })
    return json.dumps({"pay_response" : total['reply'], "title" : recipe['title'], "image" : recipe['image'] })

@app.route('/get_pay_request', methods=['GET'])
def checkout():
    username = request.args.get('username')
    password = request.args.get('password')
    myclient = Client()
    toresponse = myclient.call({"type" : "checkout", "email" : username, "password" : password})
    response = json.loads(toresponse)
    print(response['reply'])
    return str(response['reply'])


class Client (object):

    def __init__(self):
        self.connection = pika.BlockingConnection(pika.ConnectionParameters(host='172.29.103.173'))

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
        if (n["type"] == "login" or n["type"] == "pay" or n["type"] == "checkout"):
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

if __name__ == '__main__':
    app.run(host='0.0.0.0')
