#!/usr/bin/env python

import serial
import os
import time
import copy
import re
import string
import sys
import httplib
import urllib2
from xml.dom import minidom

## CGI Version
#import cgi
#import cgitb; cgitb.enable()
#form = cgi.FieldStorage() # holds data from form
#feedname = form["selection"].value

## PHP Version
#feedname = sys.argv[1]

# feedname = "Spiegel"
# feedname = "GuardianWorld"

def listinfo(type):
    infofile = "feedlist." + type
    datafile = open(infofile, "r")
    line = datafile.readline()

    record = {}

    while line:
        data = string.split(line, ';')
        feedname = data[0]
        address = data[1]
        record[feedname] = address
        line = datafile.readline()

    return record

feedinfo = listinfo("dat")

class ModelFeed:

    def __init__(self):
        self.data = []

    def feeddata (self, feedname):
        feedaddress = feedinfo[feedname]
        return feedaddress

    def links (self, address):
        file_request = urllib2.Request(address)
        file_opener = urllib2.build_opener()
        file_feed = file_opener.open(file_request).read()
        file_xml = minidom.parseString(file_feed)

        item_node = file_xml.getElementsByTagName("item")

        linkdata = ""

        for item in item_node:
            title = item.childNodes[1]
            link = item.childNodes[3]

            ftitle = title.firstChild.data
            flink = link.firstChild.data

            #linkdata = linkdata + "<a href=\"" + flink + "\" target=\"target\">" + ftitle + "</a><br>\n"

            linkdata = linkdata + ftitle + "\n"
            
        return linkdata

    def image (self, feedname):
        image_address = imginfo[feedname]
        return image_address


def inodeValue(doc, nodename):
    dom = minidom.parseString(doc)
    node = dom.getElementsByTagName(nodename)
    norm = node[0].toxml()
    node_no_xml = re.sub('(<title>)|(<\/title>)|(<link>)|(<\/link>)|(<url>)|(</url>)', '', norm)
    value = str(node_no_xml)
    print value
    return value


def dnodeValue(doc, nodename):
    dom = minidom.parseString(doc)
    node = dom.getElementsByTagName(nodename)
    norm = node[0].toxml()
    node_no_xml = re.sub('(<title>)|(<\/title>)|(<link>)|(<\/link>)|(<description>)|(<\/description>)', '', norm)
    value = str(node_no_xml)
    print value
    return value

#~ def bodyfn(feedname):
    #~ feed = ModelFeed()
    #~ feedurl = feed.feeddata(feedname)
    #~ body = feed.links(feedurl)
    #~ return body

    

#~ def main():
    #~ i = 0
    #~ body = bodyfn(feedname)
    #~ output = body
    #~ print output
    #~ print "size of the output: " + str(len(output))


#~ if __name__ == "__main__":
    #~ main() 

