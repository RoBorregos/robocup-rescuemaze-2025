import cv2
import numpy as np

# Cargar imagen
frame = cv2.imread("frame_cam1.jpg")

# Verificar si se cargó correctamente
if frame is None:
    print("Error: No se pudo cargar la imagen.")
    exit()

# Convertir a HSV
#hsv = cv2.resize(frame, (224,224))
hsv = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)

redLow1 = np.array([0, 100, 102], np.uint8)
redHigh1 = np.array([10, 255, 255], np.uint8)
redLow2 = np.array([170, 100, 102], np.uint8)
redHigh2 = np.array([179, 255, 255], np.uint8)
yellowLow = np.array([20, 110, 102], np.uint8)
yellowHigh = np.array([35, 255, 255], np.uint8)
greenLow = np.array([36, 80, 102], np.uint8)
greenHigh = np.array([85, 255, 255], np.uint8)

# Crear máscaras para rojo
mask1 = cv2.inRange(hsv, redLow1, redHigh1)
mask2 = cv2.inRange(hsv, redLow2, redHigh2)
mask_red = cv2.bitwise_or(mask1, mask2)
mask_green = cv2.inRange(hsv, greenLow, greenHigh)
mask_yellow = cv2.inRange(hsv, yellowLow, yellowHigh)
# Mostrar máscara
cv2.imshow("Mascara amarilla", mask_yellow)

# Esperar hasta que se presione una tecla
cv2.waitKey(0)
cv2.destroyAllWindows()
