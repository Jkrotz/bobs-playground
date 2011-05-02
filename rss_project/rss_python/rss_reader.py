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
#rss_feeds = [CNN]
source = ["CNN", "FOX", "CBS", "ABC", "NPR", "9NEWS"]
#source = ["CNN"]

com_port = sys.argv[1]  # COM port for the Arduino

ser = serial.Serial(com_port)
ser.baudrate = 9600
ser.timeout = 0.1   #0.1 second timeout for receives


time.sleep(3)
rss_index = 0

while (1 == 1):
    rss_index = 0
    for rss_feed in rss_feeds:
        d = feedparser.parse(rss_feed)
        index = 0
        if (len(d['entries']) < 10):
            num_stories = len(d['entries'])
        else:
            num_stories = 10
        print source[rss_index] + " total num stories: " + str(num_stories)
        while (index < num_stories):
            print "index: " + str(index) + " " + source[rss_index] + ": " + d['items'][index].title
            output = source[rss_index] + ": " + d['items'][index].title
            output_list = output.split(' ')
            # print output_list
            row = 0
            column = 0
            for word in output_list:
                #print word
                #column = 1
                #if (column != 0):
                #    ser.write(chr(0x20))  # space character
                #    column = column + 1
                if (len(word) < (14 - column)):
                    for a_byte in word:
                        ser.write(chr(ord(a_byte)))
                        column = column + 1
                        #print str(a_byte)
                    ser.write(chr(0x20))
                    column = column + 1
                else:
                    ser.write(chr(0xAA))  # send flag to go to the next line
                    #print "NEWLINE"
                    row = row + 1                
                    if (row == 6):
                        time.sleep(3)
                        break
                    column = 0
                    for a_byte in word:
                        ser.write(chr(ord(a_byte)))
                        column = column + 1
                        #print str(a_byte)
                    ser.write(chr(0x20))
                    column = column + 1
            #print "END OF STORY"
            ser.write(chr(0xAB))  # send flag for end of story
            time.sleep(5)
            index = index + 1
        rss_index = rss_index + 1

    
