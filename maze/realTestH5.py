import os
import cv2
import tensorflow as tf
import numpy as np
import tensorflow_hub as hub
from tensorflow.keras.utils import custom_object_scope

# Cargar el modelo
with custom_object_scope({'KerasLayer': hub.KerasLayer}):
    model = tf.keras.models.load_model("HSU_detection_mobilenetJetson.h5", compile=False)

# Mapeo de etiquetas
labels = {0: "H", 1: "S", 2: "U", 3: "none"}

# Pipeline GStreamer para cámaras CSI
def _gstreamer_pipeline(sensor_id=0, width=640, height=480, framerate=30):
    return (
        f"nvarguscamerasrc sensor-id={sensor_id} ! "
        f"video/x-raw(memory:NVMM), width={width}, height={height}, format=NV12, framerate={framerate}/1 ! "
        f"nvvidconv flip-method=0 ! video/x-raw, format=BGRx ! "
        f"videoconvert ! video/x-raw, format=BGR ! appsink"
    )

# Abrir ambas cámaras
cap0 = cv2.VideoCapture(_gstreamer_pipeline(0), cv2.CAP_GSTREAMER)
cap1 = cv2.VideoCapture(_gstreamer_pipeline(1), cv2.CAP_GSTREAMER)

if not cap0.isOpened() or not cap1.isOpened():
    print("❌ No se pudieron abrir ambas cámaras.")
    exit()

print("🎥 Cámaras abiertas. Presiona Ctrl+C para detener.")

try:
    while True:
        detections = []

        for i, cap in enumerate([cap0, cap1]):
            ret, frame = cap.read()
            if ret:
                print(f"📸 Frame capturado de cam{i}")

                # Mostrar la imagen si hay entorno gráfico
                if "DISPLAY" in os.environ:
                    window_name = f"Cam{i}"
                    cv2.imshow(window_name, frame)
                    cv2.waitKey(1)

                # Preprocesamiento para predicción
                image = cv2.resize(frame, (224, 224))
                image = np.expand_dims(image.astype(np.float32) / 255.0, axis=0)

                # Predicción
                result = model.predict(image)
                class_id = np.argmax(result)
                label = labels.get(class_id, "unknown")

                print(f"🔍 Detección cam{i}: {label}")
                detections.append((f"cam{i}", label))

            else:
                print(f"⚠️ No se pudo capturar frame de cam{i}")

        # Mostrar todas las detecciones del ciclo
        print("📊 Resultados del ciclo:", detections)

except KeyboardInterrupt:
    print("🛑 Detección detenida por el usuario.")

finally:
    cap0.release()
    cap1.release()
    cv2.destroyAllWindows()
    print("✅ Recursos liberados.")
