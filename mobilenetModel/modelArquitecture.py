#artificial vision model using the google mobilenet that uses imagenet dataset to detect H-S-U letters
import tensorflow_hub as hub
import tensorflow as tf
from keras.preprocessing.image import ImageDataGenerator
import numpy as np
print(tf.version)
#Aumento de datos con ImageData Generator que es una clase de keras que aplica ediciones a las imagenes 
#durante el entrenamiento de manera aleatoria segun los parametros dados 
datagen=ImageDataGenerator(
    rescale=1. / 255,#normalizar
    rotation_range=180,#rotar
    width_shift_range=.25,#mover la imagen vertical y horizontal
    height_shift_range=.25,
    shear_range=0.15,#cizallamiento en imagenes
    zoom_range=[0.5,1.5],#zoom
    validation_split=0.2#20% for test
)
#generadores para sets de entrenamiento y pruebas
#datasets using google images
data_gen_train=datagen.flow_from_directory("C:\\Users\\ferna\\Documents\\vision_artificial\\fotos\dataset_HSU",target_size=(224,224),
                                           batch_size=32,shuffle=True,subset="training")
data_gen_test=datagen.flow_from_directory("C:\\Users\\ferna\\Documents\\vision_artificial\\fotos\dataset_HSU",target_size=(224,224),
                                           batch_size=32,shuffle=True,subset="validation")
#import mobileNet from its url using kerasLayer
mobilenet_url="https://www.kaggle.com/models/google/mobilenet-v2/TensorFlow2/tf2-preview-feature-vector/4"
mobilenetv2 = hub.KerasLayer(mobilenet_url, input_shape=(224,224,3))
mobilenetv2.trainable = False
#model architecture
model = tf.keras.Sequential([
    mobilenetv2,
    tf.keras.layers.Dense(4, activation='softmax')
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
Epochs=50
history=model.fit(
    data_gen_train,epochs=Epochs,batch_size=32,
    validation_data=data_gen_test
)
#save the model
model.save("HSU_detection_mobilenetv4.h5")
