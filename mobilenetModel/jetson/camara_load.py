
import cv2
import tensorflow as tf
import numpy as np
import matplotlib.pyplot as plt
import tensorflow_hub as hub
from tensorflow.keras.utils import custom_object_scope
# from PIL import Image  # Replaces OpenCV

# Load model
with custom_object_scope({'KerasLayer': hub.KerasLayer}):
    model = tf.keras.models.load_model("HSU_detection_mobilenetJetson.h5")


def _gstreamer_pipeline(sensor_id=0, width=640, height=480, framerate=30):
    return (
        f"nvarguscamerasrc sensor-id={sensor_id} ! "
        f"video/x-raw(memory:NVMM), width={width}, height={height}, format=NV12, framerate={framerate}/1 ! "
        f"nvvidconv flip-method=0 ! video/x-raw, format=BGRx ! "
        f"videoconvert ! video/x-raw, format=BGR ! appsink"
    )



# Pipelines CSI para Jetson Nano
cam = _gstreamer_pipeline(sensor_id=0)

cap = cv2.VideoCapture(cam, cv2.CAP_GSTREAMER)
# Verificar si la cámara se abrió correctamente
if not cam.isOpened():
    print("❌ No se pudo abrir la cámara CSI.")
    exit()
while(True):
    # Capturar una imagen
    ret, frame = cap.read()
    if ret:
        # Mostrar la imagen capturada en una ventana
        cv2.imshow("Foto Capturada", frame)
        # Esperar hasta que el usuario presione una tecla para continuar
        cv2.waitKey(2000)
    else:
        print("❌ No se pudo capturar la imagen.")

    # Liberar la cámara y cerrar ventanas
    cap.release()
    cv2.destroyAllWindows()
    image=frame
    # Normalize image
    image = cv2.resize(frame, (224, 224))
    image_np = np.expand_dims(image.astype(np.float32) / 255.0, axis=0)
    # # Show image using Matplotlib instead of OpenCV
    # plt.imshow(image)
    # plt.axis("off")  # Hide axis
    # plt.show()

    # Perform prediction
    result = model.predict(image.reshape(1, 224, 224, 3))  # Reshape for model input
    result = np.argmax(result)

    # Label mapping
    labels = {0: "H", 1: "S", 2: "U", 3: "none"}
    detection = labels.get(result, "unknown")

    print(detection)




