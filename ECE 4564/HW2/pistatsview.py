import sys
import getopt
import pymongo
import json
import pika
import RPi.GPIO as GPIO

def main(argv):
    #setup of GPIO for LED use
    GPIO.setmode(GPIO.BCM)
    GPIO.setwarnings(False)
    GPIO.setup(25,GPIO.OUT)
    GPIO.setup(24,GPIO.OUT)

    message_broker = ''
    virtual_host = ''
    login_and_password = ''
    login = ''
    password = ''

    #collect argument data for connecting to the message broker
    if len(sys.argv) < 2:
        print('usage: pistatsview –b message broker [–p virtual host] [–c login:password] –k routing key')
        sys.exit(2)
    try:
        opts, args = getopt.getopt(argv, "hb:p:c:k:")
    except getopt.GetoptError:
        print('pistatsview –b message broker [–p virtual host] [–c login:password] –k routing key')
        sys.exit(2)
    for opt, arg in opts:
        if opt == '-h':
            print('usage: pistatsview –b message broker [–p virtual host] [–c login:password] –k routing key')
            sys.exit(0)
        elif opt in "-b":
            message_broker = arg
        elif opt in "-p":
            virtual_host = arg
            print(virtual_host)
        elif opt in "-c":
            login_and_password = arg
            login,password = login_and_password.split(":")            
            print(login_and_password)
        elif opt in "-k":
            routing_key = arg
            print(routing_key)
    if login_and_password == '':
        login = 'guest'
        password = 'guest'
    if virtual_host == '':
        virtual_host = '/'

    #connect to the message broker
    try:
        creds = pika.PlainCredentials (login, password)
        params = pika.ConnectionParameters (virtual_host=virtual_host, credentials=creds, host=message_broker)
        connection = pika.BlockingConnection (params)
        channel = connection.channel()
        channel.exchange_declare (exchange='pi_utilization', type='direct')
    except:
        print ('Failed to connect to server. Please try again.')
        sys.exit(3)

    result = channel.queue_declare (exclusive=True)
    qname = result.method.queue

    channel.queue_bind (exchange='pi_utilization', queue=qname, routing_key='host1')
    channel.queue_bind (exchange='pi_utilization', queue=qname, routing_key='host2')

    #retrieves the data from the message broker
    def callback (ch, method, properties, body):
        stats = json.loads(body.decode('utf-8'))
        print (type(stats))
        dbupdate(stats, method.routing_key)


    def dbupdate (stats, route):

        try:
            # Connects to Mongo, must run the Mongo server before running this file
            client = pymongo.MongoClient("localhost", 27017)
            db = client.test
            db = pymongo.MongoClient().test
        except:
            print ('Failed to connect to Mongo database')
            sys.exit(3)

        #inserts jsons
        db.utilization.insert(stats)

        # Gets current value for CPU
        current_cpu = json.dumps(stats['cpu'])

        # Highest Value for CPU
        cursor = db.utilization.find().sort([('cpu', -1)])
        high_cpu = json.dumps(cursor.next()['cpu'])

        # Lowest Value for CPU
        cursor = db.utilization.find().sort([('cpu', 1)])
        low_cpu = json.dumps(cursor.next()['cpu'])

        # Gets the highest rx value for lo
        cursor = db.utilization.find().sort([('net.lo.rx',-1)])
        high_rx = cursor.next()['net']
        get_lo = json.dumps(high_rx['lo'])
        parse_lo = json.loads(get_lo)
        lo_high_rx = parse_lo['rx']

        # Gets the lowest rx value for lo
        cursor = db.utilization.find().sort([('net.lo.rx', 1)])
        low_rx = cursor.next()['net']
        get_lo = json.dumps(low_rx['lo'])
        parse_lo = json.loads(get_lo)
        lo_low_rx = parse_lo['rx']

        # Gets the highest tx value for lo
        cursor = db.utilization.find().sort([('net.lo.tx', -1)])
        high_tx = cursor.next()['net']
        get_lo = json.dumps(high_tx['lo'])
        parse_lo = json.loads(get_lo)
        lo_high_tx = parse_lo['tx']

        # Gets the lowest tx value for lo
        cursor = db.utilization.find().sort([('net.lo.tx', 1)])
        low_tx = cursor.next()['net']
        get_lo = json.dumps(low_tx['lo'])
        parse_lo = json.loads(get_lo)
        lo_low_tx = parse_lo['tx']

        # Gets the highest rx value for wlan0
        cursor = db.utilization.find().sort([('net.wlan0.rx', -1)])
        high_rx = cursor.next()['net']
        get_wlan0 = json.dumps(high_rx['wlan0'])
        parse_wlan0 = json.loads(get_wlan0)
        wlan0_high_rx = parse_wlan0['rx']

        # Gets the lowest rx value for wlan0
        cursor = db.utilization.find().sort([('net.wlan0.rx', 1)])
        low_rx = cursor.next()['net']
        get_wlan0 = json.dumps(low_rx['wlan0'])
        parse_wlan0 = json.loads(get_wlan0)
        wlan0_low_rx = parse_wlan0['rx']

        # Gets the highest tx value for wlan0
        cursor = db.utilization.find().sort([('net.wlan0.tx', -1)])
        high_tx = cursor.next()['net']
        get_wlan0 = json.dumps(high_tx['wlan0'])
        parse_wlan0 = json.loads(get_wlan0)
        wlan0_high_tx = parse_wlan0['tx']

        # Gets the lowest tx value for wlan0
        cursor = db.utilization.find().sort([('net.wlan0.tx', 1)])
        low_tx = cursor.next()['net']
        get_wlan0 = json.dumps(low_tx['wlan0'])
        parse_wlan0 = json.loads(get_wlan0)
        wlan0_low_tx = parse_wlan0['tx']

        # Gets the highest rx value for eth0
        cursor = db.utilization.find().sort([('net.eth0.rx', -1)])
        high_rx = cursor.next()['net']
        get_eth0 = json.dumps(high_rx['eth0'])
        parse_eth0 = json.loads(get_eth0)
        eth0_high_rx = parse_eth0['rx']

        # Gets the lowest rx value for eth0
        cursor = db.utilization.find().sort([('net.eth0.rx', 1)])
        low_rx = cursor.next()['net']
        get_eth0 = json.dumps(low_rx['eth0'])
        parse_eth0 = json.loads(get_eth0)
        eth0_low_rx = parse_eth0['rx']

        # Gets the highest tx value for eth0
        cursor = db.utilization.find().sort([('net.eth0.tx', -1)])
        high_tx = cursor.next()['net']
        get_eth0 = json.dumps(high_tx['eth0'])
        parse_eth0 = json.loads(get_eth0)
        eth0_high_tx = parse_eth0['tx']

        # Gets the lowest tx value for eth0
        cursor = db.utilization.find().sort([('net.eth0.tx', 1)])
        low_tx = cursor.next()['net']
        get_eth0 = json.dumps(low_tx['eth0'])
        parse_eth0 = json.loads(get_eth0)
        eth0_low_tx = parse_eth0['tx']

        to_json = json.dumps(stats['net'])
        parsenet = json.loads(to_json)

        get_lo = json.dumps(parsenet['lo'])
        parse_lo = json.loads(get_lo)

        get_wlan = json.dumps(parsenet['wlan0'])
        parse_wlan = json.loads(get_wlan)

        get_eth0 = json.dumps(parsenet['eth0'])
        parse_eth0 = json.loads(get_eth0)

        lo_current_tx = parse_lo['tx']
        lo_current_rx = parse_lo['rx']

        wlan0_current_tx = parse_wlan["tx"]
        wlan0_current_rx = parse_wlan["rx"]

        eth0_current_tx = parse_eth0["tx"]
        eth0_current_rx = parse_eth0["rx"]

        # Prints the necessary output messages
        print(route)
        print("cpu: " + str(current_cpu) + " [Hi: " + str(high_cpu) + ", Lo: " + str(low_cpu) + "]")
        print("lo: rx=" + str(lo_current_rx) + " B/s" + " [Hi: " + str(lo_high_rx) + " B/s, Lo: " + str(lo_low_rx) + " B/s], tx=" +
              str(lo_current_tx) + " B/s" + " [Hi: " + str(lo_high_tx) + " B/s, Lo: " + str(lo_low_tx) + " B/s]")
        print("wlan0: rx=" + str(wlan0_current_rx) + " B/s" + " [Hi: " + str(wlan0_high_rx) + " B/s, Lo: " + str(wlan0_low_rx) + " B/s], tx=" +
              str(wlan0_current_tx) + " B/s" + " [Hi: " + str(wlan0_high_tx) + " B/s, Lo: " + str(wlan0_low_tx) + " B/s]")
        print("eth0: rx=" + str(eth0_current_rx) + " B/s" + " [Hi: " + str(eth0_high_rx) + " B/s, Lo: " + str(eth0_low_rx) + " B/s], tx=" +
              str(eth0_current_tx) + " B/s" + " [Hi: " + str(eth0_high_tx) + " B/s, Lo: " + str(eth0_low_tx) + " B/s]")

        # produces the proper color of LED in regards to cpu usage
        if float(current_cpu) < .25:
            GPIO.output(24,GPIO.HIGH)
            GPIO.output(25,GPIO.LOW)
        elif float(current_cpu) >= .25 and float(current_cpu) <= .50:
            GPIO.output(24,GPIO.HIGH)
            GPIO.output(25,GPIO.HIGH)
        else:
            GPIO.output(25,GPIO.HIGH)
            GPIO.output(24,GPIO.LOW)

    #initiates and continues to collect data from the queue
    channel.basic_consume (callback, queue=qname, no_ack=True)
    channel.start_consuming ()

if __name__ == "__main__":
    main(sys.argv[1:])
