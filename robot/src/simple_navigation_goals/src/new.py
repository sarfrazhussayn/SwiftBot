# USAGE
# python picamera_fps_demo.py
# python picamera_fps_demo.py --display 1

# import the necessary packages
from __future__ import print_function
from imutils.video.pivideostream import PiVideoStream
from imutils.video import FPS
from picamera.array import PiRGBArray
from picamera import PiCamera
import argparse
import imutils
import time
import cv2
import os
def funct():
	# construct the argument parse and parse the arguments
	ap = argparse.ArgumentParser()
	ap.add_argument("-n", "--num-frames", type=int, default=100,
		help="# of frames to loop over for FPS test")
	ap.add_argument("-d", "--display", type=int, default=0,
		help="Whether or not frames should be displayed")
	args = vars(ap.parse_args())
	
	
	# created a *threaded *video stream, allow the camera sensor to warmup,
	# and start the FPS counter
	print("[INFO] sampling THREADED frames from `picamera` module...")
	vs = PiVideoStream().start()
	time.sleep(2.0)
	fps = FPS().start()
	
	faceCascade = cv2.CascadeClassifier("haarcascade_frontalface_default.xml")
	
	# loop over some frames...this time using the threaded stream
	while True:
		# grab the frame from the threaded video stream and resize it
		# to have a maximum width of 400 pixels
	
		frame =vs.read()
		frame = imutils.resize(frame, width=400)
	        # Our operations on the frame come here
		gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
		
	        print("draw")	
		# Detect faces in the image
		faces = faceCascade.detectMultiScale(
		        gray,
			scaleFactor=1.1,
			minNeighbors=5,
			minSize=(10, 10)
			#flags = cv2.CV_HAAR_SCALE_IMAGE
		)
	
		print("Found {0} faces!".format(len(faces)))
	        
		# Draw a rectangle around the faces
		for (x, y, w, h) in faces:
			cv2.rectangle(frame, (x, y), (x+w, y+h), (0, 255, 0), 2)
		if len(faces)>0:
			cv2.imwrite("./Gray.jpg",gray)
			pathe = str(os.path.abspath("./Gray.jpg"))
			#print(pathe)
			break
		# check to see if the frame should be displayed to our screen
		if args["display"] > 0:
			cv2.imshow("Frame", frame)
			key = cv2.waitKey(1) & 0xFF
	
		# update the FPS counter
		fps.update()
	
	# stop the timer and display FPS information
	fps.stop()
	print("[INFO] elasped time: {:.2f}".format(fps.elapsed()))
	print("[INFO] approx. FPS: {:.2f}".format(fps.fps()))
	
	# do a bit of cleanup
	cv2.destroyAllWindows()
	vs.stop()
	return pathe
