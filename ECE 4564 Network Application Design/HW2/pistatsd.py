#!/usr/bin/python3

from __future__ import print_function
from time import sleep
import json
import pika
import sys
import getopt


def main(argv):
    message_broker = ''
    virtual_host = ''
    login_and_password = ''
    login = ''
    password = ''
    routing_key = ''

    #collects argument data to connect to message broker
    if len(sys.argv) < 2:
        print('usage: pistatsd –b message broker [–p virtual host] [–c login:password] –k routing key')
        sys.exit(2)
    try:
        opts, args = getopt.getopt(argv, "hb:p:c:k:")
    except getopt.GetoptError:
        print('pistatsd –b message broker [–p virtual host] [–c login:password] –k routing key')
        sys.exit(2)
    for opt, arg in opts:
        if opt == '-h':
            print('usage: pistatsd –b message broker [–p virtual host] [–c login:password] –k routing key')
            sys.exit(0)
        elif opt in "-b":
            message_broker = arg
        elif opt in "-p":
            virtual_host = arg
            print("vhost: {}".format(virtual_host))
        elif opt in "-c":
            login_and_password = arg
            login,password = login_and_password.split(":")            
            print("creds: {}".format(login_and_password))
        elif opt in "-k":
            routing_key = arg
            print(routing_key)
    if login_and_password == '':
        login = 'guest'
        password = 'guest'

    #connects to message broker
    try:
        creds = pika.PlainCredentials (login, password)
        params = pika.ConnectionParameters (virtual_host=virtual_host, credentials=creds, host=message_broker)
        connection = pika.BlockingConnection (params)
        channel = connection.channel()
        channel.exchange_declare (exchange='pi_utilization', type='direct')
    except:
        print ("Failed to connect to RabbitMQ server. Please try again.")
        sys.exit(3)

    last_idle = last_total = 0

    #acquires usage data to be used in usage calculations
    def get_network_rx_tx_bytes():
        network_bytes_list = []

        receive_eth0_bytes = transmit_eth0_bytes = 0
        receive_lo_bytes = transmit_lo_bytes = 0
        receive_wlan0_bytes = transmit_wlan0_bytes = 0

        with open('/proc/net/dev') as d:
            for line in d:
                if "wlan0" in line:
                    line_list = line.split()
                    receive_wlan0_bytes = line_list[1]
                    transmit_wlan0_bytes = line_list[9]
                if "lo" in line:
                    line_list = line.split()
                    receive_lo_bytes = line_list[1]
                    transmit_lo_bytes = line_list[9]
                if "eth0" in line:
                    line_list = line.split()
                    receive_eth0_bytes = line_list[1]
                    transmit_eth0_bytes = line_list[9]
        d.close()

        network_bytes_list.append(receive_wlan0_bytes)
        network_bytes_list.append(transmit_wlan0_bytes)
        network_bytes_list.append(receive_lo_bytes)
        network_bytes_list.append(transmit_lo_bytes)
        network_bytes_list.append(receive_eth0_bytes)
        network_bytes_list.append(transmit_eth0_bytes)

        return network_bytes_list


    while True:

        with open('/proc/stat') as f:
            fields = [float(column) for column in f.readline().strip().split()[1:]]
        idle, total = fields[3], sum(fields)
        idle_delta, total_delta = idle - last_idle, total - last_total
        last_idle, last_total = idle, total
        utilisation = (1.0 - idle_delta / total_delta)

        network_bytes_list = get_network_rx_tx_bytes()

        sleep(1)

        network_bytes_list_prev = network_bytes_list
        network_bytes_list = get_network_rx_tx_bytes()

        #calculates usage values
        wlan0_rx_throughput = (int(network_bytes_list[0]) - int(network_bytes_list_prev[0])) / total_delta
        wlan0_tx_throughput = (int(network_bytes_list[1]) - int(network_bytes_list_prev[1])) / total_delta
        lo_rx_throughput = (int(network_bytes_list[2]) - int(network_bytes_list_prev[2])) / total_delta
        lo_tx_throughput = (int(network_bytes_list[3]) - int(network_bytes_list_prev[3])) / total_delta
        eth0_rx_throughput = (int(network_bytes_list[4]) - int(network_bytes_list_prev[4])) / total_delta
        eth0_tx_throughput = (int(network_bytes_list[5]) - int(network_bytes_list_prev[5])) / total_delta

        #changes the data into JSON format
        data_dict = {"net": {"lo": {"rx": lo_rx_throughput, "tx": lo_tx_throughput},
                             "wlan0": {"rx": wlan0_rx_throughput, "tx": wlan0_tx_throughput},
                             "eth0": {"rx": eth0_rx_throughput, "tx": eth0_tx_throughput}},
                     "cpu": utilisation}

        # Create JSON object and prints it
        json_data = json.dumps(data_dict, sort_keys=True, indent=4, separators=(',', ':'), ensure_ascii=False)
        print(json_data)

        #sends the data to the message broker
        channel.basic_publish (exchange='pi_utilization', routing_key=routing_key, body=json_data)

    connection.close()

if __name__ == "__main__":
    main(sys.argv[1:])
