import cv2
import numpy as np

def get_color_name(image):
    hsv = cv2.cvtColor(image, cv2.COLOR_BGR2HSV)

    redLow1 = np.array([0, 120, 70])
    redHigh1 = np.array([10, 255, 255])
    redLow2 = np.array([170, 120, 70])
    redHigh2 = np.array([180, 255, 255])

    yellowLow = np.array([20, 100, 100])
    yellowHigh = np.array([35, 255, 255])

    greenLow = np.array([36, 80, 80])
    greenHigh = np.array([89, 255, 255])

    redMask = cv2.inRange(hsv, redLow1, redHigh1) + cv2.inRange(hsv, redLow2, redHigh2)
    yellowMask = cv2.inRange(hsv, yellowLow, yellowHigh)
    greenMask = cv2.inRange(hsv, greenLow, greenHigh)

    redCount = np.count_nonzero(redMask)
    yellowCount = np.count_nonzero(yellowMask)
    greenCount = np.count_nonzero(greenMask)

    threshold = 0.01 * (image.shape[0] * image.shape[1])

    if redCount > threshold:
        return "🔴 RED"
    elif yellowCount > threshold:
        return "🟡 YELLOW"
    elif greenCount > threshold:
        return "🟢 GREEN"
    else:
        return "⚪️ NONE"

def gstreamer_pipeline(sensor_id=0, capture_width=1280, capture_height=720, display_width=224, display_height=224, framerate=30):
    return (
        f"nvarguscamerasrc sensor-id={sensor_id} ! "
        f"video/x-raw(memory:NVMM), width={capture_width}, height={capture_height}, "
        f"format=NV12, framerate={framerate}/1 ! "
        f"nvvidconv flip-method=2 ! "
        f"video/x-raw, width={display_width}, height={display_height}, format=BGRx ! "
        f"videoconvert ! video/x-raw, format=BGR ! appsink"
    )

cam0 = cv2.VideoCapture(gstreamer_pipeline(0), cv2.CAP_GSTREAMER)
cam1 = cv2.VideoCapture(gstreamer_pipeline(1), cv2.CAP_GSTREAMER)

if not cam0.isOpened() or not cam1.isOpened():
    print("❌ No se pudieron abrir una o ambas cámaras")
    exit()

print("✅ Cámaras abiertas. Presiona Ctrl+C para salir.")

try:
    while True:
        ret0, frame0 = cam0.read()
        ret1, frame1 = cam1.read()

        if not ret0 or not ret1:
            print("❌ Error capturando imagen")
            break

        # Rota las imágenes 180°
        frame0 = cv2.rotate(frame0, cv2.ROTATE_180)
        frame1 = cv2.rotate(frame1, cv2.ROTATE_180)

        color0 = get_color_name(frame0)
        color1 = get_color_name(frame1)

        print(f"[Cam0] {color0} | [Cam1] {color1}")

except KeyboardInterrupt:
    print("🛑 Detección interrumpida por el usuario")

cam0.release()
cam1.release()
