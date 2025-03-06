# # from Comunication import Esp32
# import Constants 
# from Esp_Comunication import Esp32
# import time

# robot=Esp32()
# robot.connect()
# while(True):
#     confirmation=robot.sent_harmed_victim()
#     print(confirmation)

# import tensorflow_hub as hub
# model = hub.load("https://tfhub.dev/google/tf2-preview/mobilenet_v2/feature_vector/4")
# print("Modelo cargado correctamente.")

import os
os.environ["CUDA_VISIBLE_DEVICES"] = "-1"

import tensorflow as tf
print("Dispositivos disponibles:", tf.config.list_physical_devices())  # Verifica que solo use CPU
