import os
from PIL import Image
import cv2
import numpy as np
input_path="C:\\Users\\ferna\\Documents\\robocup-rescuemaze-2025-4\\FotosMaze\\S"
# input_path="C:\\Users\\ferna\\Downloads\\none1"
outputh_path="C:\\Users\\ferna\\Documents\\robocup-rescuemaze-2025-4\\tmrDataset\\S2"
os.makedirs(outputh_path,exist_ok=True)#crear carpeta si no ha sido creada

for archivo in os.listdir(input_path):#recorrer archivos
    print(archivo)
    if archivo.endswith(".jpg") or archivo.endswith(".png"):#si es imagen
        if archivo.startswith("._"):
            archivo=archivo[2:]
            print(os.path.join(input_path,archivo))
        img=cv2.imread(os.path.join(input_path,archivo))#abrir imagen usando PIL
        if img is None:
            print("error al cargar la imagen")
        else:
            print("cargada")
        img=cv2.resize(img,(1000,1000))
        # conver to gray scale
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
            # cv2.drawContours(edges,contours,-1,color=0,thickness=-1)
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
        image_resized=cv2.resize(cut_image,(224,224))
        image=cv2.cvtColor(image_resized,cv2.COLOR_GRAY2RGB)
        # image = np.array(image).astype(float)/255
        # cv2.imshow("Image", image)
        # cv2.waitKey(0)
        # cv2.destroyAllWindows()

        cv2.imwrite(os.path.join(outputh_path,f"graycale_{archivo}"),image)#guardar
print("completado")
