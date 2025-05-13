from model_class2 import Model
import time

m=Model()

while True:
    detection=m.getDetectionRight()
    print(detection)
    time.sleep(0.3)
