import numpy as np
import tensorflow as tf
import tensorflow_hub as hub
from tensorflow.keras.utils import custom_object_scope

try:
    print("🚀 Trying to load model with compile=False...")
    with custom_object_scope({'KerasLayer': hub.KerasLayer}):
        model = tf.keras.models.load_model("HSU_detection_mobilenetJetson.h5", compile=False)

    model.summary()

    print("✅ Model loaded. Testing dummy input...")
    dummy_input = np.random.rand(1, 224, 224, 3).astype(np.float32)
    prediction = model.predict(dummy_input)
    print("🔍 Prediction shape:", prediction.shape)

except Exception as e:
    print("❌ Model is NOT compatible:")
    print(e)
