#!/usr/local/bin/python3

import pickle
import asyncio
from aiocoap import *
import time
import sys

if (len(sys.argv) != 2):
    print("usage: minecraft.py <ip address>")
    sys.exit(2)

async def main ():
    protocol = await Context.create_client_context()

    while (1):
        time.sleep(1)
        getRequest = Message (code=GET, uri='coap://' + sys.argv[1] + '/location')

        try:
            response = await protocol.request(getRequest).response
        except Exception as e:
            print ('Failure.  Exception: {}'.format(e))
        else:
            r = pickle.loads(response.payload)
            print('location at {}'.format(r))
            newLoc = (0,0,1+r[2])
            postPayload = pickle.dumps(newLoc)
            postRequest = Message (code=POST, payload=postPayload)
            postRequest.opt.uri_host = sys.argv[1]
            postRequest.opt.uri_path = ('location',)
            try:
                postresponse = await protocol.request(postRequest).response
            except:
                print ('Failed to POST {}'.format (newLoc))
            else:
                print ('SUCCESS: {}'.format(pickle.loads(postresponse.payload)))

if __name__ == "__main__":
    asyncio.get_event_loop().run_until_complete(main())
