#!/usr/bin/env python
import feedparser
import serial
import os
import time
import copy
import sys

####################
#  MAIN
####################
#
#  from the command line type:  python rss_reader.py COM15
#
####################
CNN = "http://rss.cnn.com/rss/cnn_topstories.rss"
FOX = "http://feeds.foxnews.com/foxnews/latest"
CBS = "http://www.cbsnews.com/feeds/rss/main.rss"
ABC = "http://feeds.abcnews.com/abcnews/topstories"
NPR = "http://www.npr.org/rss/rss.php?id=1001"
NINENEWS = "http://www.9news.com/rss/topstories.aspx"

rss_feeds = [CNN, FOX, CBS, ABC, NPR, NINENEWS]
source = ["CNN", "FOX", "CBS", "ABC", "NPR", "9NEWS"]

com_port = sys.argv[1]  # COM port for the Arduino

ser = serial.Serial(com_port)
ser.baudrate = 9600
ser.timeout = 0.1   #0.1 second timeout for receives

rss_index = 0
for rss_feed in rss_feeds:
    d = feedparser.parse(rss_feed)
    index = 0
    if (len(d['entries']) < 10):
        num_stories = len(d['entries'])
    else:
        num_stories = 10
    while (index < num_stories):
        print source[rss_index] + ": " + d['items'][index].title
        output = source[rss_index] + ": " + d['items'][index].title
        out_index = 0
        for a_byte in output:
            out_index = out_index + 1
            if out_index == len(output):
               ser.write(chr(ord(a_byte))) 
               print "ENDLINE"
               time.sleep(3)
            else:
               ser.write(chr(ord(a_byte)))
        index = index + 1
    rss_index = rss_index + 1

    
