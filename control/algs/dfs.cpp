void left();
void corregir_giro();
void wait(int time);
void aheadC();
int c;
int c2;
int colors[3] = {0};

// ZONA C !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
void girar(int directions[4][2]){
  int tempx = directions[0][0];
  int tempy = directions[0][1];
  for (int i = 0; i< 3; i++){
    directions[i][0] = directions[i+1][0];
    directions[i][1] = directions[i+1][1];
  }
  directions[3][0] = tempx;
  directions[3][1] = tempy;
  left();
  corregir_giro();
  c=0;
  c2=0;
}
void colorDet(){
  wait(500);
  int col = getcolor();
  if(col == 1){
    digitalWrite(purpleLed,1);
    delay(500);
    digitalWrite(purpleLed,0);
  }
  else if(col == 2){
    digitalWrite(pinkLed,1);
    delay(500);
    digitalWrite(pinkLed,0);
  }
  else if(col == 3){
    digitalWrite(yellowLed,1);
    delay(500);
    digitalWrite(yellowLed,0);
  }
  colors[col-1]++;
}
//arriba, izquierda, abajo, derecha
void search(bool visited[3][5], int x, int y, int directions[4][2], int backstep[3][5], int& cnt, bool& pathFound){
  colorDet();
  visited[x][y] = true;
  if(pathFound == false){
      cnt++;
      backstep[x][y] = cnt;
  }
  if(x == 2 && y == 4){
    pathFound = true; 
  }
  for (int i = 0; i<4; i++){
    int newX = x + directions[0][0];
    int newY = y + directions[0][1];
    // Serial.println(newY);
    // delay(100);
    if((newX >= 0 && newY >= 0 && newX<3 && newY <5) && (paredAdelante() == false)){
      if(visited[newX][newY] == false){
        aheadC();
        c=0;
        c2=0;
        if(lineaNegra == false){
          search(visited, newX, newY, directions, backstep, cnt, pathFound);
          corregir_giro();
          c=0;
          c2=0;
          back(100);
          stop(1000);
        }
        else{
          visited[newX][newY] = true;
          analogWrite(blackLed,125);
          delay(1000);
          digitalWrite(blackLed,0);
        }
      }
    }
    girar(directions);
  }
  if(pathFound == false){
    backstep[x][y] = 30;
    cnt--;
  }
}
void fuga(int cnt, int x, int y, int Mcolor, int directions[4][2], int backstep[3][5]){
  bool foundColor = false;
  for(int i = 1; i < cnt; i++){
    if(foundColor == false){
      int col = getcolor();
      if (col== Mcolor){
        Mcolor = 30;
        foundColor = true;
        colorDet();
      }
    }
    //búsqueda 
    for (int j = 0; j < 4; j++){
      int newX = x + directions[0][0];
      int newY = y + directions[0][1];
      if (newX >= 0 && newY >= 0 && newX < 3 && newY <5 && backstep[newX][newY] == i+1){
        c = 0;
        c2=0;
        aheadC();
        j=4;
        x = newX;
        y = newY;
      }else{
        girar(directions);
      }
    }
  }
  //salida en la ultima casilla
  for(int i = 0; i< 4; i++){
    int newX = x + directions[0][0];
    int newY = y + directions[0][1];
    if(newX == 2 && newY == 5){
      aheadC();
    }
  }
}
void zonaC() {
  
  //adelante, izquierda, atras, derecha
  int directions[4][2] = {{0, 1}, {-1, 0}, {0, -1}, {1, 0}};
  // matriz de casillas visitadas
  bool visited[3][5] = {{false}}; 
  // matriz para trazar el camino a la salida desde 1 hasta n pasos.
  int backstep[3][5] = {{30}};
  // contador de pasos
  int cnt = 0;
  // si ya se encontro el camino
  bool pathFound = false;
  // para almacenar el valor del color que más se repite
  int Mcolor = 0;
  // punto en x y en y inicial
  int start_x = 1, start_y = 0;
  // llamada a la función de búsqueda
  search(visited, start_x, start_y, directions, backstep, cnt, pathFound);
  // ciclo para encontrar el color que más se repite. 
  for(int i = 0; i<2; i++){
    if(colors[i]< colors[i+1]){
      Mcolor = i+1;
    }
  }
  // llamada a la función para salir del laberinto
  fuga(cnt, start_x, start_y, Mcolor, directions, backstep);
}