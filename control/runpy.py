import socket
import time
# Configura la IP del ESP32 y el puerto
ESP32_IP = "192.168.1.118"
PORT = 1234

# Crea el socket en la PC
client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
client_socket.connect((ESP32_IP, PORT))

# Recibe los datos
while True:
    data = client_socket.recv(1024)
    if data:
        print(data.decode())
        #time.sleep(0.1)

client_socket.close()