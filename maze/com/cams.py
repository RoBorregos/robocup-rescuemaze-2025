import cv2
import time

def gstreamer_pipeline(sensor_id=0):
    return (
        f"nvarguscamerasrc sensor-id={sensor_id} ! "
        f"video/x-raw(memory:NVMM), width=640, height=480, format=NV12, framerate=30/1 ! "
        f"nvvidconv flip-method=0 ! video/x-raw, format=BGRx ! "
        f"videoconvert ! video/x-raw, format=BGR ! appsink"
    )

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
            frame0 = cv2.rotate(frame0, cv2.ROTATE_180)
            cv2.imshow("Camara 0", frame0)

        if ret1:
            frame1 = cv2.rotate(frame1, cv2.ROTATE_180)
            cv2.imshow("Camara 1", frame1)

        # Salir con 'q'
        if cv2.waitKey(1) & 0xFF == ord('q'):
            break

except KeyboardInterrupt:
    print("🛑 Captura interrumpida por el usuario.")

finally:
    cap0.release()
    cap1.release()
    cv2.destroyAllWindows()
