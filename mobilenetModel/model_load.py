
# import os
# os.environ["CUDA_VISIBLE_DEVICES"] = "-1"  # Disable all GPUs

import tensorflow as tf
import numpy as np
import matplotlib.pyplot as plt
import tensorflow_hub as hub
from tensorflow.keras.utils import custom_object_scope
from PIL import Image  # Replaces OpenCV

# Load model
# with custom_object_scope({'KerasLayer': hub.KerasLayer}):
#     model = tf.keras.models.load_model("C:\\Users\\ferna\\Documents\\robocup-rescuemaze-2025-4\\mobilenetModel\\HSU_detection_mobilenetJetson.h5")

model = tf.keras.models.load_model("C:\\Users\\ferna\\Documents\\robocup-rescuemaze-2025-4\\HSU_FinalP6.h5")
print("modelo cargado")
# # Load image using PIL (instead of OpenCV)
# image_path = "C:\\Users\\ferna\\Documents\\robocup-rescuemaze-2025-4\\trackFotos\\H\\foto_000.jpg"
image_path = "C:\\Users\\ferna\\Downloads\\h4.jpg"

try:
    image = Image.open(image_path).convert("RGB")  # Ensure RGB format
except Exception as e:
    print(f"Error loading image: {e}")
    exit()

# Normalize image
image = np.array(image.resize((224, 224)), dtype=np.float32) / 255.0  # Resize and normalize

# # Show image using Matplotlib instead of OpenCV
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
