
# Nombre de la sesión tmux
SESSION="maze_session"

# Crear una nueva sesión (detached)
tmux new-session -d -s $SESSION

# Panel 0: cámara
tmux send-keys -t $SESSION "export LD_PRELOAD=/usr/lib/aarch64-linux-gnu/libGLdispatch.so.0 && python3 /home/jetson/maze/com/load_cams.py" C-m

# Dividir en panel vertical
tmux split-window -v -t $SESSION

# Panel 1: scripts del modelo
tmux send-keys -t $SESSION "python3 /home/jetson/maze/com/Constants.py & \
python3 /home/jetson/maze/com/model_class.py & \
python3 /home/jetson/maze/com/Esp_Comunication.py & \
python3 /home/jetson/maze/com/test.py" C-m

# Adjuntar a la sesión
tmux attach-session -t $SESSION



# Nombre de la sesión tmux
SESSION="maze_session"

# Crear una nueva sesión (detached)
tmux new-session -d -s $SESSION

# Panel 0: cámara
tmux send-keys -t $SESSION "export LD_PRELOAD=/usr/lib/aarch64-linux-gnu/libGLdispatch.so.0 && python3 /home/jetson/maze/com/load_cams.py" C-m

# Dividir en panel vertical
tmux split-window -v -t $SESSION

# Panel 1: scripts del modelo
tmux send-keys -t $SESSION "python3 /home/jetson/maze/com/Constants.py ;
python3 /home/jetson/maze/com/model_class.py ;
python3 /home/jetson/maze/com/Esp_Comunication.py ;
python3 /home/jetson/maze/com/test.py" C-m

# Adjuntar a la sesión
tmux attach-session -t $SESSION




#!/bin/bash

echo "Iniciando scripts de Python..."

# Activar entorno virtual o variables si es necesario
export LD_PRELOAD=/usr/lib/aarch64-linux-gnu/libGLdispatch.so.0

python3 /home/jetson/maze/com/Constants.py &
python3 /home/jetson/maze/com/load_colors.py &
python3 /home/jetson/maze/com/Esp_Comunication.py &
python3 /home/jetson/maze/com/test.py &

echo "Todos los scripts han sido lanzados."