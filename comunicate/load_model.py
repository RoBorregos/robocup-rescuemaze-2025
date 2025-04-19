
import os
os.environ["CUDA_VISIBLE_DEVICES"] = "-1"  # Disable all GPUs

import tensorflow as tf
import numpy as np
import matplotlib.pyplot as plt
import tensorflow_hub as hub
from tensorflow.keras.utils import custom_object_scope
from PIL import Image  # Replaces OpenCV
model_path="C:\\Users\\ferna\\Documents\\robocup-rescuemaze-2025-4\\HSU_detection_Jetson3.h5"

class Model():   
    def __init__(self):
        # Load model
        self.model = tf.keras.models.load_model(model_path)
    def getDetection(self):
        # Load image using PIL (instead of OpenCV)
        image_path = "C:\\Users\\ferna\\Documents\\vision_artificial\\fotos\\dataset_HSU\\S\\image7.jpeg"
        try:
            image = Image.open(image_path).convert("RGB")  # Ensure RGB format
        except Exception as e:
            print(f"Error loading image: {e}")
            exit()
        # Normalize image
        image = np.array(image.resize((224, 224)), dtype=np.float32) / 255.0  # Resize and normalize
        # Perform prediction
        result = self.model.predict(image.reshape(1, 224, 224, 3))  # Reshape for model input
        result = np.argmax(result)
        # Label mapping
        labels = {0: 0xA, 1: 0xB, 2: 0xC, 3:0xD}
        detection = labels.get(result, "unknown")
        return detection