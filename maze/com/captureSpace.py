import cv2
import time
import os

# GStreamer pipeline for CSI camera
def gstreamer_pipeline(sensor_id=0, width=640, height=480, framerate=30):
    return (
        f"nvarguscamerasrc sensor-id={sensor_id} ! "
        f"video/x-raw(memory:NVMM), width={width}, height={height}, format=NV12, framerate={framerate}/1 ! "
        f"nvvidconv flip-method=0 ! video/x-raw, format=BGRx ! "
        f"videoconvert ! video/x-raw, format=BGR ! appsink drop=true"
    )

# Open CSI camera 0
cap = cv2.VideoCapture(gstreamer_pipeline(0), cv2.CAP_GSTREAMER)

if not cap.isOpened():
    print("No se pudo abrir la cámara CSI (sensor-id=0)")
    exit()

print("Presiona ESPACIO para capturar una imagen. Presiona ESC para salir.")
img_count = 0

while True:
    ret, frame = cap.read()
    if not ret:
        print("No se pudo leer el frame de la cámara.")
        break

    cv2.imshow("CSI Camera", frame)
    key = cv2.waitKey(1) & 0xFF

    if key == 27:  # ESC
        print("Saliendo...")
        break
    elif key == 32:  # SPACE
        filename = f"capture_{img_count:03d}.jpg"
        cv2.imwrite(filename, frame)
        print(f"Imagen guardada: {filename}")
        img_count += 1

cap.release()
cv2.destroyAllWindows()
