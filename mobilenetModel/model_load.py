import tensorflow as tf
import numpy 
import cv2
import matplotlib.pyplot as plt
import tensorflow_datasets as tfd
from keras.utils import custom_object_scope
import tensorflow_hub as hub
#carga de modelo
with custom_object_scope({'KerasLayer': hub.KerasLayer}):
    model = tf.keras.models.load_model("C:\\Users\\ferna\\Documents\\vision_artificial\\mobilenetModel\\HSU_detection_mobilenetv3.h5")
#carga de imagen
image=cv2.imread("C:\\Users\\ferna\\Documents\\vision_artificial\\fotos\\dataset_HSU\\S\\image6.jpeg")
if image is None:
    print("error al cargar la imagen")
#normalizar imagen
image = numpy.array(image).astype(float)/255
image = cv2.resize(image, (224,224))
cv2.imshow("Imagen", image.squeeze())  # Eliminar dimensiones extra para mostrar la imagen
# Espera a presionar una tecla para cerrar la imagen
cv2.waitKey(0)
cv2.destroyAllWindows()
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
