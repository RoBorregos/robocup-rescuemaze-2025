import cv2
import numpy as np
import tensorflow as tf
import time as t
from collections import Counter

class Model():
    def __init__(self):
        # Cargar modelo .h5
        print("🚀 Cargando modelo .h5...")
        self.model = tf.keras.models.load_model("HSU_detection_Jetson6.h5")

        # Etiquetas hexadecimales
        self.labels = {0: 0xA, 1: 0xB, 2: 0xC, 3: 0xD}  # H, S, >, fallback

        # Ejecutar warmup
        self.warmup()

    def warmup(self):
        print("🔥 STARTING WARMUP ...")
        duration = 5  # segundos
        end_time = t.time() + duration
        passed_times = 0

        img = cv2.imread("S2.jpg")
        if img is None:
            print("❌ Imagen S2.jpg no encontrada.")
            return

        img_resized = cv2.resize(img, (224, 224))
        img_norm = np.expand_dims(img_resized.astype(np.float32) / 255.0, axis=0)

        start = t.time()
        while t.time() < end_time:
            _ = self.model.predict(img_norm)
            passed_times += 1
        print(f"✅ WARMUP FINISHED: {passed_times} veces predicho en {t.time() - start:.2f} segundos")

    def getColor(self, imageHSV):
        redLow1 = np.array([0, 100, 102], np.uint8)
        redHigh1 = np.array([10, 255, 255], np.uint8)
        redLow2 = np.array([170, 100, 102], np.uint8)
        redHigh2 = np.array([180, 255, 255], np.uint8)
        yellowLow = np.array([20, 110, 102], np.uint8)
        yellowHigh = np.array([35, 255, 255], np.uint8)
        greenLow = np.array([36, 80, 102], np.uint8)
        greenHigh = np.array([85, 255, 255], np.uint8)

        redPixels = cv2.inRange(imageHSV, redLow1, redHigh1) + cv2.inRange(imageHSV, redLow2, redHigh2)
        yellowPixels = cv2.inRange(imageHSV, yellowLow, yellowHigh)
        greenPixels = cv2.inRange(imageHSV, greenLow, greenHigh)

        numRed = cv2.countNonZero(redPixels)
        numYellow = cv2.countNonZero(yellowPixels)
        numGreen = cv2.countNonZero(greenPixels)

        threshold = 0.1 * (224 * 224)
        if numRed > threshold:
            return 0xA  # H
        elif numYellow > threshold:
            return 0xB  # S
        elif numGreen > threshold:
            return 0xC  # U
        else:
            return None

    def detection(self, frame, attempts=3, min_repeats=3):
        results = []
        frame_resized = cv2.resize(frame, (224, 224))

        for _ in range(attempts):
            hsv = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)
            color = self.getColor(hsv)
            if color is not None:
                results.append(color)
            else:
                image_np = np.expand_dims(frame_resized.astype(np.float32) / 255.0, axis=0)
                prediction = self.model.predict(image_np)

                max_prob = np.max(prediction)
                if max_prob < 0.60:
                    result = 3
                else:
                    result = np.argmax(prediction)

                print(prediction)
                #result = np.argmax(prediction)
                label = self.labels.get(result, 0xD)
                results.append(label)

        count = Counter(results)
        most_common = count.most_common(1)[0]
        if most_common[1] >= min_repeats:
            print(f"✅ Detección estable: {hex(most_common[0])} repetido {most_common[1]} veces")
            return most_common[0]
        else:
            print("⚠️ No se alcanzó una inferencia estable, devolviendo 0xD")
            return 0xD

    def getDetectionRight(self):
        cap0 = cv2.imread("frame_cam0.jpg")
        return self.detection(cap0)

    def getDetectionLeft(self):
        cap1 = cv2.imread("frame_cam1.jpg")
        return self.detection(cap1)

    def close(self):
        cv2.destroyAllWindows()
