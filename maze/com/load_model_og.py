import cv2
import numpy as np
import tensorflow as tf
import tensorflow_hub as hub
from tensorflow.keras.utils import custom_object_scope

class Model():
    def __init__(self):
        # Cargar modelo .h5 con capa personalizada
        with custom_object_scope({'KerasLayer': hub.KerasLayer}):
            self.model = tf.keras.models.load_model("HSU_detection_mobilenetJetson.h5")

        # Etiquetas hexadecimales
        self.labels = {0: 0xA, 1: 0xB, 2: 0xC, 3: 0xD}

        # Pipelines CSI para Jetson Nano
        self.cam0 = self._gstreamer_pipeline(sensor_id=0)
        self.cam1 = self._gstreamer_pipeline(sensor_id=1)

        self.cap0 = cv2.VideoCapture(self.cam0, cv2.CAP_GSTREAMER)
        self.cap1 = cv2.VideoCapture(self.cam1, cv2.CAP_GSTREAMER)

        if not self.cap0.isOpened():
            print("❌ No se pudo acceder a la cámara 0")
        if not self.cap1.isOpened():
            print("❌ No se pudo acceder a la cámara 1")

    def _gstreamer_pipeline(self, sensor_id=0, width=640, height=480, framerate=30):
        return (
            f"nvarguscamerasrc sensor-id={sensor_id} ! "
            f"video/x-raw(memory:NVMM), width={width}, height={height}, format=NV12, framerate={framerate}/1 ! "
            f"nvvidconv flip-method=0 ! video/x-raw, format=BGRx ! "
            f"videoconvert ! video/x-raw, format=BGR ! appsink"
        )

    def getColor(self, imageHSV):
        redLow1 = np.array([0, 200, 25], np.uint8)
        redHigh1 = np.array([10, 255, 255], np.uint8)
        redLow2 = np.array([175, 200, 25], np.uint8)
        redHigh2 = np.array([180, 255, 255], np.uint8)
        yellowLow = np.array([25, 150, 25], np.uint8)
        yellowHigh = np.array([35, 255, 255], np.uint8)
        greenLow = np.array([42, 150, 25], np.uint8)
        greenHigh = np.array([72, 255, 255], np.uint8)

        redPixels = cv2.add(
            cv2.inRange(imageHSV, redLow1, redHigh1),
            cv2.inRange(imageHSV, redLow2, redHigh2)
        )
        yellowPixels = cv2.inRange(imageHSV, yellowLow, yellowHigh)
        greenPixels = cv2.inRange(imageHSV, greenLow, greenHigh)

        numRed = cv2.countNonZero(redPixels)
        numYellow = cv2.countNonZero(yellowPixels)
        numGreen = cv2.countNonZero(greenPixels)

        threshold = 0.05 * (224 * 224)

        if numRed > threshold:
            return 0xA
        elif numYellow > threshold:
            return 0xB
        elif numGreen > threshold:
            return 0xC
        else:
            return None

    def _detect_from_cap(self, cap):
        ret, frame = cap.read()
        if not ret:
            return None

        frame_resized = cv2.resize(frame, (224, 224))
        hsv = cv2.cvtColor(frame_resized, cv2.COLOR_BGR2HSV)
        color = self.getColor(hsv)
        if color is not None:
            print("🎨 Color detectado:", color)
            return color
	#image_np = np.array(frame).astype(float)/255
        image_np = np.expand_dims(frame_resized.astype(np.float32) / 255.0, axis=0)
        cv2.imshow("Foto Capturada", frame)
        # Esperar hasta que el usuario presione una tecla para continuar
        cv2.waitKey(2000)
        output = self.model.predict(image_np)
        prediction = np.argmax(output)
        label = self.labels.get(prediction, 0xD)
        print("🧠 Modelo detectó:", label)
        return label

    def getDetection(self):
        # Prioridad: cam0 → cam1
        for cam in [self.cap0, self.cap1]:
            label = self._detect_from_cap(cam)
            if label is not None and label != 0xD:
                return label
        return 0xD  # None / default

    def close(self):
        self.cap0.release()
        self.cap1.release()
        cv2.destroyAllWindows()
