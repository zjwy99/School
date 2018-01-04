__name__ = "twitteraccess"
__author__ = "sarah.kharimah"

import tweepy


"""
Authenticate OAuth authentication and get a tweepy API handle.

:param: None
:return: tweepy.API handle to access twitter API if successfully authenticated; None otherwise

"""


def get_twitter_api_handle():

    consumer_key = 'N2oJxmLqgiVmrMpTwpkPwbO25'
    consumer_secret = 'l1U4FhXWCtzCdzP0iYZri3KVWH5mm6xXNxHbzFV2Cz2eK2pQx8'
    access_token = '830957663366287365-g4fZE6kWMjqVv6PkBkBuPzRXpWtOhhO'
    access_token_secret = 'E9nhhLnP75Pc7dC3bODyvrNomFcONHQj05Tciy2HNN6rB'

    try:
        auth = tweepy.OAuthHandler(consumer_key, consumer_secret)
        auth.set_access_token(access_token, access_token_secret)
        api = tweepy.API(auth)
        return api
    except Exception as err:
        print(err)
        print('Unable to generate a Twitter API handle')
        return None


"""
Read the most recent tweet on the public timeline.
Print the newest status message on public Twitter timeline to the console.

:param: api_handle: (tweepy.API) tweepy API handle to access Twitter API
:return: (String) the most recent tweet if successful; None otherwise

"""


def get_most_recent_tweet(api_handle):
    recent_tweet = None
    try:
        public_tweets = api_handle.user_timeline()
        for tweet in public_tweets:
            recent_tweet = tweet.text
        print(recent_tweet)
        return recent_tweet
    except Exception as err:
        print(err)
        print('Unable to read the most recent tweet')
        return None


"""
Post a new status message to a public Twitter timeline.
Print the newest status message on public Twitter timeline to the console.

:param: api_handle: (tweepy.API) tweepy API handle to access Twitter API
:param: new_status_message: (String) status message to send to the timeline
:return: True if successfully post a new message to Twitter timeline; False otherwise

"""


def send_tweet_to_timeline(api_handle, new_status_message):

    try:
        api_handle.update_status(new_status_message)
        print('Tweeted successfully')
        return True
    except Exception as err:
        print(err)
        print('Unable to update status message')
        return False

