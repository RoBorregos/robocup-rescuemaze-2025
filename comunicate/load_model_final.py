import cv2
import numpy as np
import tensorflow as tf
from PIL import Image

class Model():
    def __init__(self):
        # Cargar modelo TFLite
        self.interpreter = tf.lite.Interpreter(model_path="HSU_detection_mobilenetJetson_fp16.tflite")
        self.interpreter.allocate_tensors()

        # Obtener detalles de entrada/salida
        self.input_details = self.interpreter.get_input_details()
        self.output_details = self.interpreter.get_output_details()

        # Etiquetas
        self.labels = {0: 0xA, 1: 0xB, 2: 0xC, 3: 0xD}

        # Iniciar la cámara
        self.cap = cv2.VideoCapture(0)
        if not self.cap.isOpened():
            print("No se pudo acceder a la cámara")
            exit()

        print("Presiona 'q' para salir...")

    def getDetection(self):
        ret, frame = self.cap.read()
        if not ret:
            print("No se pudo leer el frame de la cámara.")
            return None

        # Preprocesar la imagen
        image = cv2.resize(frame, (224, 224))
        image_np = np.expand_dims(image.astype(np.float32) / 255.0, axis=0)

        # Inferencia
        self.interpreter.set_tensor(self.input_details[0]['index'], image_np)
        self.interpreter.invoke()
        output_data = self.interpreter.get_tensor(self.output_details[0]['index'])

        prediction = np.argmax(output_data)
        label = self.labels.get(prediction, "unknown")

        print("Detección:", label)

        # Salir con 'q'
        if cv2.waitKey(1) & 0xFF == ord('q'):
            return "exit"

        return label

    def close(self):
        self.cap.release()
        cv2.destroyAllWindows()
