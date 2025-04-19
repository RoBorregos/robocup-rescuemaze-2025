# from Comunication import Esp32
import Constants 
from Esp_Comunication import Esp32
import time

robot=Esp32()
robot.connect()

while(True):
    robot.sentDetection()



# import serial

# # Configuración del puerto serial
# puerto = "COM11"  # Cambia si es necesario (Linux/Jetson Nano)
# # Si estás en Windows, usa algo como: puerto = "COM3"

# # Inicializar la conexión serial
# ser = serial.Serial(port=puerto, baudrate=115200, timeout=1)

# # Verificar si el puerto está abierto
# if ser.isOpen():
#     print(f"Conectado correctamente a {puerto}")
# else:
#     print(f"Error al abrir el puerto {puerto}")
#     exit()

# # Leer datos continuamente desde el puerto
# try:
#     while True:
#         if ser.in_waiting > 0:
#             # Leer una línea completa del puerto
#             datos = ser.readline().decode('utf-8').strip()

#             if datos:
#                 print(f"Datos recibidos: {datos}")

# except KeyboardInterrupt:
#     print("\nInterrumpido por el usuario, cerrando conexión...")
#     ser.close()
#     print("Conexión cerrada correctamente.")




