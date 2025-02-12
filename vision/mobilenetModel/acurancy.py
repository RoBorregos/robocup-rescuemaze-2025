import tensorflow as tf
import numpy 
import cv2
import matplotlib.pyplot as plt
import tensorflow_datasets as tfd
from keras.utils import custom_object_scope
import tensorflow_hub as hub
import os
#carga de modelo
with custom_object_scope({'KerasLayer': hub.KerasLayer}):
    model = tf.keras.models.load_model("C:\\Users\\ferna\\Documents\\vision_artificial\\entrenamientoPocosDatos\\HSU_detection_mobilenetv2.h5")
folder_path="C:\\Users\\ferna\\Documents\\vision_artificial\\fotos\\none"
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
    if result==3:
        counter+=1
acurancy=counter/no_images
print(acurancy)

#luis model

# import tensorflow as tf
# import numpy 
# import cv2
# import matplotlib.pyplot as plt
# import tensorflow_datasets as tfd
# from keras.utils import custom_object_scope
# import tensorflow_hub as hub
# import os
# #carga de modelo
# model=tf.keras.models.load_model("C:\\Users\\ferna\\Documents\\vision_artificial\\entrenamientoPocosDatos\\letter_classification_model.h5")
# folder_path="C:\\Users\\ferna\\Documents\\vision_artificial\\fotos\\S"
# image_list=os.listdir(folder_path)
# no_images=0
# counter=0
# #contar elementos
# for i in image_list:
#     no_images+=1
#     print(no_images)
# for image_name in image_list:
#     #carga de imagen
#     image_path=os.path.join(folder_path,image_name)
#     image=cv2.imread(image_path,cv2.IMREAD_GRAYSCALE)
#     if image is None:
#         print("error al cargar la imagen")
#     #normalizar imagen
#     image=cv2.resize(image,(64,64))
#     image=image/255.0
#     image = numpy.expand_dims(image, axis=-1)  # Agregar un canal (grayscale)
#     image = numpy.expand_dims(image, axis=0)  # Agregar dimensión de batch
#     #realizar prediccion
#     result = model.predict(image)
#     result=numpy.argmax(result)
#     if result==1:
#         counter+=1
# acurancy=counter/no_images
# print(acurancy)

