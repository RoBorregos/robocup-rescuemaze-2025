import cv2
import os
import time

# Configura el pipeline de GStreamer
def gstreamer_pipeline(sensor_id=0):
    return (
        f"nvarguscamerasrc sensor-id={sensor_id} ! "
        f"video/x-raw(memory:NVMM), width=640, height=480, format=NV12, framerate=30/1 ! "
        f"nvvidconv flip-method=0 ! video/x-raw, format=BGRx ! "
        f"videoconvert ! video/x-raw, format=BGR ! appsink drop=true"
    )

# Crear carpeta de salida
output_dir = "U2"
os.makedirs(output_dir, exist_ok=True)

# Inicializa la cámara
cap = cv2.VideoCapture(gstreamer_pipeline(0), cv2.CAP_GSTREAMER)

if not cap.isOpened():
    print("❌ No se pudo abrir la cámara.")
    exit()

print("📸 Tomando una foto automática cada 1 segundo. Usa Ctrl+C para detener.")

counter = 0
try:
    while True:
        ret, frame = cap.read()
        if not ret:
            print("⚠️ No se pudo leer el frame.")
            continue

        frame_rotated = cv2.rotate(frame, cv2.ROTATE_180)

        filename = os.path.join(output_dir, f"foto_{counter:03d}.jpg")
        cv2.imwrite(filename, frame_rotated)
        print(f"✅ Foto {counter} guardada como: {filename}")
        counter += 1

        time.sleep(0.2)  # Espera 1 segundo entre capturas

except KeyboardInterrupt:
    print("🛑 Captura interrumpida por el usuario.")

finally:
    cap.release()
    cv2.destroyAllWindows()
