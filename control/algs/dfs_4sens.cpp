#include <iostream>
#include <vector>
#include <climits>
using namespace std;

bool black = false; 
void ahead(){
    //ahead and return changing black to true
    if(0 == 1){
        black = true;
    }
}
void back(){}
void left(){}
void right(){}
bool cuadroNegro(){return false;}
// sensores: 0 -adelante, 1 - izquierda, 2 -atras , 3 - derecha
bool paredAdelante(int sensor){}
int rows = 0;
int cols = 0;
//directions: up, left, down, right
int directions[4][2] = {{0, 1},{ -1, 0}, {0, -1}, {1, 0}};
int steps[4][2] = {{0, 1},{ -1, 0}, {0, -1}, {1, 0}};
//regresar arr*arr 
void girar(int change[4][2]){
    int tempx = change[0][0];
    int tempy = change[0][1];
    for (int i = 0; i< 3; i++){
        change[i][0] = change[i+1][0];
        change[i][1] = change[i+1][1];
    }
    change[3][0] = tempx;
    change[3][1] = tempy;
}

void search(vector<vector<bool> > visited, int x, int y, vector<vector<int> > backstep, int& count, bool& pathFound){
    visited[x][y] = true;
    if(!pathFound){
        count++;
        backstep[x][y] = count;
    }
    if(x== rows-1 && y == cols-1){
        pathFound = true; 
    }
    int j =0;
    for (int i = 0; i<4; i++){
        int newX = x + steps[0][0];
        int newY = y + steps[0][1];
        //i - j for choosing the right sensor
        if((newX >= 0 && newY >=0 && newX<=rows && newY <= cols) && (paredAdelante(i-j) == false) && (visited[newX][newY] == false)){
            //check right() function
            while(directions[0][0] != steps[0][0]){
                girar(directions);
                left();
                j++;
            }
            ahead();
            if(black == false){
                search(visited, newX, newY, backstep, count, pathFound);
                while(j<4){
                    left();
                    j++;
                }
                back();
            }else{
                visited[newX][newY] = true; 
            }
        }             
        girar(steps);
    }
    if(!pathFound){
        backstep[x][y] = INT_MAX;
        count--;
    }
}
void fuga(int count, int x, int y, int directions[4][2], int backstep[5][3]){
    for(int i = 0; i < count; i++){
        for (int j = 0; j < 4; j++){
            int newX = x + directions[0][0];
            int newY = y + directions[0][1];
            if (newX >= 0 && newY >= 0 && newX < 5 && newY < 3 && backstep[newX][newY] == i+1){
                ahead();
                j=4;
            }else{
                girar(directions);
            }
        }
    }
    ahead();
}
void maze() {

    //initialize maze and vectors for logic
    vector<vector<bool> > visited;
    vector<vector<int> > backstep;
    for(int i = 0; i < rows; i++){
        vector<bool> temp;
        vector<int> temp2;
        for(int j = 0; j < cols; j++){
            temp.push_back(false);
            temp2.push_back(INT_MAX);
        }
        visited.push_back(temp);
        backstep.push_back(temp2);
    }
    int start_x = 0;
    int start_y = 0;  
    //initialize count and pathFound
    int count = 0;
    bool pathFound = false;
    search(visited, start_x, start_y, backstep, count, pathFound);
    
}