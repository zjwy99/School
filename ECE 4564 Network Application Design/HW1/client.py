#!/usr/bin/python3

import hashlib
import sys
import re
import pickle
import socket
import tweepy
import twitteraccess
import json
import random

host = '172.29.62.176'
port = 54321
backlog = 5
size = 1024

def getmd5 (message):
    m = hashlib.md5()
    m.update(message.encode('utf-8'))
    return m.hexdigest()

def makemessage (message):
    return (message, getmd5(message))

"""
Create class MyStreamListener inheriting from StreamListener to listen to stream and saves the resulting tweet to a text file.

"""


class MyStreamListener(tweepy.StreamListener):

    def on_status(self, status):
        print("Twitter status: {}".format(status.text))

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

while (1):
    api = twitteraccess.get_twitter_api_handle()
    myStreamListener = MyStreamListener()
    myStream = tweepy.Stream(auth=api.auth, listener=myStreamListener)

    result = myStream.filter(track=['@netappTeam01_'])

    question_tweet = get_question_tweet_from_file()         # question_tweet contains the retrieved tweet
    print("question tweet is this : " + question_tweet)     # prints retrieved tweet

    msg = question_tweet.split('_')

    print("Caught tweet: {}".format(msg))


    """
    End of Twitter Streaming API
    """

    fields = msg
    q = makemessage(fields[1])
    print ("Tuple message: {}".format(q))
    r = pickle.dumps(q)
    print("Pickled tuple: {}".format(r))
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.connect ((host, port))
    s.send(r)
    response = s.recv(1024)
    data = pickle.loads(response)
    print ("Tuple response: {}".format(data))
    s.close()
    if (data[1] != getmd5(data[0])):
        print ("ERROR IN MESSAGE RECEIPT")
        break
    answers = data[0].split('\n')
    for i in answers:
        if len(i) > 125:
            s = i[0:125]
        else:
            s = i
        print ("Tweeting: {}".format(s))
        twitteraccess.send_tweet_to_timeline(api_handle=api, new_status_message='@VTNetApps Team_01 "{}" {}'.format(s, random.randrange(0,1000))) # use this to tweet new message
