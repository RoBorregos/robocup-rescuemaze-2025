#include <iostream>
#include <vector>
#include "coord.h"
#include "Tile.cpp"
using namespace std;
vector<vector<char> > RealMaze = {
        {'#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#'},
        {'#', ' ', ' ', ' ', '#', ' ', '#', ' ', ' ', ' ', '#'}, // 1.
        {'#', ' ', '.', ' ', '.', ' ', '.', '#', '.', '#', '#'},
        {'#', ' ', '#', ' ', ' ', ' ', '#', ' ', '#', ' ', '#'}, // 3.
        {'#', ' ', '.', ' ', '.', ' ', '.', ' ', '.', '#', '#'},
        {'#', ' ', '#', ' ', ' ', ' ', '#', ' ', ' ', ' ', '#'}, // 5.
        {'#', '#', '.', '#', '.', '#', '.', '#', '.', ' ', '#'},
        {'#', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#'}, // 7.
        {'#', ' ', '.', ' ', '.', ' ', '.', ' ', '.', ' ', '#'},
        {'#', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', '#'}, // 9.
        {'#', ' ', '.', ' ', '.', ' ', '.', ' ', '.', ' ', '#'},
        {'#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#'}, // 1.
        {'#', ' ', '.', '#', '.', ' ', '.', '#', '.', ' ', '#'},
        {'#', ' ', ' ', ' ', '#', ' ', '#', ' ', ' ', ' ', '#'}, // 3.
        {'#', ' ', '.', '#', '.', ' ', '.', '#', '.', ' ', '#'},
        {'#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#'}, // 5.
        {'#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#'}
};

vector<vector<char> > UndescoverdMazeSecondLevel = {
    {'#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#'},
    {'#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#'}, // 1.
    {'#', ' ', '.', ' ', '#', ' ', '#', '#', '#', '#', '#'},
    {'#', ' ', '#', ' ', '#', ' ', ' ', ' ', '#', ' ', '#'}, // 3.
    {'#', ' ', '.', '#', '#', ' ', '#', ' ', '#', '#', '#'},
    {'#', ' ', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', '#'}, // 5.
    {'#', ' ', '.', '#', '#', ' ', '#', '#', ' ', ' ', '#'},
    {'#', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#'}, // 7.
    {'#', ' ', '.', ' ', '#', ' ', ' ', ' ', ' ', ' ', '#'},
    {'#', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', '#'}, // 9.
    {'#', ' ', '#', ' ', '#', ' ', '#', ' ', '#', ' ', '#'},
    {'#', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#'}, // 1.
    {'#', ' ', '#', ' ', '#', '#', '#', ' ', ' ', ' ', '#'},
    {'#', ' ', ' ', 'r', ' ', ' ', ' ', 'l', ' ', ' ', '#'}, // 3.
    {'#', ' ', ' ', ' ', '#', '#', '#', ' ', ' ', ' ', '#'},
    {'#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#'}, // 5.
    {'#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#'}
};
/*Izq:'<'
Der: '>'
Arriba: '^'
Abajo: 'v'
*/
vector<vector<char> > discoverMaze ={
    {'#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#'},
    {'#', 'v', '.', '.', '.', '.', '.', '.', '.', '.', '#'}, // 1.
    {'#', '.', '.', '.', '.', '.', '.', '.', '.', '.', '#'},
    {'#', '.', '.', '.', '.', '.', '.', '.', '.', '.', '#'}, // 3.
    {'#', '.', '.', '.', '.', '.', '.', '.', '.', '.', '#'},
    {'#', '.', '.', '.', '.', '.', '.', '.', '.', '.', '#'}, // 5.
    {'#', '.', '.', '.', '.', '.', '.', '.', '.', '.', '#'},
    {'#', '.', '.', '.', '.', '.', '.', '.', '.', '.', '#'}, // 7.
    {'#', '.', '.', '.', '.', '.', '.', '.', '.', '.', '#'},
    {'#', '.', '.', '.', '.', '.', '.', '.', '.', '.', '#'}, // 9.
    {'#', '.', '.', '.', '.', '.', '.', '.', '.', '.', '#'},
    {'#', '.', '.', '.', '.', '.', '.', '.', '.', '.', '#'}, // 1.
    {'#', '.', '.', '.', '.', '.', '.', '.', '.', '.', '#'},
    {'#', '.', '.', '.', '.', '.', '.', '.', '.', '.', '#'}, // 3.
    {'#', '.', '.', '.', '.', '.', '.', '.', '.', '.', '#'},
    {'#', '.', '.', '.', '.', '.', '.', '.', '.', '.', '#'}, // 5.
    {'#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#'},
};
int steps = 0; 
coord robotCoord = {1,1,1};

const int rows = 17;
const int cols = 11;

int directions[4][2] = {{0,2},{2,0},{0,-2},{-2,0}};
char robotChar = 'v';

void right(){
    switch(robotChar){
        case '^':
            robotChar = '>';
            break;
        case '>':
            robotChar = 'v';
            break;
        case 'v':
            robotChar = '<';
            break;
        case '<':
            robotChar = '^';
            break;
    }
    discoverMaze[robotCoord.y][robotCoord.x] = robotChar;
}
void girar(int directions[4][2]){
    int tempx = directions[0][0];
    int tempy = directions[0][1];
    for (int i = 0; i< 3; i++){
        directions[i][0] = directions[i+1][0];
        directions[i][1] = directions[i+1][1];
    }
    directions[3][0] = tempx;
    directions[3][1] = tempy;
    right();
}
bool paredAdelante(coord next){
    if(RealMaze[next.y- (directions[0][1]/2)][next.x-(directions[0][0]/2)] == '#'){
        return true;
    }
    return false;
    
}
void ahead(coord next){
    discoverMaze[robotCoord.y][robotCoord.x] = '*';
    robotCoord = next;
    discoverMaze[robotCoord.y][robotCoord.x] = robotChar;
}
void back(coord past){
    discoverMaze[robotCoord.y][robotCoord.x] = '*';
    robotCoord = past;
    discoverMaze[robotCoord.y][robotCoord.x] = robotChar;
}
void printMaze(vector<vector<char> > maze){
    for(int i = 0; i < maze.size(); ++i){
        for(int j = 0; j < maze[i].size(); j++){
            cout << maze[i][j] << " ";
        }
        cout << endl;
    }
    steps++;
}
void search(bool visited[rows][cols], coord current, int directions[4][2], int backstep[rows][cols], int& cnt, bool& pathFound){
    int x = current.x;
    int y = current.y;
    visited[y][x] = true; 
    if (!pathFound){
        cnt++;
        backstep[y][x] = cnt;
    }
    if(x == cols && y == rows){
    pathFound = true; 
    }
    for (int i = 0; i<4; i++){
        coord next = coord{x + directions[0][0], y + directions[0][1], 1};
        // Serial.println(newY);
        // delay(100);
        if((next.x >= 0 && next.y >= 0 && next.x<cols && next.y <rows) && (visited[next.y][next.x] == false)){
            if((paredAdelante(next) == false)){
                ahead(next);
                printMaze(discoverMaze);
                search(visited, next, directions, backstep, cnt, pathFound);
                back(current);
                printMaze(discoverMaze);
            }
            else{
                discoverMaze[next.y- (directions[0][1]/2)][next.x-(directions[0][0]/2)] = 'X';
                printMaze(discoverMaze);
            }
        }
        girar(directions);
        printMaze(discoverMaze);
    }
    
    if(pathFound == false){
        backstep[x][y] = 30;
        cnt--;
    }
}
int main(){
    bool visited[rows][cols];
    int backstep[rows][cols];
    for(int i = 0; i<rows; i++){
        for(int j = 0; j<cols; j++){
            visited[i][j] = false;
            backstep[i][j] = 30;
        }
    }
    int cnt = 0;
    bool pathFound = false;
    search(visited, robotCoord, directions, backstep, cnt, pathFound);

    cout << "Amount of steps" << steps << endl;
    return 0;
}
/*
Output:
# # # # # # # # # # # 
# v . * . * . * . * # 
# . . . . . . X . X # 
# * . * X * . * X . # 
# . . X . . . . . X # 
# * . * . * X * . * # 
# . . X . . . X . . # 
# * . * . * . * . * # 
# . . . . . . . . . # 
# * . * . * . * . * # 
# . . . . . . . . . # 
# * . * . * . * . * # 
# . . X . . . X . . # 
# * . * X * X * . * # 
# . . X . . . . . . # 
# * . * . * . * . * # 
# # # # # # # # # # # 
Amount of steps 246

# # # # # # # # # # # 
# v . * X * X . . . # 
# . . . . . . . . . # 
# * X * . * X . . . # 
# . . . . . . . . . # 
# * X * . * X . . . # 
# X . X . X . . . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# # # # # # # # # # # 
Amount of steps61
*/