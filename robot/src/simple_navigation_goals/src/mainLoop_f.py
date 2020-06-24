#!/usr/bin/env python
import requests
import json, os
import cv2
import rospy
from std_msgs.msg import Float32
import random
import time
from new import funct #uncomment this when on pi

headers={'content-type' : 'image/jpg'}

currentBookingInfo = ""
currentLocation = [1,1]
customerId = None

pub = rospy.Publisher('locker', Float32, queue_size=10)
rospy.init_node('talker', anonymous=False)

URL='https://8066ab1ca4ae.ngrok.io/upload'

def send_nodes(img_file=None):
    f=open("Gray.jpg","rb")
    img={'file': f}
    response=requests.post(URL,files=img)
    response=response.json()
    print(response)
    if " " in response['message']:
        return response['message'].replace(" ","_")
    return response['message']

def getNextBookingDetails(currentBookingId):
    URL = "https://api.anomoz.com/api/swift/post/read_new_booking.php?currentBookingId="+str(currentBookingId)
    r = requests.get(url = URL)
    # extracting data in json format
    data = r.json()
    info = (data[0])
    return info

def markBookingAsEnded(currentBookingId):
    URL = "https://api.anomoz.com/api/swift/post/mark_booking_as_ended.php?currentBookingId="+str(currentBookingId)
    r = requests.get(url = URL)
    return True

def getPickupLocation(currentBookingInfo):
    print("getPickupLocation set: ", [currentBookingInfo["fromLocation_lat"], currentBookingInfo["fromLocation_lng"]])
    return([currentBookingInfo["fromLocation_lat"], currentBookingInfo["fromLocation_lng"]])

def getFinalLocation(currentBookingInfo):
    print("getFinalLocation set: ", [currentBookingInfo["toLocation_lat"], currentBookingInfo["toLocation_lng"]])
    return([currentBookingInfo["toLocation_lat"], currentBookingInfo["toLocation_lng"]])
    
#def startNavigator(currentLocation, destination):
    #programCommand = "rosrun simple_navigation_goals simple_navigation_goals  _param_x:="+str(destination[0])+" _param_y:="+str(destination[1])
    #if os.system(programCommand) == 0:
        #print("program finished successfully")
    #else:
        #print("launch failed")

def startNavigator(resp):
    # programCommand = "rosrun simple_navigation_goals simple_navigation_goals  _param_x:="+str(destination[0])+" _param_y:="+str(destination[1])
    # if os.system(programCommand) == 0:
    #     print("program finished successfully")
    # else:
    #     print("launch failed")
    URL = "http://192.168.86.28:2020/movebase?resp="+str(resp[0])+"&resp1="+str(resp[1])
    r = requests.get(url = URL)
    return True
    
def generateExe():
    if os.system("g++ -g foo.cpp -o foo") == 0:
        print ("exe compiled")
    else:
        print ("exe compiling Failed")

def open_lock(data):
    pub.publish(data)
    # try:
         #rospy.init_node('talker', anonymous=True)
     #except: 
         #print("some")
    print("hello")
    rospy.Subscriber("open_lock", Float32, callback)
    rospy.sleep(5.0)

def callback(data):
    print(data.data,"this is data")
#    time.sleep(5.0)
#    if data.data=="open":
#        time.sleep(10.0)
#        pub.publish("close")
#	time.sleep(5.0)
#     if data.data=="close":
#         rospy.sleep(10.0)
#         # return "some"
#         # rospy.signal_shutdown("The lid has been closed")
#         # return "some"

def Get_pin():
    #send the number as a callback to customer to enter pin
    #get callback from app that the pin has been entered successfully
    try:
        file1 = open("lockstatus.txt","r+")
        fileinp = file1.read()
        file1.close()
        if(fileinp=="opened"):
            return True
        else:
            return False
    except:
        return False

def runSingleCycle():
    
    currentBookingInfo = getNextBookingDetails(0)
    #currentBookingInfo = {'bookingId': '187', 'timeAdded': '1573634449', 'fromLocation': 'Auditorium', 'toLocation': 'Auditorium', 'fromPerson': 'Dr. taj', 'toPerson': 'Ahsan', 'status': 'waiting', 'fromLocation_lat': '1.973', 'fromLocation_lng': '0.171', 'toLocation_lat': '0.52', 'toLocation_lng': '1.00'}
    print("currentBookingInfo updated: ", currentBookingInfo)
    customerId=currentBookingInfo['toPerson']
    #go to pickup location
    pickupLocation = getPickupLocation(currentBookingInfo)
    print(startNavigator(pickupLocation)) #program stops until
    #print(startNavigator(currentBookingInfo))
    #open lock for pickup
    open_lock(1)

    #to to destination
    dropoffLocation = getFinalLocation(currentBookingInfo)
    print("dropoffLocation", dropoffLocation)
    startNavigator(dropoffLocation)

    #openlock for dropoff
    name=""
    if(Get_pin()==False):
	try:
	    name=send_nodes(funct())
	except:
	    print("no")
    if name==currentBookingInfo["toPerson"]:
        open_lock(1)
        print("Need pin to unlock")
    else:
        while(True):
            if(Get_pin()==True):
	        open("lockstatus.txt", 'w').close()
                break
    open_lock(1)
    #mark booking
    markBookingAsEnded(currentBookingInfo['bookingId'])
    
    #runSingleCycle()
    
    
def main():
    #generateExe()

    runSingleCycle()
    #verifyUser()
    #pass
        
    

    #get next booking Status
# print(send_nodes(funct())) #uncomment this when on pi
main()
rospy.signal_shutdown("The lid has been closed")
#print()
