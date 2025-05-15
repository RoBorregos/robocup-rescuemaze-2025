#artificial vision model using the google mobilenet that uses imagenet dataset to detect H-S-U letters
import os
import tensorflow_hub as hub
import tensorflow as tf
from tensorflow.keras.preprocessing.image import ImageDataGenerator
from tensorflow.keras.applications import MobileNetV2
from tensorflow.keras.layers import GlobalAveragePooling2D, Dense
from tensorflow.keras.models import Sequential
from tensorflow.keras.callbacks import ReduceLROnPlateau

import numpy as np
#Aumento de datos con ImageData Generator que es una clase de keras que aplica ediciones a las imagenes 
#durante el entrenamiento de manera aleatoria segun los parametros dados 
datagen=ImageDataGenerator(
    rescale=1. / 255,#normalizar
    rotation_range=180,#rotar
    width_shift_range=.25,#mover la imagen vertical y horizontal
    height_shift_range=.25,
    shear_range=0.2,#cizallamiento en imagenes
    zoom_range=[0.5,1],#zoom
    brightness_range=(0.5,1.4),
    validation_split=0.2#20% for test
)
#generadores para sets de entrenamiento y pruebas
#datasets using google images
data_gen_train=datagen.flow_from_directory("C:\\Users\\ferna\\Documents\\robocup-rescuemaze-2025-4\\tmr3Dataset",target_size=(224,224),
                                           batch_size=16,shuffle=True,subset="training")
data_gen_test=datagen.flow_from_directory("C:\\Users\\ferna\\Documents\\robocup-rescuemaze-2025-4\\tmr3Dataset",target_size=(224,224),
                                           batch_size=16,shuffle=True,subset="validation")
#import mobileNet from its url using kerasLayer
mobilenet_url = "https://tfhub.dev/google/tf2-preview/mobilenet_v2/feature_vector/4"

#without keras layer
mobilenetv2 = MobileNetV2(weights="imagenet", include_top=False, input_shape=(224, 224, 3))
mobilenetv2.trainable = False

#model architecture
model = tf.keras.Sequential([
    mobilenetv2,
    GlobalAveragePooling2D(),  
    Dense(4, activation='softmax') 
])
#show model details
#model.summary()
#compile the model 
model.compile(
    optimizer="adam",
    loss="categorical_crossentropy",
    metrics=["accuracy"]
)
#train the model
Epochs=150
history=model.fit(
    data_gen_train,epochs=Epochs,
    validation_data=data_gen_test
)
#save the model
model.save("HSU_FinalQ.h5")



# #artificial vision model using the google mobilenet that uses imagenet dataset to detect H-S-U letters
# import os
# import tensorflow_hub as hub
# import tensorflow as tf
# from tensorflow.keras.preprocessing.image import ImageDataGenerator
# from tensorflow.keras.applications import MobileNetV2
# from tensorflow.keras.layers import GlobalAveragePooling2D, Dense
# from tensorflow.keras.models import Sequential
# import numpy as np
# #Aumento de datos con ImageData Generator que es una clase de keras que aplica ediciones a las imagenes 
# #durante el entrenamiento de manera aleatoria segun los parametros dados 
# datagen=ImageDataGenerator(
#     rescale=1. / 255,#normalizar
#     rotation_range=180,#rotar
#     width_shift_range=.25,#mover la imagen vertical y horizontal
#     height_shift_range=.25,
#     shear_range=0.2,#cizallamiento en imagenes
#     zoom_range=[0.5,1],#zoom
#     horizontal_flip=True,
#     vertical_flip=True,
#     # brightness_range=(0.5,1.4),
#     validation_split=0.2#20% for test
# )
# #generadores para sets de entrenamiento y pruebas
# #datasets using google images
# data_gen_train=datagen.flow_from_directory("C:\\Users\\ferna\\Documents\\robocup-rescuemaze-2025-4\\contourDataset2",target_size=(224,224),
#                                            batch_size=16,shuffle=True,subset="training")
# data_gen_test=datagen.flow_from_directory("C:\\Users\\ferna\\Documents\\robocup-rescuemaze-2025-4\\contourDataset2",target_size=(224,224),
#                                            batch_size=16,shuffle=True,subset="validation")
# #import mobileNet from its url using kerasLayer
# mobilenet_url = "https://tfhub.dev/google/tf2-preview/mobilenet_v2/feature_vector/4"

