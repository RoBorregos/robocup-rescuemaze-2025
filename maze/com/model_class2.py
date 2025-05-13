import cv2
import numpy as np
import tensorflow as tf
import time as t
from collections import Counter
import json

class Model():
    def __init__(self):
        # Cargar modelo .h5
        print("🚀 Cargando modelo .h5...")
        self.model = tf.keras.models.load_model("HSU_FinalP5.h5")

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
        with open("calibration_hsv.json", "r") as f:
            hsv_ranges = json.load(f)

        red1 = cv2.inRange(imageHSV, np.array(hsv_ranges["RED1"]["lower"]), np.array(hsv_ranges["RED1"]["upper"]))
        red2 = cv2.inRange(imageHSV, np.array(hsv_ranges["RED2"]["lower"]), np.array(hsv_ranges["RED2"]["upper"]))
        red_mask = cv2.bitwise_or(red1, red2)
        yellow_mask = cv2.inRange(imageHSV, np.array(hsv_ranges["YELLOW"]["lower"]), np.array(hsv_ranges["YELLOW"]["upper"]))
        green_mask = cv2.inRange(imageHSV, np.array(hsv_ranges["GREEN"]["lower"]), np.array(hsv_ranges["GREEN"]["upper"]))

        masks = {
            "RED": red_mask,
            "YELLOW": yellow_mask,
            "GREEN": green_mask
        }

        color_codes = {
            "RED": 0x7,
            "YELLOW": 0x8,
            "GREEN": 0x9
        }

        detected_color = None
        max_area = 0
        for color, mask in masks.items():
            contours, _ = cv2.findContours(mask, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)
            if not contours:
                continue
            largest = max(contours, key=cv2.contourArea)
            area = cv2.contourArea(largest)
            if area > 10000 and area > max_area:
                detected_color = color
                max_area = area

        return color_codes.get(detected_color, None)

    def detection(self, frame, attempts=1, min_repeats=1):
        results = []
        #frame_resized = cv2.resize(frame, (1000, 1000))

        for _ in range(attempts):
            hsv = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)
            color = self.getColor(hsv)
            if color is not None:
                results.append(color)
                print("DetColor😀")
                #return 0xA
            else:
                #cv2.imshow("edges", frame)
                #cv2.waitKey(0)
                #cv2.destroyAllWindows()

                # Normalize image
                #image_resized=cv2.resize(frame,(224,224))
                #image = np.array(image_resized).astype(float)/255
                #image = np.expand_dims(image.astype(np.float32) / 255.0, axis=0)

                image_resized = cv2.resize(frame, (224, 224))
                image = cv2.cvtColor(image_resized, cv2.COLOR_BGR2RGB)
                image = np.expand_dims(image.astype(np.float32) / 255.0, axis=0)
                prediction = self.model.predict(image)

                #realizar prediccion
                #image_show=(image[0]*255).astype(np.uint8)

                #cv2.imwrite("normalizada.jpg",image_show)
                #cv2.imshow("image",image_show)
                #cv2.waitKey(0)
                #cv2.destroyAllWindows()

                max_prob = np.max(prediction)
                if max_prob < 0.85:
                    result = 3
                else:
                    result = np.argmax(prediction)

                print(prediction)
                label = self.labels.get(result, 0xD)
                print(label)
                results.append(label)
                #if label != 0xD: 
                 #   return 0xB
                #return 0xD

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
