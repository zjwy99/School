import requests, json

zip = '24060'
payload = {'zip':zip,'cnt':'15','APPID':'a336a6c918103af9bd7045bf9304caa3'}
r = requests.get('http://api.openweathermap.org/data/2.5/forecast/daily?us', params=payload)

response = r.json()
#print(json.dumps(response,skipkeys=True,indent=4))
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

        if num['dt'] == dt_string:
            r_dump = json.dumps(num['clouds'])
            r_load = json.loads(r_dump)
            return r_dump


print(dateTimeCloudiness(1491670800, r_load2))
