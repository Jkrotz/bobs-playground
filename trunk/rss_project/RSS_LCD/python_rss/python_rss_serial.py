#!/usr/bin/env python

import ModelFeed
import serial
import os
import time
import copy
#~ import re
#~ import string
import sys
#~ import httplib
#~ import urllib2
#~ from xml.dom import minidom



## the first argument is the name of the feed listed in feedlist.dat
feedname = sys.argv[1]
com_port = sys.argv[2]


def bodyfn(feedname):
    feed = ModelFeed.ModelFeed()
    feedurl = feed.feeddata(feedname)
    body = feed.links(feedurl, feedname)
    return body


ser = serial.Serial(com_port)
ser.baudrate = 9600
ser.timeout = 0.1   #0.1 second timeout for receives

i = 0
feedname_list = ["CNN","9NEWS","CBS","NPR","KMGH"]
while(1 == 1):
    output = ""
    for feedname in feedname_list:
        body = bodyfn(feedname)
        output = output + body
    print output
    print "size of the output: " + str(len(output))


    print ser.portstr
    print ser

    for a_byte in output:
        if a_byte == '\n':
           time.sleep(3)
        else:
           ser.write(chr(ord(a_byte)))
