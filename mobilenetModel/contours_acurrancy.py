import os
import tensorflow as tf
import numpy as np 
import cv2

model = tf.keras.models.load_model("C:\\Users\\ferna\\Documents\\robocup-rescuemaze-2025-4\\HSU_FinalP.h5")
folder_path="C:\\Users\\ferna\\Documents\\robocup-rescuemaze-2025-4\\FotosMaze\\U"
# folder_path="C:\\Users\\ferna\\Downloads\\none1"

image_list=os.listdir(folder_path)
no_images=0
counter=0
#contar elementos
for i in image_list:    
    no_images+=1
    print(no_images)
for image_name in image_list:
    if image_name.startswith("._"):
            image_name=image_name[2:]
    #carga de imagen
    image_path=os.path.join(folder_path,image_name)
    img=cv2.imread(image_path)
    if img is None:
        print("error al cargar la imagen")
    # conver to gray scale
    img=cv2.resize(img,(1000,1000))
    gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)

    # create lights mask 
    _, mask_lights = cv2.threshold(gray, 180, 255, cv2.THRESH_BINARY)
    kernel = np.ones((15, 15), np.uint8)
    mask_lights = cv2.dilate(mask_lights, kernel, iterations=1)
    mask_no_lights = cv2.bitwise_not(mask_lights)
    # Find edges
    edges = cv2.Canny(gray, threshold1=20, threshold2=50)  
    # edges = cv2.bitwise_and(edges, edges, mask=mask_no_lights)
    # Expand edges
    kernel=np.ones((10,10),np.uint8)
    edges=cv2.dilate(edges,kernel,iterations=1)
    # Find Contours
    contours, _=cv2.findContours(edges,cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)
    contours = [c for c in contours if cv2.contourArea(c) > 4700]#delete small contours
    edges_rgb=cv2.cvtColor(edges,cv2.COLOR_GRAY2RGB)
    image_contours=cv2.drawContours(edges_rgb, contours, -1, (0, 255, 255), 2)
    image_contours=cv2.bitwise_not(image_contours)
    edges=cv2.bitwise_not(edges)
    #Find center image 
    height,width=image_contours.shape[:2]
    center=(width//2,height//2)
    # Find closest contour to center image
    min_distance=float('inf')
    closest_contour=None
    cut_image=np.ones((204,204),np.uint8)*255
    for contour in contours:
        for point in contour:
            x, y = point[0]
            distance = np.sqrt((x - center[0])**2 + (y - center[1])**2)
            if distance < min_distance:
                min_distance = distance
                closest_contour = contour
    if closest_contour is not None:
        # fill contour
        #find dimensions of contour
        x,y,w,h=cv2.boundingRect(closest_contour)
        #cut image
        cut_image=edges[y:y+h,x:x+w]
    # Add white background
    cut_image = cv2.copyMakeBorder(
        cut_image,                
        20, 20, 20, 20,  
        cv2.BORDER_CONSTANT,
        value=[255, 255, 255] 
    )
    # normalize
    image=cv2.resize(cut_image,(224,224))
    # image=cv2.cvtColor(image_resized,cv2.COLOR_GRAY2RGB)
    image = np.array(image).astype(float)/255
    if len(image.shape) == 2:  # escala de grises
        image = (image * 255).astype(np.uint8)
        image = cv2.cvtColor(image, cv2.COLOR_GRAY2RGB)
        image = np.array(image).astype(float)/255
    #realizar prediccion
    result = model.predict(image.reshape(1, 224, 224, 3))
    print(image_path)
    # print(result)
    max_prob = np.max(result)
    if max_prob < 0.70:
        result = 3
    else:
        result = np.argmax(result)
    if result==2:
        counter+=1
    # else:
    #     print(image_path)
    #     cv2.imshow("Canny edges", image)
    #     cv2.waitKey(0)
    #     cv2.destroyAllWindows()
acurancy=counter/no_images
print(acurancy)

# P
# FOTOS MAZE
# H 91
# S 39
# U 24
# none 99
# import os
# import tensorflow as tf
# import numpy as np 
# import cv2

# model = tf.keras.models.load_model("C:\\Users\\ferna\\Documents\\robocup-rescuemaze-2025-4\\HSU_FinalP.h5")
# folder_path="C:\\Users\\ferna\\Documents\\robocup-rescuemaze-2025-4\\contourDataset\\U"
# image_list=os.listdir(folder_path)
# no_images=0
# counter=0
# #contar elementos
# for i in image_list:    
#     no_images+=1
#     print(no_images)
# for image_name in image_list:
#     #carga de imagen
#     image_path=os.path.join(folder_path,image_name)
#     img=cv2.imread(image_path)
#     if img is None:
#         print("error al cargar la imagen")
#     # conver to gray scale
#     img=cv2.resize(img,(1000,1000))
#     gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)

#     # create lights mask 
#     _, mask_lights = cv2.threshold(gray, 180, 255, cv2.THRESH_BINARY)
#     kernel = np.ones((15, 15), np.uint8)
#     mask_lights = cv2.dilate(mask_lights, kernel, iterations=1)
#     mask_no_lights = cv2.bitwise_not(mask_lights)
#     # Find edges
#     edges = cv2.Canny(gray, threshold1=20, threshold2=50)  
#     edges = cv2.bitwise_and(edges, edges, mask=mask_no_lights)
#     # Expand edges
#     kernel=np.ones((10,10),np.uint8)
#     edges=cv2.dilate(edges,kernel,iterations=1)
#     # Find Contours
#     contours, _=cv2.findContours(edges,cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)
#     contours = [c for c in contours if cv2.contourArea(c) > 4700]#delete small contours
#     edges_rgb=cv2.cvtColor(edges,cv2.COLOR_GRAY2RGB)
#     image_contours=cv2.drawContours(edges_rgb, contours, -1, (0, 255, 255), 2)
#     image_contours=cv2.bitwise_not(image_contours)
#     edges=cv2.bitwise_not(edges)
#     #Find center image 
#     height,width=image_contours.shape[:2]
#     center=(width//2,height//2)
#     # Find closest contour to center image
#     min_distance=float('inf')
#     closest_contour=None
#     cut_image=np.ones((204,204),np.uint8)*255
#     for contour in contours:
#         for point in contour:
#             x, y = point[0]
#             distance = np.sqrt((x - center[0])**2 + (y - center[1])**2)
#             if distance < min_distance:
#                 min_distance = distance
#                 closest_contour = contour
#     if closest_contour is not None:
#         # fill contour
#         #find dimensions of contour
#         x,y,w,h=cv2.boundingRect(closest_contour)
#         #cut image
#         cut_image=edges[y:y+h,x:x+w]
#     # Add white background
#     cut_image = cv2.copyMakeBorder(
#         cut_image,                
#         20, 20, 20, 20,  
#         cv2.BORDER_CONSTANT,
#         value=[255, 255, 255] 
#     )
#     # normalize
#     image=cv2.resize(cut_image,(224,224))
#     # image=cv2.cvtColor(image_resized,cv2.COLOR_GRAY2RGB)
#     image = np.array(image).astype(float)/255
#     if len(image.shape) == 2:  # escala de grises
#         image = (image * 255).astype(np.uint8)
#         image = cv2.cvtColor(image, cv2.COLOR_GRAY2RGB)
#         image = np.array(image).astype(float)/255
#     #realizar prediccion
#     result1 = model.predict(image.reshape(1, 224, 224, 3))
#     print(image_path)
#     # print(result)
#     max_prob = np.max(result1)
#     if max_prob < 0.70:
#         result = 3
#     else:
#         result = np.argmax(result1)
#     if result==2:
#         counter+=1
#     else:
#         print(image_path)
#         print(result1)
#         # cv2.imshow("Canny edges", image)
#         # cv2.waitKey()
#         # cv2.destroyAllWindows()
# acurancy=counter/no_images
# print(acurancy)

# track sin y con rango de confianza 75%
# PP  70%
# H   0.93
# S   0.88
# U 
# none    

# track sin y con rango de confianza 75%
# PPP  70%
# H   0.89
# S   0.90
# U   0.15
# none  


# final1  70%
# H 0.89  0.94
# S 0.90  0.99
# U 0.88  0.92
# none    0.98

# jetson8
# H 0.89  0.98
# S 0.90  0.73
# U 0.88  0.73
# none    0.98

# final2  70%
# H 0.89  0.93 
# S 0.90  0.88
# U 0.88  0.73
# none    0.98

# final3
# H 0.89  0.88
# S 0.90  0.98
# U 0.71  0.73
# none    0.87