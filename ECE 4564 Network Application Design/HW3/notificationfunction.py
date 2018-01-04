#!/usr/bin/python3

import subprocess
from datetime import datetime
from datetime import timedelta
import time
from twilio.rest import TwilioRestClient
import RPi.GPIO as GPIO

def notifyme (sattime):
    while (datetime.now() + timedelta(minutes = 15) < sattime):
        time.sleep(1)

# send a text message to the user
    accountSID = 'AC28a997e255c9c940015297fd1f9cbbd1'
    authToken = '175088c5d6f2f5e1b44cefb742b6ad3b'
    twilioCli = TwilioRestClient (accountSID, authToken)
    myTwilioNumber = '+13177933263'
    myCellPhone = '+13176168197'
    message = twilioCli.messages.create (body='Satellite incoming at {}'.format(sattime), from_=myTwilioNumber, to=myCellPhone)
    GPIO.setmode(GPIO.BCM)
    GPIO.setwarnings(False)
    GPIO.setup(25,GPIO.OUT)
    GPIO.setup(24,GPIO.OUT)
    GPIO.setup(23,GPIO.OUT)

    while (datetime.now() < sattime):
        subprocess.Popen(['/usr/bin/aplay', './beep-01.wav'])
        print("on")
#        GPIO.output(25,GPIO.HIGH)
#        GPIO.output(24,GPIO.HIGH)
        time.sleep(1)
        print ("off")
#        GPIO.output(25,GPIO.LOW)
#        GPIO.output(24,GPIO.LOW)
        time.sleep(1)

notifyme (datetime.now() + timedelta(minutes = 16))