# #without keras layer
# mobilenetv2 = MobileNetV2(weights="imagenet", include_top=False, input_shape=(224, 224, 3))
# mobilenetv2.trainable = False

# #model architecture
# model = tf.keras.Sequential([
#     mobilenetv2,
#     GlobalAveragePooling2D(),
#     #Dense(64,activation='relu'),
#     Dense(4, activation='softmax')
# ])
# #show model details
# #model.summary()
# #compile the model 
# model.compile(
#     optimizer="adam",
#     loss="categorical_crossentropy",
#     metrics=["accuracy"]
# )
# #train the model
# Epochs=30
# history=model.fit(
#     data_gen_train,epochs=Epochs,
#     validation_data=data_gen_test
# )
# #save the model
# model.save("HSU_FinalPPP.h5")







# #artificial vision model using the google mobilenet that uses imagenet dataset to detect H-S-U letters
# import os
# import tensorflow_hub as hub
# import tensorflow as tf
# from tensorflow.keras.preprocessing.image import ImageDataGenerator
# from tensorflow.keras.applications import MobileNetV2
# from tensorflow.keras.layers import GlobalAveragePooling2D, Dense
# from tensorflow.keras.models import Sequential
# import numpy as np
# print(tf.version)
# #Aumento de datos con ImageData Generator que es una clase de keras que aplica ediciones a las imagenes 
# #durante el entrenamiento de manera aleatoria segun los parametros dados 
# datagen=ImageDataGenerator(
#     rescale=1. / 255,#normalizar
#     rotation_range=180,#rotar
#     width_shift_range=.25,#mover la imagen vertical y horizontal
#     height_shift_range=.25,
#     shear_range=0.15,#cizallamiento en imagenes
#     zoom_range=[0.3,1],#zoom
#     brightness_range=(0.5,1.4),
#     validation_split=0.2#20% for test
# )
# #generadores para sets de entrenamiento y pruebas
# #datasets using google images
# data_gen_train=datagen.flow_from_directory("C:\\Users\\ferna\\Documents\\robocup-rescuemaze-2025-4\\fotos\\grayDataset",target_size=(224,224),
#                                            batch_size=16,shuffle=True,subset="training")
# data_gen_test=datagen.flow_from_directory("C:\\Users\\ferna\\Documents\\robocup-rescuemaze-2025-4\\fotos\\grayDataset",target_size=(224,224),
#                                            batch_size=16,shuffle=True,subset="validation")
# #import mobileNet from its url using kerasLayer
# mobilenet_url = "https://tfhub.dev/google/tf2-preview/mobilenet_v2/feature_vector/4"

# #without keras layer
# mobilenetv2 = MobileNetV2(weights="imagenet", include_top=False, input_shape=(224, 224, 3))
# mobilenetv2.trainable = False

# #model architecture
# model = tf.keras.Sequential([
#     mobilenetv2,
#     GlobalAveragePooling2D(),
#     Dense(4, activation='softmax') 
# ])
# #show model details
# #model.summary()
# #compile the model 
# model.compile(
#     optimizer="adam",
#     loss="categorical_crossentropy",
#     metrics=["accuracy"]
# )
# #train the model
# Epochs=32
# history=model.fit(
#     data_gen_train,epochs=Epochs,
#     validation_data=data_gen_test
# )
# #save the model
# model.save("HSU_detection_Jetson9.h5")

