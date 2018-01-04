#!/usr/bin/python3

from twilio.rest import TwilioRestClient

accountSID = 'AC28a997e255c9c940015297fd1f9cbbd1'
authToken = '175088c5d6f2f5e1b44cefb742b6ad3b'

twilioCli = TwilioRestClient (accountSID, authToken)

myTwilioNumber = '+13177933263'
myCellPhone = '+13176168197'

message = twilioCli.messages.create (body='Attention All Planets of the Solar Federation', from_=myTwilioNumber, to=myCellPhone)
