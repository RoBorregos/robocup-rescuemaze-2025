import cv2
import numpy as np
import time as t
from collections import Counter

labels = {0: 0xA, 1: 0xB, 2: 0xC, 3: 0xD} 

def getColor( imageHSV):
    redLow1 = np.array([0, 100, 102], np.uint8)
    redHigh1 = np.array([10, 255, 255], np.uint8)
    redLow2 = np.array([170, 100, 102], np.uint8)
    redHigh2 = np.array([180, 255, 255], np.uint8)
    yellowLow = np.array([20, 110, 102], np.uint8)
    yellowHigh = np.array([35, 255, 255], np.uint8)
    greenLow = np.array([36, 80, 102], np.uint8)
    greenHigh = np.array([85, 255, 255], np.uint8)
    redPixels = cv2.inRange(imageHSV, redLow1, redHigh1) + cv2.inRange(imageHSV, redLow2, redHigh2)
    yellowPixels = cv2.inRange(imageHSV, yellowLow, yellowHigh)
    greenPixels = cv2.inRange(imageHSV, greenLow, greenHigh)

    numRed = cv2.countNonZero(redPixels)
    numYellow = cv2.countNonZero(yellowPixels)
    numGreen = cv2.countNonZero(greenPixels)

    threshold = 0.1 * (224 * 224)
    if numRed > threshold:
        return 0xA  # H
    elif numYellow > threshold:
        return 0xB  # S
    elif numGreen > threshold:
        return 0xC  # U
    else:
        return None
    

def detection(frame):
    #frame = cv2.resize(frame, (224, 224))
    hsv = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)
    color = getColor(hsv)
    print(color)
    
while True:
    cap0 = cv2.imread("frame_cam1.jpg")
    detection(cap0)
