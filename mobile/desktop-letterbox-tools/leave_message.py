#!/usr/bin/env python
# -*- coding: utf-8 -*-
import sys
from bluetooth import BluetoothSocket, discover_devices, RFCOMM

MAX_MESSAGE_LENGTH = 112+4

def encapsulate(data):
    tag = '\r\n'
    return "%s%s%s" % (tag, data, tag)

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

message = ''

print "Go and type away (empty line to quit)"
while True:
    data = raw_input()
    if not data:
        break
    message += data

message = encapsulate(message)
if len(message) <= MAX_MESSAGE_LENGTH:
    print "WARNING: maximum length %d!" % MAX_MESSAGE_LENGTH
bytes = sock.send(message)
print "%d bytes sent" % bytes
print "Closing connection to said letterbox"
sock.close()
