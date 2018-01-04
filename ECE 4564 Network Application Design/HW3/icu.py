#!/usr/bin/python3

import sys
import requests
import getopt
import json
import subprocess
from datetime import datetime
from datetime import date
from datetime import timedelta
import time
from twilio.rest import TwilioRestClient
#import RPi.GPIO as GPIO
import ephem
import math

def get_tle_results(norad_id):

    username = 'sarahkh@vt.edu'
    password = 'murasakishikibu1900'
    base_url = 'https://www.space-track.org/'
    login_url = base_url + 'ajaxauth/login'

    d = datetime.today()
    d1 = d - timedelta(days=2)
    dstr = d.strftime("%Y-%m-%d")
    d1str = d1.strftime("%Y-%m-%d")

    print('test')

    query = base_url + "/basicspacedata/query/class/tle/NORAD_CAT_ID/" + \
            norad_id + \
            "/EPOCH/" + d1str + "%2000:00:00--" + dstr + "%2000:00:00"

    data = {'identity': username, 'password': password, 'query': query}

    # Makes a POST REST call to space-track.org and return the result as a list of JSON
    try:
        resp = requests.post(login_url, data=data)
    except requests.exceptions.RequestException as err:
        print("POST RESTful call unsuccessful - unable to obtain TLE : " + err)

    tle_data_list = json.loads(resp.text)

    # Prints the TLE_LINE values to the console
    # Note: Some dates will generate more than one JSON TLE information
    # print(tle_data_list[index].get('EPOCH')) to get the specific datetime that the JSON belongs to
    for index in range(len(tle_data_list)):
        print("------------------------- TLE JSON # " + str(index) + " -------------------------")
        print(tle_data_list[index].get('EPOCH'))
        print(tle_data_list[index].get('TLE_LINE0'))
        print(tle_data_list[index].get('TLE_LINE1'))
        print(tle_data_list[index].get('TLE_LINE2'))

    return tle_data_list

def notifyme (sattime):
    while (ephem.Date(datetime.now() + timedelta(days = 5)) < sattime):
        time.sleep(1)

# send a text message to the user
#    accountSID = 'AC1de3fd4a6b571ac161acc2ac3498bdde'
#    authToken = '5ea6af71b2aa12a80c5b18d9dbf1486a'
#    twilioCli = TwilioRestClient (accountSID, authToken)
#    myTwilioNumber = '+17575097597'
#    myCellPhone = '+17572433632'
#    message = twilioCli.messages.create (body='Satellite incoming at {}'.format(sattime), from_=myTwilioNumber, to=myCellPhone)
#    GPIO.setmode(GPIO.BCM)
#    GPIO.setwarnings(False)
#    GPIO.setup(25,GPIO.OUT)
#    GPIO.setup(24,GPIO.OUT)

    while (ephem.Date(datetime.now()) < sattime):
        subprocess.Popen(['/usr/bin/aplay', './beep-01.wav'])
        print("on")
#        GPIO.output(25,GPIO.HIGH)
#        GPIO.output(24,GPIO.HIGH)
#        time.sleep(1)
        print ("off")
#        GPIO.output(25,GPIO.LOW)
#        GPIO.output(24,GPIO.LOW)
#        time.sleep(1)

def main(argv):
    #setup of GPIO for LED use
#    GPIO.setmode(GPIO.BCM)
#    GPIO.setwarnings(False)
#    GPIO.setup(25,GPIO.OUT)
#    GPIO.setup(24,GPIO.OUT)

    zipcode = '24060'
    norad_id = '25544'
    events = []
    tle_line0 = ''
    tle_line1 = ''
    tle_line2 = ''

    if len(sys.argv) < 2:
        print('usage: icu -z zipcode -s norad id')
        sys.exit(2)
    try:
        opts, args = getopt.getopt(argv, "hz:s:")
    except getopt.GetoptError:
        print('icu -z zipcode -s norad id')
        sys.exit(2)
    for opt, arg in opts:
        if opt == '-h':
            print('usage: icu -z zipcode -s norad id')
            sys.exit(0)
        elif opt in "-z":
            zipcode = arg
        elif opt in "-s":
            norad_id = arg

    payload = {'zip':zipcode,'cnt':'15','APPID':'a336a6c918103af9bd7045bf9304caa3'}
    r = requests.get('http://api.openweathermap.org/data/2.5/forecast/daily?us', params=payload)

    response = r.json()
    r_json = json.dumps(response['city'])
    r_load = json.loads(r_json)
    r_coord = json.dumps(r_load['coord'])
    r_coord_load = json.loads(r_coord)

    # the longitude string
    longitude = json.dumps(r_coord_load['lon'])

    # the latitude string
    latitude = json.dumps(r_coord_load['lat'])

    r_json2 = json.dumps(response['list'])
    r_load2 = json.loads(r_json2)

    def dateTimeCloudiness(dt_string, response_list):
        for num in response_list:

            if num['dt'] <= dt_string:
                r_dump = json.dumps(num['clouds'])
        return r_dump

    def getForecast(response_list):
        for num in response_list:
            r_clouds = json.dumps(num['clouds'])
            r_dates =  json.dumps(num['dt'])
            print (str(date.fromtimestamp(int(r_dates))) + ' | ' + str(r_clouds) + '% cloudy')

    tle_results = get_tle_results(norad_id=norad_id)
    for index in range(len(tle_results)):
        tle_line0 = tle_results[index].get('TLE_LINE0')
        tle_line1 = tle_results[index].get('TLE_LINE1')
        tle_line2 = tle_results[index].get('TLE_LINE2')

    print(tle_results)

    satellite = ephem.readtle(str(tle_line0), tle_line1, tle_line2)
    print

    obs = ephem.Observer()
    obs.lat = latitude
    obs.long = longitude
    print('Latitude: ' + str(obs.lat))
    print('Longitude: ' + str(obs.lon))
    print
    getForecast(r_load2)
    print
    p = 0
    sattimes = []
    while p < 5:
        tr, azr, tt, altt, ts, azs = obs.next_pass(satellite)
        unixTime = time.mktime(tr.datetime().timetuple())
        if int(dateTimeCloudiness(int(unixTime), r_load2)) <= 20:
            events.append('{0} | {1:.1f}     | {2:.1f}     | {3}'.format(tr, math.degrees(satellite.alt), math.degrees(satellite.az), (ts - tr) * 60 * 60 * 24))
            p = p+1
            sattimes.append(ephem.Date(tr))
        while tr < ts:
            obs.date = tr
            satellite.compute(obs)
            print ('{0} {1:.1f} {2:.1f}'.format(tr, math.degrees(satellite.alt), math.degrees(satellite.az)))
            tr = ephem.Date(tr + 60.0 * ephem.second)
        print
        obs.date = tr + ephem.minute


    print("Date       Time   | Location | Direction | Duration")
    m = 0
    try:
        for k in range(5):
            print( str(events[k]))
            m = m+1
    except:
        print("There are less than 5 viewable events in the next 15 days")

    if m ==0:
        print("There are no viewable events; exiting...")
        sys.exit(1)

    for k in range(m):
        notifyme (sattimes[k])
        print(events[k])
        print('Message sent')

if __name__ == "__main__":
    main(sys.argv[1:])
