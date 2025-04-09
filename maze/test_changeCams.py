import cv2

cap = cv2.VideoCapture("/dev/video0")  # o video1 si es USB

if not cap.isOpened():
    print("No se pudo abrir la cámara")
else:
    ret, frame = cap.read()
    if ret:
        cv2.imwrite("test.jpg", frame)
        print("✅ Captura guardada en test.jpg")
    cap.release()
