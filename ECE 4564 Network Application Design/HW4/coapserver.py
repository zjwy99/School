#!/usr/local/bin/python3

import aiocoap
import asyncio
import aiocoap.resource as resource
import pickle
import sys
import mcpi.minecraft as minecraft
import mcpi.block as block
import RPi.GPIO as GPIO
import time

# Global Variable for Minecraft Connection
mc = minecraft.Minecraft.create()

# Global Variable for determining if row has been completed 
row_complete = 0

def mapblock(blockid):
    if blockid == 0:
        return block.WOOD.id
    elif blockid == 1:
        return block.DIAMOND_BLOCK.id
    elif blockid == 2:
        return block.STONE.id
    else:
        return block.DIRT.id
        
class LocationResource (resource.Resource):
    
    token_id = 0
    x = 0
    def __init__(self):
        super(LocationResource, self).__init__()
        self.content = (0,0,0,0,0)

    async def render_get (self, request):
        temp = tuple(mc.player.getPos())
        self.content = (temp[0], temp[1], temp[2], self.token_id, self.x)
        g = pickle.dumps (self.content)
        return aiocoap.Message(payload=g)

    async def render_post (self, request):
        p = pickle.loads (request.payload)
        print ('POST: {}'.format (p))
        
        if self.token_id >= 2:
            self.token_id = 0
        else:
            self.token_id += 1
        self.x += 1
        
        if (self.token_id == 0):
            GPIO.output(25, GPIO.HIGH)
            GPIO.output(24, GPIO.LOW)
            GPIO.output(23, GPIO.LOW)
        elif (self.token_id == 1):
            GPIO.output(23, GPIO.HIGH)
            GPIO.output(25, GPIO.LOW)
            GPIO.output(24, GPIO.LOW)
        elif (self.token_id == 2):
            GPIO.output(24, GPIO.HIGH)
            GPIO.output(25, GPIO.LOW)
            GPIO.output(23, GPIO.LOW)
        else:
            print ("TOKEN ERROR")
        
        global row_complete
        if (self.x <= 10):
            self.content = (1 + self.content[0],self.content[1], self.content[2], self.token_id, self.x)
            mc.setBlock(self.content[0], self.content[1], self.content[2], mapblock(self.token_id))
            mc.player.setPos(self.content[0], self.content[1], self.content[2])
            mc.postToChat('Block placed at ' + str(self.content[0]) + ' ' + str(self.content[1]) + ' ' + str(self.content[2]))
            print ('Content: {}'.format(self.content))
            payload = pickle.dumps ('POST request received to place block at {}'.format (p))
            return aiocoap.Message (payload=payload)
        elif (row_complete < 1):
            self.x = 1
            row_complete += 1
            self.content = (self.content[0] - 9, 1 + self.content[1], self.content[2], self.token_id, self.content[4])
            mc.setBlock(self.content[0], self.content[1], self.content[2], mapblock(self.token_id))
            mc.player.setPos(self.content[0], self.content[1], self.content[2])
            mc.postToChat('Block placed at ' + str(self.content[0]) + ' ' + str(self.content[1]) + ' ' + str(self.content[2]))
            print ('Content: {}'.format(self.content))
            payload = pickle.dumps ('POST request received to place block at {}'.format (p))
            return aiocoap.Message (payload=payload)
        else:
            print('Content: {}'.format(self.content))
            payload = pickle.dumps ('Wall is done')
            return aiocoap.Message (payload=payload)

def main ():
    GPIO.setmode(GPIO.BCM)
    GPIO.setwarnings(False)
    GPIO.setup(25, GPIO.OUT)
    GPIO.setup(24, GPIO.OUT)
    GPIO.setup(23, GPIO.OUT)
    
    
    root = resource.Site()
    root.add_resource (('location',), LocationResource())

    asyncio.Task (aiocoap.Context.create_server_context(root))
    asyncio.get_event_loop().run_forever()

if __name__ == "__main__":
    main()
