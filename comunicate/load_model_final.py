import cv2
import numpy as np
import tensorflow as tf
from PIL import Image
class Model():
    def __init__(self):
        # Cargar modelo TFLite
        interpreter = tf.lite.Interpreter(model_path="HSU_detection_mobilenetJetson_fp16.tflite")
        interpreter.allocate_tensors()
        
        # Obtener detalles de entrada/salida
        input_details = interpreter.get_input_details()
        output_details = interpreter.get_output_details()
        
        # Etiquetas
        labels = {0: 0xA, 1: 0xB, 2: 0xC, 3:0xD}
        
        # Iniciar la cámara
        cap = cv2.VideoCapture(0)
        if not cap.isOpened():
            print("No se pudo acceder a la cámara")
            exit()
        
        print("Presiona 'q' para salir...")
    def getDetection(self):
        ret, frame = cap.read()
        if not ret:
            print("No se pudo leer el frame de la cámara.")
            break
    
        # Preprocesar la imagen para el modelo
        image = cv2.resize(frame, (224, 224))
        image_np = np.expand_dims(image.astype(np.float32) / 255.0, axis=0)
    
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
        return label
    cap.release()
    cv2.destroyAllWindows()
