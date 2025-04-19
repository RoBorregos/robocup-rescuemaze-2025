import cv2
import numpy as np
import tensorflow as tf
import tensorflow_hub as hub
from tensorflow.keras.utils import custom_object_scope
import time
import os

class Model:
    def __init__(self):
        print("🚀 Cargando el modelo...")
        # Carga el modelo .h5 con la capa personalizada
        with custom_object_scope({'KerasLayer': hub.KerasLayer}):
            self.model = tf.keras.models.load_model("HSU_detection_mobilenetJetson.h5")
        self.model = tf.keras.models.load_model("HSU_detection_mobilenetJetson2.h5")
        self.labels = {0: "H", 1: "S", 2: "U", 3: "?"}

    def predict_image(self, image):
        image_resized = cv2.resize(image, (224, 224))
        image_normalized = image_resized.astype(np.float32) / 255.0
        input_tensor = np.expand_dims(image_normalized, axis=0)
        prediction = self.model.predict(input_tensor)
        class_id = int(np.argmax(prediction))
        return self.labels.get(class_id, "?")

# Inicializa el modelo
model = Model()

print("✅ Modelo cargado. Esperando imágenes para procesar...")
try:
    while True:
        detection = None

        # Procesa la imagen de la cámara 0
        if os.path.exists("frame_cam0.jpg"):
            img0 = cv2.imread("frame_cam0.jpg")
            if img0 is not None:
                det0 = model.predict_image(img0)
                if det0 != "?":
                    detection = det0
                    print(f"📷 cam0 detectó: {det0}")

        # Procesa la imagen de la cámara 1
        if os.path.exists("frame_cam1.jpg"):
            img1 = cv2.imread("frame_cam1.jpg")
            if img1 is not None:
                det1 = model.predict_image(img1)
                if det1 != "?":
                    detection = det1
                    print(f"📷 cam1 detectó: {det1}")

        if detection is None:
            print("🔍 No se detectó nada en esta iteración.")
except KeyboardInterrupt:
    print("🛑 Predicción interrumpida por el usuario.")












































import os
import cv2
import numpy as np
import tensorflow as tf
import tensorflow_hub as hub
import tensorflow.keras.backend as K
from tensorflow.keras.utils import custom_object_scope

class Model:
    def __init__(self):
        print("🚀 Cargando modelo .h5 con capa personalizada...")

        with custom_object_scope({'KerasLayer': hub.KerasLayer}):
            self.model = tf.keras.models.load_model("HSU_detection_mobilenetJetson.h5")
        print(1.1)
        self.labels = {0: "H", 1: "S", 2: "U", 3: "?"}  # puedes ajustar los labels si necesitas

    def predict_image(self, image):
        image_resized = cv2.resize(image, (224, 224))
        image_normalized = image_resized.astype(np.float32) / 255.0
        input_tensor = np.expand_dims(image_normalized, axis=0)
        prediction = self.model.predict(input_tensor)
        class_id = int(np.argmax(prediction))
        return self.labels.get(class_id, "?")
