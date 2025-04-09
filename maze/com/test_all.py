# test_model.py

import time
from load_model import Model  # Asegúrate de que este archivo esté en el mismo directorio

if __name__ == "__main__":
    try:
        model = Model()
        print("🔁 Iniciando detección. Presiona Ctrl+C para salir.")
        while True:
            label = model.getDetection()
            print(f"📸 Detección: {hex(label)}")
            time.sleep(1)
    except KeyboardInterrupt:
        print("\n🛑 Detección interrumpida por el usuario.")
    finally:
        model.close()
