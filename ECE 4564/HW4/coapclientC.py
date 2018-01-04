#!/usr/local/bin/python3

import pickle
import asyncio
from aiocoap import *
import time
import sys

if (len(sys.argv) != 2):
    print("usage: minecraft.py <ip address>")
    sys.exit(2)

async def main():
    protocol = await Context.create_client_context()

    while (1):
        time.sleep(5)
        getRequest = Message(code=GET, uri='coap://' + sys.argv[1] + '/location')

        try:
            response = await protocol.request(getRequest).response
        except Exception as e:
            print('Failure.  Exception: {}'.format(e))
        else:
            r = pickle.loads(response.payload)
            print('location at {}'.format(r))
            token_id = r[3]
            x = r[4]
            if token_id == 2:
                newLoc = (r[0], r[1], r[2], token_id, x)
                postPayload = pickle.dumps(newLoc)
                postRequest = Message(code=POST, payload=postPayload)
                postRequest.opt.uri_host = sys.argv[1]
                postRequest.opt.uri_path = ('location',)
                try:
                    postresponse = await protocol.request(postRequest).response
                except:
                    print('Failed to POST {}'.format(newLoc))
                else:
                    resp = pickle.loads(postresponse.payload)
                    print ('SUCCESS: {}'.format(resp))
                    if (resp == "Wall is done"):
                        sys.exit(0)


if __name__ == "__main__":
    asyncio.get_event_loop().run_until_complete(main())
