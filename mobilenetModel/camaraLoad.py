import tensorflow as tf
import numpy as np
import cv2
import matplotlib.pyplot as plt
import tensorflow_datasets as tfd
from keras.utils import custom_object_scope
import tensorflow_hub as hub
import time
def getColor(imageHSV):
    #red
    redLow1=np.array([0,200,25],np.uint8)
    redHigh1=np.array([10,255,255],np.uint8)
    redLow2=np.array([175,200,25],np.uint8)
    redHigh2=np.array([180,255,255],np.uint8)
    #yellow
    yellowLow=np.array([25,150,25],np.uint8)
    yellowHigh=np.array([35,255,255],np.uint8)
    #green
    greenLow=np.array([42,150,25],np.uint8)
    greenHigh=np.array([72,255,255],np.uint8)
    #search colors
    redPixels1=cv2.inRange(imageHSV,redLow1,redHigh1)
    redPixels2=cv2.inRange(imageHSV,redLow2,redHigh2)
    redPixels=cv2.add(redPixels1,redPixels2)
    yellowPixels=cv2.inRange(imageHSV,yellowLow,yellowHigh)
    greenPixels=cv2.inRange(imageHSV,greenLow,greenHigh)
    numGreenPixels=cv2.countNonZero(greenPixels)
    numRednPixels=cv2.countNonZero(redPixels)
    numYellowPixels=cv2.countNonZero(yellowPixels)
    # result = cv2.bitwise_and(image, image, mask=mask)
    imageHSV_BGR = cv2.cvtColor(imageHSV, cv2.COLOR_HSV2BGR)
    # cv2.imshow("Imagen", redPixels.squeeze())
    # cv2.waitKey(1)
    thresholdColor=5*(224*224)/100#minimum 4 porcent
    if numYellowPixels>thresholdColor: color = 2
    elif numRednPixels>thresholdColor: color = 3
    elif numGreenPixels>thresholdColor: color = 1
    else: color=0
    return color
#carga de modelo
with custom_object_scope({'KerasLayer': hub.KerasLayer}):
    model = tf.keras.models.load_model("C:\\Users\\ferna\\Documents\\vision_artificial\\mobilenetModel\\HSU_detection_mobilenetv5.h5")
#imiciar camara
cam=cv2.VideoCapture(1)
# #verificar camara
if not cam.isOpened():
    print("error al iniciar camara")
    exit()
intervalCam=0.5
while True:
    # #capturar imagen
    ret,image=cam.read()
    image_original=image
    colors=["white","green","yellow","red"]
    image=cv2.cvtColor(image,cv2.COLOR_BGR2HSV)
    image = cv2.resize(image, (224,224))
    color=getColor(image)
    # #normalizar imagen
    image=cv2.cvtColor(image,cv2.COLOR_HSV2BGR)
    image = np.array(image).astype(float)/255
    # #mostrar cam
    cv2.imshow("Imagen", image.squeeze())
    cv2.waitKey(1)
    if color>0:
        print(colors[color])
    else:
        #realizar prediccion
        result = model.predict(image.reshape(-1, 224, 224, 3))
        result=np.argmax(result)
        if result==0:
            detection="H"
        elif result==1:
            detection="S"
        elif result==2:
            detection="U"
        elif result==3:
            detection="none"
        print(detection)
    time.sleep(intervalCam)
    cv2.destroyAllWindows()
