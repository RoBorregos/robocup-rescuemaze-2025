import numpy as np
import tensorflow as tf
import tensorflow_hub as hub
from tensorflow.keras.utils import custom_object_scope

MODEL_PATH = "HSU_detection_mobilenetJetson2.h5"

try:
    print("🚀 Loading model:", MODEL_PATH)
    
    with custom_object_scope({'KerasLayer': hub.KerasLayer}):
        model = tf.keras.models.load_model(MODEL_PATH, compile=False)

    print("✅ Model loaded successfully.\n")
    model.summary()

    print("\n🧪 Running dummy prediction...")
    dummy_input = np.random.rand(1, 224, 224, 3).astype(np.float32)
    prediction = model.predict(dummy_input)
    
    print("🔍 Prediction shape:", prediction.shape)
    print("📈 Prediction values:", prediction)

except Exception as e:
    print("❌ Error loading or running model:")
    print(e)
