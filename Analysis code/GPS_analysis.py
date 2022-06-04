import csv
import math
from typing import Tuple
import re
import numpy as np
import json
from datetime import datetime, timezone, timedelta
import pytz

def CDT_degree_converter(lati: str, longi: str) -> Tuple[float, float]:
    lati = abs(float(lati)); longi = abs(float(longi))
    lati_temp = format(lati, '.4f'); longi_temp = format(longi, '.4f') #format sanity checking
    print(lati_temp, longi_temp)
    lati = math.floor(lati) + int(str(lati_temp)[3:5]) / 60 + int(str(lati_temp)[5:7]) * 0.6 / 3600
    longi = math.floor(longi) + int(str(longi_temp)[3:5]) / 60 + int(str(longi_temp)[5:7]) * 0.6 / 3600
    if lati > 80:
        lati *= -1
    if longi > 80:
        longi *= -1
    return lati, longi

def green_dots_with_gps() -> None:
    file = open('lastMonth.csv')
    csvreader = csv.reader(file)
    header = next(csvreader)
    print(header)
    rows = []
    for row in csvreader:
        if row[5] == "gps":
            x = row[-1].split(",")
            lati, logi = CDT_degree_converter(x[0], x[1])
            rows.append([lati, logi, 'green'])
    header = ['lati', 'longi', 'color']
    with open('lastMonth_preprocessed.csv', 'w', encoding='UTF8', newline='') as f:
        writer = csv.writer(f)
        # write the header
        writer.writerow(header)
        # write the data
        for i in rows:
            writer.writerow(i)

#Parse GPS & time data from phone
def Phone_GPS_Parser():
    GPXfile = 'now.gpx'
    data = open(GPXfile).read()
    lat = np.array(re.findall(r'lat="([^"]+)', data), dtype=float)
    lon = np.array(re.findall(r'lon="([^"]+)', data), dtype=float)
    time_temp = re.findall(r'<time>([^\<]+)', data)
    time = np.array([])
    for i in time_temp:
        utc_date = datetime.strptime(i, '%Y-%m-%dT%H:%M:%SZ').replace(tzinfo=timezone.utc)
        time = np.append(time, utc_date.astimezone(pytz.timezone('US/Central')))
    return np.array(list(zip(lat, lon, time))) # already sorted

# Parse Seq & time data from Helium Console
def Helium_Console_Parser():
    filePath = "%s.csv" % ("1434-1543")
    sequenceNumberWithTimeList = []

    with open(filePath, newline='') as csvfile:
        data = csv.reader(csvfile, delimiter=',')
        for row in data:
            if row[5] == "motion":
                utc_date = datetime.strptime(row[0], '%Y-%m-%dT%H:%M:%SZ').replace(tzinfo=timezone.utc)
                temp_date = utc_date.astimezone(pytz.timezone('US/Central'))
                tuple = (row[7], temp_date) #seq, time
                if sequenceNumberWithTimeList[-1][-1] == row[7]:
                    continue
                sequenceNumberWithTimeList.append(tuple)
    csvfile.close()
    return sequenceNumberWithTimeList

#Generate [geolocation, time] data for visualization
def Helium_consolidator() -> None:
    seq_time_list = Helium_Console_Parser()
    np_aa = Phone_GPS_Parser()
    return_list = []
    counter = 0
    for consoler in seq_time_list:
        for timer in np_aa:
            temp_time_diff = (timer[2] - consoler[1])
            if consoler[1] > timer[2]:
                temp_time_diff = consoler[1] - timer[2]
            if temp_time_diff <= timedelta(seconds = 4): # todo: customizable?
                counter += 1
                #lat, longi, real time, color
                return_list.append([timer[0], timer[1], consoler[1], "green"]) #now it's all green
                break
    header = ['lati', 'longi', 'time', 'color']
    with open('5-28-morning.csv', 'w', encoding='UTF8', newline='') as f:
        writer = csv.writer(f)
        # write the header
        writer.writerow(header)
        # write the data
        for i in return_list:
            writer.writerow(i)

def trial() -> None: # Play with time for some familarity
    date = '2022-05-27T19:02:21Z'
    utc_date = datetime.strptime(date, '%Y-%m-%dT%H:%M:%SZ').replace(tzinfo=timezone.utc)
    london_date = utc_date.astimezone(pytz.timezone('US/Central'))
    print(london_date)

    temp1 = 'Tue, 24 May 2022 21:57:56 GMT'
    temp2 = 'Tue, 24 May 2022 21:58:46 GMT'
    utc_date = datetime.strptime(temp1, '%a, %d %b %Y %H:%M:%S GMT')
    aa1 = utc_date.astimezone(pytz.timezone('US/Central'))
    print(aa1)
    aa3 = datetime.strptime(temp2, '%a, %d %b %Y %H:%M:%S GMT').astimezone(pytz.timezone('US/Central')) - aa1
    print(aa3) #difference is datetime.timedelta
    if datetime.strptime(temp2, '%a, %d %b %Y %H:%M:%S GMT').astimezone(pytz.timezone('US/Central')) - aa1:
        print("ee")
    else:
        print("nn")

if __name__ == "__main__":
   #green_dots_with_gps()
   #Phone_GPS_Parser()
   Helium_consolidator()
   #trial()
