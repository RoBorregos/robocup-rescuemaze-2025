import cv2

def gstreamer_pipeline(sensor_id=0, capture_width=1280, capture_height=720, display_width=640, display_height=360, framerate=30, flip_method=0):
    return (
        f"nvarguscamerasrc sensor-id={sensor_id} ! "
        f"video/x-raw(memory:NVMM), width=(int){capture_width}, height=(int){capture_height}, "
        f"format=(string)NV12, framerate=(fraction){framerate}/1 ! "
        f"nvvidconv flip-method={flip_method} ! "
        f"video/x-raw, width=(int){display_width}, height=(int){display_height}, format=(string)BGRx ! "
        f"videoconvert ! "
        f"video/x-raw, format=(string)BGR ! appsink"
    )

# Abrir ambas cámaras CSI
cap0 = cv2.VideoCapture(gstreamer_pipeline(sensor_id=0), cv2.CAP_GSTREAMER)
cap1 = cv2.VideoCapture(gstreamer_pipeline(sensor_id=1), cv2.CAP_GSTREAMER)

if not cap0.isOpened():
    print("❌ No se pudo abrir la cámara 0 (CAM0)")
if not cap1.isOpened():
    print("❌ No se pudo abrir la cámara 1 (CAM1)")

if not cap0.isOpened() and not cap1.isOpened():
    print("🚫 Ninguna cámara disponible. Saliendo.")
    exit()

print("🎥 Mostrando cámaras. Presiona 'q' para salir.")

while True:
    ret0, frame0 = cap0.read()
    ret1, frame1 = cap1.read()

    if ret0:
        cv2.imshow("Camara 0 - CAM0", frame0)
    if ret1:
        cv2.imshow("Camara 1 - CAM1", frame1)

    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

# Liberar recursos
cap0.release()
cap1.release()
cv2.destroyAllWindows()
