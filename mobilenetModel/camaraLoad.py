import tensorflow as tf
import numpy 
import cv2
import matplotlib.pyplot as plt
import tensorflow_datasets as tfd
from keras.utils import custom_object_scope
import tensorflow_hub as hub
import time
#carga de modelo
with custom_object_scope({'KerasLayer': hub.KerasLayer}):
    model = tf.keras.models.load_model("C:\\Users\\ferna\\Documents\\vision_artificial\\mobilenetModel\\HSU_detection_mobilenetv4.h5")
#imiciar camara
cam=cv2.VideoCapture(1)
#verificar camara
if not cam.isOpened():
    print("error al iniciar camara")
    exit()
intervalCam=0.5
while True:
    #capturar imagen
    ret,image=cam.read()

    #normalizar imagen
    image = numpy.array(image).astype(float)/255
    image = cv2.resize(image, (224,224))
    #mostrar cam
    cv2.imshow("Imagen", image.squeeze())
    cv2.waitKey(1)
    
    #realizar prediccion
    result = model.predict(image.reshape(-1, 224, 224, 3))
    result=numpy.argmax(result)
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

