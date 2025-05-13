
import cv2
import time
import os

# Configura el pipeline de GStreamer para las cámaras CSI
def gstreamer_pipeline(sensor_id=0):
    return (
        f"nvarguscamerasrc sensor-id={sensor_id} ! "
        f"video/x-raw(memory:NVMM), width=640, height=480, format=NV12, framerate=30/1 ! "
        f"nvvidconv flip-method=0 ! video/x-raw, format=BGRx ! "
        f"videoconvert ! video/x-raw, format=BGR ! appsink drop=true"
    )

# Inicializa las cámaras
cap0 = cv2.VideoCapture(gstreamer_pipeline(0), cv2.CAP_GSTREAMER)
cap1 = cv2.VideoCapture(gstreamer_pipeline(1), cv2.CAP_GSTREAMER)

if not cap0.isOpened():
    print("❌ No se pudo abrir la cámara 0")
if not cap1.isOpened():
    print("❌ No se pudo abrir la cámara 1")

try:
    while True:
        ret0, frame0 = cap0.read()
        ret1, frame1 = cap1.read()

        if ret0:
            #recorte_cuadrado = frame0[0:300, 80:640]
            #frame0 = cv2.rotate(frame0,cv.ROTATE_180)
            cv2.imwrite("temp_cam0.jpg", frame0)  # Guarda imagen de la cámara 0
            os.rename("temp_cam0.jpg","frame_cam0.jpg")

        if ret1:
            #frame1 = cv2.rotate(frame1, cv2.ROTATE_180)
            cv2.imwrite("temp_cam1.jpg", frame1)  # Guarda imagen de la cámara 1
            os.rename("temp_cam1.jpg","frame_cam1.jpg")
            print("foto tomada")


except KeyboardInterrupt:
    print("🛑 Captura interrumpida por el usuario.")

finally:
    cap0.release()
    cap1.release()
