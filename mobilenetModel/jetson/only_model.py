import tensorflow as tf
import numpy as np
import matplotlib.pyplot as plt
import tensorflow_hub as hub
from tensorflow.keras.utils import custom_object_scope
from PIL import Image  # Replaces OpenCV
# Limitar el uso de memoria en la GPU
gpus = tf.config.experimental.list_physical_devices('GPU')
if gpus:
    try:
        for gpu in gpus:
            tf.config.experimental.set_memory_growth(gpu, True)
        print("✅ Memoria GPU ajustada para evitar OOM.")
    except RuntimeError as e:
        print(e)
# Load model
with custom_object_scope({'KerasLayer': hub.KerasLayer}):
    model = tf.keras.models.load_model("HSU_detection_mobilenetJetson.h5")

# Load image using PIL (instead of OpenCV)
image_path = "images7.jpg"
try:
    image = Image.open(image_path).convert("RGB")  # Ensure RGB format
except Exception as e:
    print(f"Error loading image: {e}")
    exit()

# Normalize image
image = np.array(image.resize((224, 224)), dtype=np.float32) / 255.0  # Resize and normalize

# Show image using Matplotlib instead of OpenCV
plt.imshow(image)
plt.axis("off")  # Hide axis
plt.show()

# Perform prediction
result = model.predict(image.reshape(1, 224, 224, 3))  # Reshape for model input
result = np.argmax(result)

# Label mapping
labels = {0: "H", 1: "S", 2: "U", 3: "none"}
detection = labels.get(result, "unknown")

print(detection)