import cv2
import numpy as np
import tensorflow as tf

class Model():
    def __init__(self):
        # Cargar modelo TFLite
        self.interpreter = tf.lite.Interpreter(model_path="HSU_detection_mobilenetJetson_fp16.tflite")
        self.interpreter.allocate_tensors()

        # Obtener detalles de entrada/salida
        self.input_details = self.interpreter.get_input_details()
        self.output_details = self.interpreter.get_output_details()

        # Etiquetas
        self.labels = {0: 0xA, 1: 0xB, 2: 0xC, 3: 0xD}  # H, S, U, none

        # Iniciar la cámara
        self.cap = cv2.VideoCapture(0)
        if not self.cap.isOpened():
            print("No se pudo acceder a la cámara")
            exit()

    def getColor(self, imageHSV):
        # Rangos de colores
        redLow1 = np.array([0, 200, 25], np.uint8)
        redHigh1 = np.array([10, 255, 255], np.uint8)
        redLow2 = np.array([175, 200, 25], np.uint8)
        redHigh2 = np.array([180, 255, 255], np.uint8)
        yellowLow = np.array([25, 150, 25], np.uint8)
        yellowHigh = np.array([35, 255, 255], np.uint8)
        greenLow = np.array([42, 150, 25], np.uint8)
        greenHigh = np.array([72, 255, 255], np.uint8)

        redPixels1 = cv2.inRange(imageHSV, redLow1, redHigh1)
        redPixels2 = cv2.inRange(imageHSV, redLow2, redHigh2)
        redPixels = cv2.add(redPixels1, redPixels2)
        yellowPixels = cv2.inRange(imageHSV, yellowLow, yellowHigh)
        greenPixels = cv2.inRange(imageHSV, greenLow, greenHigh)

        numRed = cv2.countNonZero(redPixels)
        numYellow = cv2.countNonZero(yellowPixels)
        numGreen = cv2.countNonZero(greenPixels)

        threshold = 5 * (224 * 224) / 100  # 5% 

        if numRed > threshold:
            return 0xA  # Harmed → 0xA
        elif numYellow > threshold:
            return 0xB  # Stable → 0xD
        elif numGreen > threshold:
            return 0xC  # Unharmed → 0xB
        else:
            return None  # Sin color predominante

    def getDetection(self):
        ret, frame = self.cap.read()
        if not ret:
            print("No se pudo leer el frame.")
            return 0xD  # Default: "none"

        frame_resized = cv2.resize(frame, (224, 224))
        hsv = cv2.cvtColor(frame_resized, cv2.COLOR_BGR2HSV)

        color = self.getColor(hsv)
        if color is not None:
            print("Color detectado:", color)
            return color

        image_np = np.expand_dims(frame_resized.astype(np.float32) / 255.0, axis=0)

        self.interpreter.set_tensor(self.input_details[0]['index'], image_np)
        self.interpreter.invoke()
        output_data = self.interpreter.get_tensor(self.output_details[0]['index'])

        prediction = np.argmax(output_data)
        label = self.labels.get(prediction, 0xD) 
        print("Modelo detectó:", label)
        return label

    def close(self):
        self.cap.release()
        cv2.destroyAllWindows()
