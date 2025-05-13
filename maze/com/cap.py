import cv2
import time

def gstreamer_pipeline(sensor_id=0):
    return (
        f"nvarguscamerasrc sensor-id={sensor_id} ! "
        f"video/x-raw(memory:NVMM), width=640, height=480, format=NV12, framerate=30/1 ! "
        f"nvvidconv flip-method=0 ! video/x-raw, format=BGRx ! "
        f"videoconvert ! video/x-raw, format=BGR ! appsink drop=true"
    )

cap = cv2.VideoCapture(gstreamer_pipeline(0), cv2.CAP_GSTREAMER)

if not cap.isOpened():
    print("❌ No se pudo abrir la cámara CSI")
    exit()

print("Presiona ESPACIO para capturar imagen. Presiona ESC para salir.")
img_count = 0

while True:
    ret, frame = cap.read()
    if not ret:
        print("❌ No se pudo leer el frame.")
        break

    #cv2.imshow("Cam0", frame)
    key = cv2.waitKey(1) & 0xFF

    if key == 27:  # ESC
        break
    elif key == 32:  # SPACE
        filename = f"capture_{img_count:03d}.jpg"
        cv2.imwrite(filename, frame)
        print(f"✅ Imagen guardada: {filename}")
        img_count += 1

cap.release()
cv2.destroyAllWindows()
