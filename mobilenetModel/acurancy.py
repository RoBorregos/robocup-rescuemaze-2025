import os
import tensorflow as tf
import numpy 
import cv2
import matplotlib.pyplot as plt
import tensorflow_datasets as tfd
from tensorflow.keras.utils import custom_object_scope
import tensorflow_hub as hub
import numpy as np
model = tf.keras.models.load_model("C:\\Users\\ferna\\Documents\\robocup-rescuemaze-2025-4\\HSU_FinalP.h5")
# folder_path="C:\\Users\\ferna\\Documents\\robocup-rescuemaze-2025-4\\fotos\\test1\\H2"
folder_path="C:\\Users\\ferna\\Downloads\\none5"

image_list=os.listdir(folder_path)
no_images=0
counter=0
#contar elementos
for i in image_list:    
    no_images+=1
    print(no_images)
for image_name in image_list:
    #carga de imagen
    image_path=os.path.join(folder_path,image_name)
    try:
        image=cv2.imread(image_path)
        # if image is None:
        #     print("error al cargar la imagen")
    except:
        print("error al cargar la imagen")
    #normalizar imagen
    # image=cv2.cvtColor(image,cv2.COLOR_RGB2GRAY)
    # image=cv2.cvtColor(image,cv2.COLOR_GRAY2RGB)
    image = numpy.array(image).astype(float)/255
    image = cv2.resize(image, (224,224))
    #realizar prediccion
    try:
        result = model.predict(image.reshape(1, 224, 224, 3))
    except:
        print("error")
    print(result)
    max_prob = np.max(result)
    if max_prob < 0.75:
        result = 3
    else:
        result = np.argmax(result)
    if result==3:
        counter+=1
    else:
        plt.imshow(image)
        plt.axis("off")  # Hide axis
        plt.show()
acurancy=counter/no_images
print(acurancy)


# 95 94 NONE
# 75  90  S
# 65  84  U
# 73  84  H





# H 86
# S 96
# U 95
# none 1 

# TEST2 trackfotos
# Jetson 9
# H 0.946
# S 0.97
# U 0.0.99



# Jetson 8 sin y con rango de confianza 75%
# H 0.99  0.96
# S 1      1
# U 0.961  0.72
# none     1


# Jetson 7 
# H 0.94
# S 1
# U 0.9923 con none 0.37





# TEST1
# Jetson 9
# H 0.62
# S 0.77
# U 0.0.99


# Jetson 8
# H 0.93 0.875
# S 1   0.88
# U 0.37 


# Jetson 7
# H 0.875   
# S 0.88
# U 0.5 con none 0.37




# Jetson 6
# H 0.9375
# S 0.88
# U 0.25

# S
# [[0.02156101 0.5959984  0.29641852 0.08602204]]
# [[0.00646951 0.98300236 0.00651644 0.00401166]]
# [[1.2471776e-02 9.0749687e-01 7.9460539e-02 5.7085307e-04]]
# [[0.02156101 0.5959984  0.29641852 0.08602204]]
# [[0.13192424 0.7848428  0.07614941 0.00708358]]
# [[0.291014   0.31557277 0.13514264 0.2582706 ]]
# [[3.2494075e-03 9.9186206e-01 4.8571425e-03 3.1392861e-05]]
# [[8.2256878e-03 9.7690338e-01 1.4812368e-02 5.8643189e-05]]
# [[0.03144523 0.40498284 0.54087615 0.0226958 ]]XX U
# 0.8888888888888888