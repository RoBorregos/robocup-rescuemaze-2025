import cv2
import numpy as np
import json
def calibrate_color(image_bgr, roi):
    x,y,w,h = roi
    roi_img = image_bgr[y:y+h, x:x+w]
    roi_hsv = cv2.cvtColor(roi_img, cv2.COLOR_BGR2HSV)
    h_mean = np.mean(roi_hsv[:, :, 0])
    s_mean = np.mean(roi_hsv[:,:,1])
    v_mean = np.mean(roi_hsv[:,:,2])
    h_margin, s_margin, v_margin = 10, 40, 40

    lower = [

        int(max(0, h_mean - h_margin)),
        int(max(30, s_mean - s_margin)),
        int(max(30, v_mean - v_margin))
    ]
    upper = [
        int(min(180, h_mean + h_margin)),
        int(min(255, s_mean + s_margin)),
        int(min(255, v_mean + v_margin))
    ]
    return lower, upper


def main():
    img_path = "frame_cam0.jpg"
    image = cv2.imread(img_path)

    if image is None:
        print("Imagen no encontrada.")
        return

    colors = ["H", "S", "U"]
    hsv_ranges ={}

    for color_name in colors:
        print(f" Selecciona zona para color '{color_name}' y presiona Enter.")
        roi = cv2.selectROI(f"Selecciona {color_name}", image, fromCenter =False, showCrosshair=True)
        cv2.destroyWindow(f"Selecciona {color_name}")
        if roi == (0,0,0,0):
            print("zona no seleccionada")
        lower, upper = calibrate_color(image,roi)
        hsv_ranges[color_name] = {"lower": lower, "upper": upper}
        print(f"{color_name}: lower = {lower}, upper = {upper}")
    with open("calibration_hsv.json", "w") as f:
        json.dump(hsv_ranges, f, indent = 2)
        print("Calibracion guardad en calibration_hsv.json")

if __name__ == "__main__":
    main()
