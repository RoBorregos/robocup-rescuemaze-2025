import cv2
import numpy as np
import tensorflow as tf
from PIL import Image
import os
import matplotlib.pyplot as plt
# Cargar modelo TFLite
# path="HSU_detection_mobilenetJetson_fp16.tflite"
# if not os.path.exists(path):
#     print(f"Error: No se encontró el modelo en ")
#     exit()
def getColor(imageHSV):
    redLow1 = np.array([0, 200, 25], np.uint8)
    redHigh1 = np.array([10, 255, 255], np.uint8)
    redLow2 = np.array([175, 200, 25], np.uint8)
    redHigh2 = np.array([180, 255, 255], np.uint8)
    yellowLow = np.array([25, 150, 25], np.uint8)
    yellowHigh = np.array([35, 255, 255], np.uint8)
    greenLow = np.array([42, 150, 25], np.uint8)
    greenHigh = np.array([72, 255, 255], np.uint8)

    redPixels = cv2.add(
        cv2.inRange(imageHSV, redLow1, redHigh1),
        cv2.inRange(imageHSV, redLow2, redHigh2)
    )
    yellowPixels = cv2.inRange(imageHSV, yellowLow, yellowHigh)
    greenPixels = cv2.inRange(imageHSV, greenLow, greenHigh)

    numRed = cv2.countNonZero(redPixels)
    numYellow = cv2.countNonZero(yellowPixels)
    numGreen = cv2.countNonZero(greenPixels)

    threshold = 0.05 * (224 * 224)

    if numRed > threshold:
        return "reddd"
    elif numYellow > threshold:
        return "yellowwwww"
    elif numGreen > threshold:
        return "greeeeen"
    else:
        return None
interpreter = tf.lite.Interpreter(model_path="C:\\Users\\ferna\\Documents\\robocup-rescuemaze-2025-4\\mobilenetModel\\HSU_detection_mobilenetJetson_fp16.tflite")

interpreter.allocate_tensors()
# Obtener detalles de entrada/salida
input_details = interpreter.get_input_details()
output_details = interpreter.get_output_details()

# Etiquetas
labels = {0: "H", 1: "S", 2: "U", 3: "none"}

# Iniciar la cámara
cap = cv2.VideoCapture(0)

if not cap.isOpened():
    print("No se pudo acceder a la cámara")
    exit()

print("Presiona 'q' para salir...")

while True:
    ret, frame = cap.read()
    if not ret:
        print("No se pudo leer el frame de la cámara.")
        break

    # Preprocesar la imagen para el modelo
    image = cv2.resize(frame, (224, 224))
    image_np = np.expand_dims(image.astype(np.float32) / 255.0, axis=0)


    print(getColor(image))

    # Configurar entrada y ejecutar inferencia
    interpreter.set_tensor(input_details[0]['index'], image_np)
    interpreter.invoke()
    output_data = interpreter.get_tensor(output_details[0]['index'])

    prediction = np.argmax(output_data)
    label = labels.get(prediction, "unknown")

    # Mostrar resultados en la imagen
    #cv2.putText(frame, f"Pred: {label}", (10, 30),
    #            cv2.FONT_HERSHEY_SIMPLEX, 1, (0, 255, 0), 2)

    # Mostrar el frame (solo si no estás en SSH)
    print("Detección:", label)

    # Salir con 'q'
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

cap.release()
cv2.destroyAllWindows()