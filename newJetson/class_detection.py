import cv2
import numpy as np
import tensorflow as tf
import tensorflow_hub as hub
from tensorflow.keras.utils import custom_object_scope

class Model():
    def __init__(self):
        # Cargar modelo .h5 con capa personalizada
        print("🚀 Cargando modelo .h5 con capa personalizada...")
        with custom_object_scope({'KerasLayer': hub.KerasLayer}):
            self.model = tf.keras.models.load_model("HSU_detection_mobilenetJetson.h5")
        # Etiquetas hexadecimales
        self.labels = {0: 0xA, 1: 0xB, 2: 0xC, 3: 0xD}  # H, S,>
        
    def getColor(self, imageHSV):
        redLow1 = np.array([0, 120, 70], np.uint8)
        redHigh1 = np.array([10, 255, 255], np.uint8)
        redLow2 = np.array([170, 120, 70], np.uint8)
        redHigh2 = np.array([180, 255, 255], np.uint8)
        yellowLow = np.array([20, 100, 100], np.uint8)
        yellowHigh = np.array([35, 255, 255], np.uint8)
        greenLow = np.array([36, 80, 80], np.uint8)
        greenHigh = np.array([89, 255, 255], np.uint8)

        redPixels = cv2.inRange(imageHSV, redLow1, redHigh1) + cv2.inRange(imageHSV, redLow2, redHigh2)
        yellowPixels = cv2.inRange(imageHSV, yellowLow, yellowHigh)
        greenPixels = cv2.inRange(imageHSV, greenLow, greenHigh)

        numRed = cv2.countNonZero(redPixels)
        numYellow = cv2.countNonZero(yellowPixels)
        numGreen = cv2.countNonZero(greenPixels)

        threshold = 0.05 * (224 * 224)

        if numRed > threshold:
            return 0xA  # H
        elif numYellow > threshold:
            return 0xB  # S
        elif numGreen > threshold:
            return 0xC  # U
        else:
            return None

    def _detect_from_cap(self, frame):
        frame_resized = cv2.resize(frame, (224, 224))
        hsv = cv2.cvtColor(frame_resized, cv2.COLOR_BGR2HSV)
        color = self.getColor(hsv)
        if color is not None:
            print("🎨 Color detectado:", hex(color))
            return color

        # Modelo si no hay color
        image_np = np.expand_dims(frame_resized.astype(np.float32) / 255.0, axis=0)
        prediction = self.model.predict(image_np)
        result = np.argmax(prediction)
        label = self.labels.get(result, 0xD)
        print("🤖 Modelo detectó:", hex(label))
        return label
    def getDetectionRight(self):
        cap0= cv2.imread("frame_cam0.jpg")
        label = self._detect_from_cap(cap0)
        if label is not None:
            return label
        return 0xD  # Nada detectado
    def getDetectionLeft(self):
        cap1 = cv2.imread("frame_cam1.jpg")
        label = self._detect_from_cap(cap1)
        if label is not None:
            return label
        return 0xD

    def close(self):
        cv2.destroyAllWindows()