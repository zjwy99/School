import tweepy
import json
import twitteraccess

"""
Create class MyStreamListener inheriting from StreamListener to listen to stream and saves the resulting tweet to a text file.

:param: None
:return: None

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


api = twitteraccess.get_twitter_api_handle()
myStreamListener = MyStreamListener()
myStream = tweepy.Stream(auth=api.auth, listener=myStreamListener)

result = myStream.filter(track=['network'])

question_tweet = get_question_tweet_from_file()
print("question_tweet : " + question_tweet)
twitteraccess.send_tweet_to_timeline(api, "question_tweet : " + question_tweet)

myStream.disconnect()
