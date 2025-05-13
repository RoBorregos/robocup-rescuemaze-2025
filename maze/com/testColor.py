import cv2
import numpy as np
import json
import time

def get_color(imageHSV, hsv_ranges):
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
        "RED": 0xA,
        "YELLOW": 0xB,
        "GREEN": 0xC
    }

    detected_color = None
    max_area = 0
    for color, mask in masks.items():
        contours, _ = cv2.findContours(mask, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)
        if not contours:
            continue
        largest = max(contours, key=cv2.contourArea)
        area = cv2.contourArea(largest)
        if area > 1000 and area > max_area:
            detected_color = color
            max_area = area

    return color_codes.get(detected_color, None), detected_color

# Cargar archivo de configuración HSV
with open("calibration_hsv.json", "r") as f:
    hsv_ranges = json.load(f)

# Pipeline GStreamer para cada cámara CSI
def gstreamer_pipeline(sensor_id=0, capture_width=1280, capture_height=720, display_width=640, display_height=480, framerate=30, flip_method=0):
    return (
        f"nvarguscamerasrc sensor-id={sensor_id} ! "
        f"video/x-raw(memory:NVMM), width={capture_width}, height={capture_height}, format=NV12, framerate={framerate}/1 ! "
        f"nvvidconv flip-method={flip_method} ! "
        f"video/x-raw, width={display_width}, height={display_height}, format=BGRx ! "
        f"videoconvert ! "
        f"video/x-raw, format=BGR ! appsink"
    )

# Abrir ambas cámaras CSI
cap_left = cv2.VideoCapture(gstreamer_pipeline(sensor_id=0), cv2.CAP_GSTREAMER)
cap_right = cv2.VideoCapture(gstreamer_pipeline(sensor_id=1), cv2.CAP_GSTREAMER)

if not cap_left.isOpened() or not cap_right.isOpened():
    print("❌ No se pudieron abrir ambas cámaras CSI con GStreamer.")
    exit()

print("🎥 Detectando desde ambas cámaras CSI (Ctrl+C para salir)...")
try:
    while True:
        ret0, frame0 = cap_left.read()
        ret1, frame1 = cap_right.read()

        if not ret0 or not ret1:
            print("⚠️ Error leyendo de las cámaras.")
            continue

        hsv0 = cv2.cvtColor(frame0, cv2.COLOR_BGR2HSV)
        hsv1 = cv2.cvtColor(frame1, cv2.COLOR_BGR2HSV)

        code0, name0 = get_color(hsv0, hsv_ranges)
        code1, name1 = get_color(hsv1, hsv_ranges)

        print(f"🔵 Izquierda (cam0): {name0 or 'None'} ({hex(code0) if code0 else '0xD'}) | 🔴 Derecha (cam1): {name1 or 'None'} ({hex(code1) if code1 else '0xD'})")
        time.sleep(0.5)
except KeyboardInterrupt:
    print("\n👋 Finalizando detección.")
finally:
    cap_left.release()
    cap_right.release()
