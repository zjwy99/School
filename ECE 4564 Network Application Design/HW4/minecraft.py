import time
import sys
import mcpi.minecraft as minecraft
import mcpi.block as block


if (len(sys.argv) != 2):
    print ('usage: minecraft <ipaddress>')
    sys.exit(2)

host = sys.argv[1]
mc = minecraft.Minecraft.create(host)
temp = tuple(mc.player.getPos())
mc.setBlock(temp[0],temp[1],temp[2])
print(temp)
