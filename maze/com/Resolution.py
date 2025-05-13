import cv2

# Función del pipeline GStreamer para cámaras CSI
def gstreamer_pipeline(sensor_id=0):
    return (
        f"nvarguscamerasrc sensor-id={sensor_id} ! "
        f"video/x-raw(memory:NVMM), width=640, height=480, format=NV12, framerate=30/1 ! "
        f"nvvidconv flip-method=0 ! video/x-raw, format=BGRx ! "
        f"videoconvert ! video/x-raw, format=BGR ! appsink drop=true"
    )

# Verifica resolución de una cámara
def verificar_resolucion(sensor_id):
    cap = cv2.VideoCapture(gstreamer_pipeline(sensor_id), cv2.CAP_GSTREAMER)
    
    if not cap.isOpened():
        print(f"❌ No se pudo abrir la cámara {sensor_id}")
        return

    ret, frame = cap.read()
    if ret:
        alto, ancho = frame.shape[:2]
        print(f"📷 Cámara {sensor_id} - Resolución: {ancho}x{alto}")
    else:
        print(f"⚠️ No se pudo leer frame de la cámara {sensor_id}")
    
    cap.release()

# Verifica las dos cámaras (puedes cambiar el rango si tienes más)
for cam_id in range(2):
    verificar_resolucion(cam_id)
