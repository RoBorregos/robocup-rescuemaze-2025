import os
import shutil

origen = "C:\\Users\\ferna\\Downloads\\none5"
destino = "C:\\Users\\ferna\\Documents\\robocup-rescuemaze-2025-4\\Ddataset\\none"

# Asegurarse de que el destino exista
os.makedirs(destino, exist_ok=True)

for archivo in os.listdir(origen):
    origen_path = os.path.join(origen, archivo)
    if os.path.isfile(origen_path):
        nombre, extension = os.path.splitext(archivo)
        destino_path = os.path.join(destino, archivo)
        contador = 1

        # Si ya existe, generar un nuevo nombre
        while os.path.exists(destino_path):
            nuevo_nombre = f"{nombre}_{contador}{extension}"
            destino_path = os.path.join(destino, nuevo_nombre)
            contador += 1

        shutil.copy2(origen_path, destino_path)
        print(f"Copiado: {archivo} → {os.path.basename(destino_path)}")
