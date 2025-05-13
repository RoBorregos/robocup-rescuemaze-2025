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
        self.model = tf.keras.models.load_model("HSU_FinalP.h5")

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
                img = cv2.resize(frame, (1000, 1000))
                gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
                _, mask_lights = cv2.threshold(gray, 180, 255, cv2.THRESH_BINARY)
                kernel = np.ones((15, 15), np.uint8)
                mask_lights = cv2.dilate(mask_lights, kernel, iterations=1)
                mask_no_lights = cv2.bitwise_not(mask_lights)
                edges = cv2.Canny(gray, threshold1=20, threshold2=50)  
                edges = cv2.bitwise_and(edges, edges, mask=mask_no_lights)
                kernel = np.ones((10, 10), np.uint8)
                edges = cv2.dilate(edges, kernel, iterations=1)
                contours, _ = cv2.findContours(edges, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)
                contours = [c for c in contours if cv2.contourArea(c) > 4700]
                edges_rgb = cv2.cvtColor(edges, cv2.COLOR_GRAY2RGB)
                image_contours = cv2.drawContours(edges_rgb, contours, -1, (0, 255, 255), 2)
                image_contours = cv2.bitwise_not(image_contours)
                edges = cv2.bitwise_not(edges)

                height, width = image_contours.shape[:2]
                center = (width // 2, height // 2)
                min_distance = float('inf')
                closest_contour = None
                cut_image = np.ones((204, 204), np.uint8) * 255
                for contour in contours:
                    for point in contour:
                        x, y = point[0]
                        distance = np.sqrt((x - center[0])**2 + (y - center[1])**2)
                        if distance < min_distance:
                            min_distance = distance
                            closest_contour = contour
                if closest_contour is not None:
                    cv2.drawContours(edges, contours, -1, color=0, thickness=-1)
                    x, y, w, h = cv2.boundingRect(closest_contour)
                    cut_image = edges[y:y + h, x:x + w]

                cut_image = cv2.copyMakeBorder(cut_image, 20, 20, 20, 20, cv2.BORDER_CONSTANT, value=[255, 255, 255])
                image_resized = cv2.resize(cut_image, (224, 224))
                image = cv2.cvtColor(image_resized, cv2.COLOR_GRAY2RGB)
                image = np.expand_dims(image.astype(np.float32) / 255.0, axis=0)
                prediction = self.model.predict(image)

                max_prob = np.max(prediction)
                if max_prob < 0.70:
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
