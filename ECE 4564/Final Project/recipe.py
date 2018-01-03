from pymongo import MongoClient
import pika
import sys
import json
import getopt
import pickle
import json
import requests

class Recipe(object):

	def recipe(self, ch, method, properties, body):
		print('Recipe')
		temp = json.loads(pickle.loads(body))
		id = temp['id']
		ingredients = ''
		price = ''
		if (id == '1010'):
			print('apple')
			ingredients = "apple"
			price = 1
		if (id == '1009'):
			print('orange')
			ingredients = "orange"
			price = 100
		if (id == '1012'):
			print('carrot')
			ingredients = "carrot"
			price = 1000000
		number = '1'
		self.db.inventory.update({"id":id}, {"$inc":{"number":-1}})

		response = requests.get("https://spoonacular-recipe-food-nutrition-v1.p.mashape.com/recipes/findByIngredients?",
		    headers={
			"X-Mashape-Key": "1nMwLi8qDomshJNqUsfRJSDTcZHCp1ZopPPjsnaY7yCIdfAnVX",
			"Accept": "application/json"
		    },
		    params={
			'ingredients': ingredients,
			'number': number,
			'fillIngredients': 'true',
			'limitLicense': 'true',
			'ranking': '1'
		    }
		    )
		print(response.json())
		data = (price, response.json())
		ch.basic_publish(exchange='', routing_key=properties.reply_to,
				 properties=pika.BasicProperties(correlation_id=properties.correlation_id),
				 body=pickle.dumps(json.dumps(data)))

	def callback (self, ch, method, properties, body):
		mess = pickle.loads (body)
		obj = json.loads(mess)
		if (obj['type'] == "recipe"):
		    self.recipe(ch, method, properties, body)
		else:
		    print ('Not Valid')

	def __init__(self):
		self.client = MongoClient('localhost', 27017)
		self.db = self.client.inventory_db
		self.db = MongoClient().inventory_db
		apples = {"item":"Apple", "id":1010, "number":5, "price":1}
		oranges = {"item":"Orange", "id":1009, "number":100, "price":100}
		carrots = {"item":"Carrot", "id":1012, "number":1000000, "price":1000000}

		self.db.inventory.insert(apples)
		self.db.inventory.insert(oranges)
		self.db.inventory.insert(carrots)

		vhost = '/'
		mbservip = sys.argv[1]
		creds = pika.PlainCredentials('guest', 'guest')
		params = pika.ConnectionParameters(virtual_host=vhost, credentials=creds, host=mbservip)
		self.connection = pika.BlockingConnection(params)
		self.channel = self.connection.channel()
		#        self.channel.exchange_declare (exchange = 'smartgroceries', type='direct')
		result = self.channel.queue_declare(queue='recipe', exclusive=True)
		self.qname = result.method.queue

		#        self.channel.queue_bind (exchange='smartgroceries', queue=qname, routing_keys='accounts')

		self.channel.basic_consume(self.callback, queue=self.qname, no_ack=True)
		self.channel.start_consuming()

q = Recipe()
