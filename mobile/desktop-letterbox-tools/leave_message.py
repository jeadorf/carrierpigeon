#!/usr/bin/env python
# -*- coding: utf-8 -*-
import sys
from bluetooth import BluetoothSocket, discover_devices, RFCOMM

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

print "Go and type away (empty line to quit)"
while True:
    data = raw_input()
    if not data:
        break
    sock.send(data)

print "Closing connection to said letterbox"
sock.close()
