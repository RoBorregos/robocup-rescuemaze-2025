import cv2
# Función para crear el pipeline de GStreamer para cámaras CSI
def gstreamer_pipeline(sensor_id=0, capture_width=1280, capture_height=720, display_width=640, display_height=480, framerate=30, flip_method=0):
    return (
        f"nvarguscamerasrc sensor-id={sensor_id} ! "
        f"video/x-raw(memory:NVMM), width=(int){capture_width}, height=(int){capture_height}, framerate=(fraction){framerate}/1 ! "
        f"nvvidconv flip-method={flip_method} ! "
        f"video/x-raw, width=(int){display_width}, height=(int){display_height}, format=(string)BGRx ! "
        f"videoconvert ! "
        f"video/x-raw, format=(string)BGR ! appsink"
    )
# Abrir la cámara CSI 0
cam = cv2.VideoCapture(gstreamer_pipeline(sensor_id=0), cv2.CAP_GSTREAMER)
# Verificar si la cámara se abrió correctamente
if not cam.isOpened():
    print("❌ No se pudo abrir la cámara CSI.")
    exit()
while(True):
    # Capturar una imagen
    ret, frame = cam.read()
    if ret:
        # Mostrar la imagen capturada en una ventana
        cv2.imshow("Foto Capturada", frame)
        # Esperar hasta que el usuario presione una tecla para continuar
        cv2.waitKey(2000)
    else:
        print("❌ No se pudo capturar la imagen.")
