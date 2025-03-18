# from Comunication import Esp32
import Constants 
from Esp_Comunication import Esp32
import time

robot=Esp32()
robot.connect()

while(True):
    robot.sentDetection()




