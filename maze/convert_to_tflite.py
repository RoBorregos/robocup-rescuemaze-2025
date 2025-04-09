import tensorflow as tf
import tensorflow_hub as hub
from tensorflow.keras.utils import custom_object_scope

# Load model with custom layer
with custom_object_scope({'KerasLayer': hub.KerasLayer}):
    model = tf.keras.models.load_model("HSU_detection_mobilenetJetson.h5")

# Convert to TFLite with float16 quantization
converter = tf.lite.TFLiteConverter.from_keras_model(model)
converter.optimizations = [tf.lite.Optimize.DEFAULT]
converter.target_spec.supported_types = [tf.float16]

tflite_model = converter.convert()

# Save the TFLite model
with open("HSU_detection_mobilenetJetson_fp16.tflite", "wb") as f:
    f.write(tflite_model)

print("Successfully converted to TFLite (float16) with custom KerasLayer.")
