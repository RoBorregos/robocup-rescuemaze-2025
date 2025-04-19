
import os
from PIL import Image
input_path="C:\\Users\\ferna\\Documents\\robocup-rescuemaze-2025-4\\fotos\\dataset_HSU\\none"
outputh_path="C:\\Users\\ferna\\Documents\\robocup-rescuemaze-2025-4\\fotos\\grayDataset\\none"
os.makedirs(outputh_path,exist_ok=True)#crear carpeta si no ha sido creada

for archivo in os.listdir(input_path):#recorrer archivos
    if archivo.endswith(".jpg") or archivo.endswith(".png"):#si es imagen
        img=Image.open(os.path.join(input_path,archivo))#abrir imagen usando PIL
        img_gray=img.convert("L")#convertir a grises
        img_gray.save(os.path.join(outputh_path,f"graycale_{archivo}"))#guardar
print("completado")