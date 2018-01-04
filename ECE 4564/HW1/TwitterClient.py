#!/usr/bin/python3

import hashlib
import sys
import re
import pickle
import socket
import tweepy
import twitteraccess
import json

host = '172.31.186.170'
port = 5000
backlog = 5
side = 1024
ready = 1

"""
Create class MyStreamListener inheriting from StreamListener to listen to stream and saves the resulting tweet to a text file.
"""


class MyStreamListener(tweepy.StreamListener):

    def on_status(self, status):
        print(status.text)

    def on_data(self, raw_data):
        data_in_json = json.loads(raw_data)
        file = open('twitter_data.txt', 'w+')
        file.write(data_in_json['text'])
        file.close()
        return False

"""
Read the "twitter_data.txt" where the last tweet is saved and return it as a string.
:param: None
:return: String of the latest tweet.
"""


def get_question_tweet_from_file():
    file = open('twitter_data.txt', 'r')
    twitter_data = file.read()
    file.close()
    return twitter_data

"""
Twitter Streaming API
"""
while(1):
    if ready:
        api = twitteraccess.get_twitter_api_handle()
        myStreamListener = MyStreamListener()
        myStream = tweepy.Stream(auth=api.auth, listener=myStreamListener)

        result = myStream.filter(track=['@netappTeam01'])

        question_tweet = get_question_tweet_from_file()         # question_tweet contains the retrieved tweet
        print("question tweet is this : " + question_tweet)     # prints retrieved tweet

        msg = "here's a question : " + question_tweet
        # twitteraccess.send_tweet_to_timeline(api_handle=api, new_status_message=msg) # use this to tweet new message

        ready = 0

    """
    End of Twitter Streaming API
    """
    """
    if len(sys.argv) != 2:
        print ("Usage: {} <message>".format(sys.argv[0]))
        sys.exit(1)
    """

    def getmd5 (message):
        m = hashlib.md5()
        m.update(message.encode('utf-8'))
        return m.hexdigest()

    def makemessage (message):
        return (message, getmd5(message))

    if re.match('@netapp_team01.*?', question_tweet):
        fields = question_tweet.split(' ',1)
        s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        s.connect ((host, port))
        q = makemessage(fields[1])
        #print ("{}".format(q))
        r = pickle.dumps(q)
        #print("{}".format(r))
        s.send(r)
        response = s.recv(1024)
        data = pickle.loads(response)
        #print (data)
        print (data[0])
        twitteraccess.send_tweet_to_timeline(api_handle=api, new_status_message=data[0])
        ready = 1
        s.close()
