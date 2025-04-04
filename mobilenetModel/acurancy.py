import os
os.environ["CUDA_VISIBLE_DEVICES"] = "-1"  # Disable all GPUs#carga de modelo
import tensorflow as tf
import numpy 
import cv2
import matplotlib.pyplot as plt
import tensorflow_datasets as tfd
from tensorflow.keras.utils import custom_object_scope
import tensorflow_hub as hub

# with custom_object_scope({'KerasLayer': hub.KerasLayer}):
#     model = tf.keras.models.load_model("C:\\Users\\ferna\\Documents\\robocup-rescuemaze-2025-4\\mobilenetModel\\HSU_detection_mobilenetJetson.h5")

model = tf.keras.models.load_model("C:\\Users\\ferna\\Documents\\robocup-rescuemaze-2025-4\\mobilenetModel\\HSU_detection_mobilenetJetson1.h5")

folder_path="C:\\Users\\ferna\\Documents\\robocup-rescuemaze-2025-4\\fotos\\U"
image_list=os.listdir(folder_path)
no_images=0
counter=0
#contar elementos
for i in image_list:
    no_images+=1
    print(no_images)
for image_name in image_list:
    #carga de imagen
    image_path=os.path.join(folder_path,image_name)
    image=cv2.imread(image_path)
    if image is None:
        print("error al cargar la imagen")
    #normalizar imagen
    image = numpy.array(image).astype(float)/255
    image = cv2.resize(image, (224,224))
    #realizar prediccion
    result = model.predict(image.reshape(-1, 224, 224, 3))
    result=numpy.argmax(result)
    if result==2:
        counter+=1
acurancy=counter/no_images
print(acurancy)


#acuarancy
# with keras layer:
# H:80%
# S:93
# U:90
# none:94


# withOUT keras layer:
# H:83%
# S:91
# U:92
# none:93

# withOUT keras layer:
# H:94%
# S:83
# U:77