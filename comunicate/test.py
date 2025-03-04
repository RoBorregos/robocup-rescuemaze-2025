# from Comunication import Esp32
import Constants 
from Esp_Comunication import Esp32
import time

robot=Esp32()
robot.connect()
while(True):
    confirmation=robot.sent_ahead()
    print(confirmation)
    confirmation2=robot.sent_stop()
    print(confirmation2)













# import serial

# esp32=serial.Serial("COM8",115200)

# while True:
#     message=esp32.readline().decode().strip()
#     # print(message)
#     if(message=="P"):
#         respuesta="hola\n"
#         esp32.write(respuesta.encode())
#         message="0"
