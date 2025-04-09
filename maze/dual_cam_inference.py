import cv2
import tensorflow as tf
import numpy as np
from PIL import Image

# GStreamer pipelines para las dos cámaras CSI
pipelines = {
    "cam0": (
        "nvarguscamerasrc sensor-id=0 ! "
        "video/x-raw(memory:NVMM), width=1280, height=720, format=NV12, framerate=30/1 ! "
        "nvvidconv flip-method=0 ! "
        "video/x-raw, format=BGRx ! "
        "videoconvert ! "
        "video/x-raw, format=BGR ! appsink"
    ),
    "cam1": (
        "nvarguscamerasrc sensor-id=1 ! "
        "video/x-raw(memory:NVMM), width=1280, height=720, format=NV12, framerate=30/1 ! "
        "nvvidconv flip-method=0 ! "
        "video/x-raw, format=BGRx ! "
        "videoconvert ! "
        "video/x-raw, format=BGR ! appsink"
    ),
}

# Etiquetas del modelo
labels = {0: "H", 1: "S", 2: "U", 3: "none"}

# Cargar modelo TFLite
interpreter = tf.lite.Interpreter("HSU_detection_mobilenetJetson_fp16.tflite")
interpreter.allocate_tensors()
input_details = interpreter.get_input_details()
output_details = interpreter.get_output_details()

def detect_from_camera(sensor_name, pipeline):
    print(f"🎥 Capturando desde {sensor_name}...")
    cap = cv2.VideoCapture(pipeline, cv2.CAP_GSTREAMER)

    if not cap.isOpened():
        print(f"❌ No se pudo capturar imagen desde {sensor_name}")
        return "none"

    ret, frame = cap.read()
    cap.release()

    if not ret:
        print(f"❌ No se pudo leer el frame desde {sensor_name}")
        return "none"

    # 🔄 Rotar imagen 180 grados
    frame = cv2.rotate(frame, cv2.ROTATE_180)

    # Preprocesar imagen
    image = cv2.resize(frame, (224, 224))
    image_np = np.expand_dims(image.astype(np.float32) / 255.0, axis=0)

    # Ejecutar inferencia
    interpreter.set_tensor(input_details[0]["index"], image_np)
    interpreter.invoke()
    output_data = interpreter.get_tensor(output_details[0]["index"])

    prediction = np.argmax(output_data)
    label = labels.get(prediction, "none")
    print(f"🔍 {sensor_name} → Detección: {label}")
    return label

# Ejecutar detección en ambas cámaras
detections = {}
for name, pipeline in pipelines.items():
    label = detect_from_camera(name, pipeline)
    detections[name] = label

# 🧠 Priorizar cam0 si tiene una detección válida
final_detection = (
    detections["cam0"] if detections["cam0"] != "none" else detections["cam1"]
)
print(f"\n✅ Resultado final: {final_detection}")
