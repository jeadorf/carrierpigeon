#!/usr/bin/env python
# -*- coding: utf-8 -*-
import sys
from bluetooth import BluetoothSocket, discover_devices, RFCOMM

def encap(data):
    tag = '\r\n'
    return tag + data + " " + tag

try:
    #host = [device[0] for device in discover_devices(lookup_names=True)
    #    if device[1] == 'eumel'][0]
    host = '00:12:6F:03:70:95'
except IndexError:
    print "No letterbox found, go figure"
    sys.exit(1)

print "Found letterbox at BT-ID:", host
sock = BluetoothSocket(RFCOMM)
sock.connect((host, 1))
print "Connected to said letterbox"


MAX_MESSAGE_LENGTH = 113
while True:
    print "Go and type away (empty line to quit)"
    data = raw_input()
    if not data:
        break
    data = encap(data)
    if len(data)<MAX_MESSAGE_LENGTH:
        bytes = sock.send(data)
        print "%i bytes sent" %bytes
    else:
        print "ERROR: maximum length %i !" %(MAX_MESSAGE_LENGTH)

print "Closing connection to said letterbox"
sock.close()
