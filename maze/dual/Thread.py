import cv2
import threading
import time

# Shared frame and lock
frame = None
lock = threading.Lock()
running = True

def gstreamer_pipeline(
    capture_width=1280, capture_height=720,
    display_width=1280, display_height=720,
    framerate=30, flip_method=0
):
    return (
        "nvarguscamerasrc ! "
        f"video/x-raw(memory:NVMM), width={capture_width}, height={capture_height}, "
        f"format=NV12, framerate={framerate}/1 ! "
        f"nvvidconv flip-method={flip_method} ! "
        f"video/x-raw, width={display_width}, height={display_height}, format=BGRx ! "
        "videoconvert ! "
        "video/x-raw, format=BGR ! appsink"
    )

def capture_loop():
    global frame, running
    cap = cv2.VideoCapture(gstreamer_pipeline(), cv2.CAP_GSTREAMER)

    if not cap.isOpened():
        print("🚫 Failed to open camera.")
        running = False
        return

    while running:
        ret, img = cap.read()
        if not ret:
            print("❌ Failed to grab frame.")
            break

        with lock:
            frame = img.copy()

    cap.release()

def main():
    global frame, running

    # Start camera thread
    thread = threading.Thread(target=capture_loop)
    thread.start()
    print("🎥 Capture thread started. Press 'q' to quit.")

    # Main display loop
    while running:
        with lock:
            current_frame = frame.copy() if frame is not None else None

        if current_frame is not None:
            cv2.imshow("CSI Camera (Multithreaded)", current_frame)

        if cv2.waitKey(1) & 0xFF == ord('q'):
            print("🛑 Exiting...")
            running = False
            break

    thread.join()
    cv2.destroyAllWindows()

if __name__ == "__main__":
    main()
