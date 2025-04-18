import cv2
import numpy as np
import json
import copy

image = cv2.imread("foto_003.jpg")
if image is None:
    raise FileNotFoundError("No se encontró la imagen.")

image = cv2.resize(image, (640, 480))
hsv_image = cv2.cvtColor(image, cv2.COLOR_BGR2HSV)

with open("calibration_hsv.json", "r") as f:
    hsv_ranges = json.load(f)

original_ranges = copy.deepcopy(hsv_ranges)

cv2.namedWindow("Sliders")

def nothing(x):
    pass

# Crear sliders dinámicamente
for color_key in hsv_ranges:
    for i, ch in enumerate(["H", "S", "V"]):
        l_key = f"{color_key}_LOW_{ch}"
        u_key = f"{color_key}_HIGH_{ch}"
        cv2.createTrackbar(l_key, "Sliders", hsv_ranges[color_key]["lower"][i], 255, nothing)
        cv2.createTrackbar(u_key, "Sliders", hsv_ranges[color_key]["upper"][i], 255, nothing)

while True:
    masks = []

    for color_key in hsv_ranges:
        low = []
        high = []
        for i, ch in enumerate(["H", "S", "V"]):
            l_val = cv2.getTrackbarPos(f"{color_key}_LOW_{ch}", "Sliders")
            u_val = cv2.getTrackbarPos(f"{color_key}_HIGH_{ch}", "Sliders")
            low.append(l_val)
            high.append(u_val)
        hsv_ranges[color_key]["lower"] = low
        hsv_ranges[color_key]["upper"] = high

        lower = np.array(low, dtype=np.uint8)
        upper = np.array(high, dtype=np.uint8)
        mask = cv2.inRange(hsv_image, lower, upper)
        masks.append((color_key, mask))

    combined_mask = np.zeros_like(masks[0][1])
    for name, mask in masks:
        combined_mask = cv2.bitwise_or(combined_mask, mask)

    # Mostrar imagen y máscara
    cv2.imshow("Original", image)
    cv2.imshow("Máscara combinada", combined_mask)

    key = cv2.waitKey(1) & 0xFF
    if key == 27:  # ESC
        break
    elif key == ord("s"):  # Guardar si se presiona "s"
        updated = False
        for color_key in hsv_ranges:
            if hsv_ranges[color_key] != original_ranges[color_key]:
                print(f"Guardando cambios para: {color_key}")
                updated = True

        if updated:
            with open("calibration_hsv.json", "w") as f:
                json.dump(hsv_ranges, f, indent=2)
                print("Guardado exitosamente en calibration_hsv.json")
        else:
            print("No se detectaron cambios.")

cv2.destroyAllWindows()
