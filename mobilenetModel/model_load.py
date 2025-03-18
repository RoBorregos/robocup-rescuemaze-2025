# import os
# os.environ["CUDA_VISIBLE_DEVICES"] = "-1"  # Desactiva todas las GPUs
# import tensorflow as tf
# import numpy 
# import cv2
# import matplotlib.pyplot as plt
# import tensorflow_datasets as tfd
# from tensorflow.keras.utils import custom_object_scope
# import tensorflow_hub as hub
# #carga de modelo
# with custom_object_scope({'KerasLayer': hub.KerasLayer}):
#     model = tf.keras.models.load_model("C:\\Users\\ferna\\Documents\\robocup-rescuemaze-2025-4\\mobilenetModel\\HSU_detection_mobilenetJetson.h5")
# #carga de imagen
# image=cv2.imread("C:\\Users\\ferna\\Documents\\vision_artificial\\fotos\\dataset_HSU\\S\\image7.jpeg")
# if image is None:
#     print("error al cargar la imagen")
# #normalizar imagen
# image = numpy.array(image).astype(float)/255
# image = cv2.resize(image, (224,224))
# cv2.imshow("Imagen", image.squeeze())  # Eliminar dimensiones extra para mostrar la imagen
# # Espera a presionar una tecla para cerrar la imagen
# cv2.waitKey(0)
# cv2.destroyAllWindows()
# #realizar prediccion
# result = model.predict(image.reshape(-1, 224, 224, 3))
# result=numpy.argmax(result)
# if result==0:
#     detection="H"
# elif result==1:
#     detection="S"
# elif result==2:
#     detection="U"
# elif result==3:
#     detection="none"
# print(detection)
import os
os.environ["CUDA_VISIBLE_DEVICES"] = "-1"  # Disable all GPUs

import tensorflow as tf
import numpy as np
import matplotlib.pyplot as plt
import tensorflow_hub as hub
from tensorflow.keras.utils import custom_object_scope
from PIL import Image  # Replaces OpenCV

# Load model
with custom_object_scope({'KerasLayer': hub.KerasLayer}):
    model = tf.keras.models.load_model("C:\\Users\\ferna\\Documents\\robocup-rescuemaze-2025-4\\mobilenetModel\\HSU_detection_mobilenetJetson.h5")

# Load image using PIL (instead of OpenCV)
image_path = "C:\\Users\\ferna\\Documents\\vision_artificial\\fotos\\dataset_HSU\\S\\image7.jpeg"
try:
    image = Image.open(image_path).convert("RGB")  # Ensure RGB format
except Exception as e:
    print(f"Error loading image: {e}")
    exit()

# Normalize image
image = np.array(image.resize((224, 224)), dtype=np.float32) / 255.0  # Resize and normalize

# Show image using Matplotlib instead of OpenCV
plt.imshow(image)
plt.axis("off")  # Hide axis
plt.show()

# Perform prediction
result = model.predict(image.reshape(1, 224, 224, 3))  # Reshape for model input
result = np.argmax(result)

# Label mapping
labels = {0: "H", 1: "S", 2: "U", 3: "none"}
detection = labels.get(result, "unknown")

print(detection)