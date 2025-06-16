
import os
os.environ["CUDA_VISIBLE_DEVICES"] = "-1"  # Disable all GPUs

import tensorflow as tf
import numpy as np
import matplotlib.pyplot as plt
from PIL import Image  # Replaces OpenCV
import cv2
# Load model
model = tf.keras.models.load_model("C:\\Users\\ferna\\Documents\\robocup-rescuemaze-2025-4\\HSU_detection_Jetson8.h5")

# Load image using PIL (instead of OpenCV)
image_path = "C:\\Users\\ferna\\Documents\\robocup-rescuemaze-2025-4\\fotos\\Ndataset\\H\\foto_029.jpg"

try:
    image=cv2.imread(image_path)
    # image = Image.open(image_path).convert("RGB")  # Ensure RGB format
except Exception as e:
    print(f"Error loading image: {e}")
    exit()

# Normalize image
# image = np.array(image.resize((224, 224)), dtype=np.float32) / 255.0  # Resize and normalize

image = np.array(image).astype(float)/255
image = cv2.resize(image, (224,224))
# Show image using Matplotlib instead of OpenCV
plt.imshow(image)
plt.axis("off")  # Hide axis
plt.show()

# Perform prediction
result = model.predict(image.reshape(1, 224, 224, 3))  # Reshape for model input
print(result)
result = np.argmax(result)

# Label mapping
labels = {0: "H", 1: "S", 2: "U", 3: "none"}
detection = labels.get(result, "unknown")

print(detection)


