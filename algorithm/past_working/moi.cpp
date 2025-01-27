
#include <iostream>
#include "coord.h"
#include "Tile.cpp"
#include "Map.cpp"
using namespace std;

vector<vector<char> > RealMaze = {
        
        {'#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#'},
        {'#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#'}, // 1.
        {'#', ' ', '.', ' ', '.', ' ', '.', ' ', '.', ' ', '#'},
        {'#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#'}, // 3.
        {'#', ' ', '.', '#', '.', '#', '.', '#', '.', ' ', '#'},
        {'#', ' ', '#', ' ', ' ', ' ', '#', ' ', '#', ' ', '#'}, // 5.
        {'#', ' ', '.', ' ', '.', ' ', '.', ' ', '.', ' ', '#'},
        {'#', ' ', '#', ' ', '#', ' ', ' ', ' ', '#', ' ', '#'}, // 7.
        {'#', ' ', '.', ' ', '.', ' ', '.', ' ', '.', ' ', '#'},
        {'#', ' ', '#', ' ', '#', ' ', ' ', ' ', '#', ' ', '#'}, // 9.
        {'#', ' ', '.', ' ', '.', '#', '.', ' ', '.', ' ', '#'},
        {'#', ' ', '#', ' ', ' ', ' ', ' ', ' ', '#', ' ', '#'}, // 1.
        {'#', ' ', '.', '#', '.', '#', '.', '#', '.', ' ', '#'},
        {'#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#'}, // 3.
        {'#', ' ', '.', ' ', '.', ' ', '.', ' ', '.', ' ', '#'},
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
    {'#', '.', '.', '.', '.', '.', '.', '.', '.', '.', '#'}, // 1.
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
coord inicio = {3,5,1};
coord robotCoord = {3,5,1};
char robotChar = 'v';
TileDirection directions[4] = {TileDirection::kDown,TileDirection::kRight,TileDirection::kLeft,TileDirection::kUp};
int robotOrientation = 0;
const int kMaxSize = 256; 

void printMaze(vector<vector<char> > maze){
    for(int i = 0; i < maze.size(); ++i){
        for(int j = 0; j < maze[i].size(); j++){
            cout << maze[i][j] << " ";
        }
        cout << endl;
    }
    steps++;
}

void ahead(coord next){
    discoverMaze[robotCoord.y][robotCoord.x] = '*';
    robotCoord = next;
    discoverMaze[robotCoord.y][robotCoord.x] = robotChar;
    printMaze(discoverMaze);
}
void back(coord past){
    discoverMaze[robotCoord.y][robotCoord.x] = '*';
    robotCoord = past;
    discoverMaze[robotCoord.y][robotCoord.x] = robotChar;
}
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
void left(){
    switch(robotChar){
        case '^':
            robotChar = '<';
            break;
        case '<':
            robotChar = 'v';
            break;
        case 'v':
            robotChar = '>';
            break;
        case '>':
            robotChar = '^';
            break;
    }
    discoverMaze[robotCoord.y][robotCoord.x] = robotChar;
}
bool checkForWall(const vector<vector<char> >& maze, const TileDirection& direction, const coord& robotCoord) {
    switch(direction) {
        case TileDirection::kRight:
            return maze[robotCoord.y][robotCoord.x + 1] == '#';
        case TileDirection::kUp:
            return maze[robotCoord.y + 1][robotCoord.x] == '#';
        case TileDirection::kLeft:
            return maze[robotCoord.y][robotCoord.x - 1] == '#';
        case TileDirection::kDown:
            return maze[robotCoord.y - 1][robotCoord.x] == '#';
    }
}
void turnRobot(const int targetOrientation) {
    int difference = targetOrientation - robotOrientation;
    if (difference == 0) {
        return;
    }
    if (difference == 90 || difference == -270) {
        right();
        robotOrientation = (robotOrientation + 90) % 360;
    } else if (difference == 270 || difference == -90) {
        left();
        robotOrientation = (robotOrientation + 270) % 360;
    } else if (difference == 180|| difference == -180) {
        right();
        right();
        robotOrientation = (robotOrientation + 180) % 360;
    }
    printMaze(discoverMaze);
}
void followPath(stack<coord>& path){
    while(!path.empty()){
        const coord& next = path.top();
        path.pop();
        if (next.x > robotCoord.x) {
            turnRobot(270);
        } else if (next.x < robotCoord.x) {
            turnRobot(90);
        } else if (next.y > robotCoord.y) {
            turnRobot(0);
        } else if (next.y < robotCoord.y) {
            turnRobot(180);
        }
        ahead(next);
        printMaze(discoverMaze);
        
    }
}
void dijkstra(coord& start, coord& end, Map& tilesMap, vector<Tile>tiles){
    stack<coord> path;
    
    vector<bool> explored(tilesMap.positions.size(), false);
    vector<int> distance(tilesMap.positions.size(), INT_MAX);
    vector<coord> previousPositions(tilesMap.positions.size(), kInvalidPosition);
    /*
    for(int i = tilesMap.positions.size()-1; i>=0; --i){
        distance.push_back(INT_MAX);
        explored.push_back(false);
    }*/

    distance[tilesMap.getIndex(start)] = 0;
    explored[tilesMap.getIndex(start)] = true;
    
    coord current = start;
    while(!explored[tilesMap.getIndex(end)]){
        int minDist = INT_MAX;
        for(const TileDirection& direction : directions){
            Tile& currentTile = tiles[tilesMap.getIndex(current)];
            coord& adjacent = currentTile.adjacentTiles_[static_cast<int>(direction)]->position_;
            // find the distance to the adjacent tile
            if(currentTile.adjacentTiles_[static_cast<int>(direction)] != nullptr && !currentTile.hasWall(direction)){
                int weight = currentTile.weights_[static_cast<int>(direction)] +distance[tilesMap.getIndex(current)];
                //int adjacentIndex = tilesMap.getIndex(adjacent);
                //if(adjacentIndex != -1){
                //if(adjacent != kInvalidPosition){
                    if(weight < distance[tilesMap.getIndex(adjacent)]){
                        distance[tilesMap.getIndex(adjacent)] = weight;
                        previousPositions[tilesMap.getIndex(adjacent)] = current;
                    }
                //}
                //}
            }
        }
        //find the minimum distance to the path line
        for(int i = tilesMap.positions.size() -1; i >= 0; --i){
            coord& currentCoord = tilesMap.positions[i];
            int currentDistance = distance[tilesMap.getIndex(currentCoord)];
            if(currentDistance < minDist && !explored[tilesMap.getIndex(currentCoord)]){
                minDist = currentDistance;
                current = currentCoord;
            }
        }
        explored[tilesMap.getIndex(current)] = true;
    }
    current = end;
    while(current != start){
        path.push(current);
        current = previousPositions[tilesMap.getIndex(current)];
    }
    path.push(start);
    followPath(path);
}
void dfs(Map& visitedMap, vector<Tile>& tiles, Map& tilesMap){
    stack<coord> unvisited;
    vector<bool> visited;
    unvisited.push(robotCoord);//push the initial position
    
    coord next;
    Tile* currentTile;
    TileDirection oppositeDirection;
    bool visitedFlag = false;
    bool wall = false;

    while(!unvisited.empty()){
        //constants for the directions
        coord current = unvisited.top();
        unvisited.pop();
        //check two times for visited?? 
        visitedFlag = false;
        for(int i = 0; i < visitedMap.positions.size(); ++i){
            if(visitedMap.positions[i] == current){
                visitedFlag = true;
                break;
            }
        }
        if (visitedFlag) {
            continue;
        }
        dijkstra(robotCoord, current, tilesMap, tiles);
        visitedMap.positions.push_back(current);
        visited.push_back(true);
        //ahead(current);
        robotCoord = current;
        for(const TileDirection direction: directions){
            wall = false; 
            coord wallCoord = {0,0,0};
            if(checkForWall(RealMaze, direction, robotCoord)){
                wall = true;
            }
            switch(direction) {
                case TileDirection::kRight:
                    next = coord{current.x + 2, current.y, 1};
                    currentTile = &tiles[tilesMap.getIndex(current)];
                    wallCoord = {current.x + 1, current.y, 1};
                    oppositeDirection = TileDirection::kLeft;
                    break;
                case TileDirection::kUp:
                    next = coord{current.x, current.y + 2, 1};
                    currentTile = &tiles[tilesMap.getIndex(current)];
                    wallCoord = {current.x, current.y + 1, 1};
                    oppositeDirection = TileDirection::kDown;
                    break;
                case TileDirection::kLeft:
                    next = coord{current.x - 2, current.y, 1};
                    currentTile = &tiles[tilesMap.getIndex(current)];
                    wallCoord = {current.x - 1, current.y, 1};
                    oppositeDirection = TileDirection::kRight;
                    break;
                case TileDirection::kDown:
                    next = coord{current.x, current.y - 2, 1};
                    currentTile = &tiles[tilesMap.getIndex(current)];
                    wallCoord = {current.x, current.y - 1, 1};
                    oppositeDirection = TileDirection::kUp;
                    break;
            }
            //right();
            //printMaze(discoverMaze);
            //end of regular dfs search
            //start of second level dfs search check if has wall   
            //check for adjacentTiles and connecting them
            if(currentTile -> adjacentTiles_[static_cast<int>(direction)] == nullptr){
                // if the tile is not in the map
                tilesMap.positions.push_back(next);
                tiles[tilesMap.getIndex(next)] = Tile(next);
                Tile* nextTile = &tiles[tilesMap.getIndex(next)];
                
                if(nextTile->position_ == kInvalidPosition){
                    nextTile->setPosition(next);
                }
                // join the tiles and if there is no wall between them
                currentTile -> addAdjacentTile(direction, nextTile, wall);
                nextTile -> addAdjacentTile(oppositeDirection, currentTile, wall);
                if(!wall){
                    visitedFlag = false;
                    for(int i = 0; i < visitedMap.positions.size(); ++i){
                        if(visitedMap.positions[i] == next){
                            visitedFlag = true;
                            break;
                        }
                    }
                    if(!visitedFlag){
                        unvisited.push(next);
                    }
                }
                else{
                    if(RealMaze[wallCoord.y][wallCoord.x] == '#'){
                        discoverMaze[wallCoord.y][wallCoord.x] = 'X';
                    }
                }
            }
        }
    }
    //dijkstra(robotCoord, inicio, visitedMap, tiles);
}
int main(){
    Map visitedMap = Map();
    Map tilesMap = Map();
    vector<Tile> tiles(kMaxSize, Tile(kInvalidPosition));
    tilesMap.positions.push_back(robotCoord);
    tiles[tilesMap.getIndex(robotCoord)] = Tile(robotCoord);
    dfs(visitedMap, tiles, tilesMap);
    cout<<"DFS Finalizado con " << steps << " movimientos"; 
    return 0;
}
//revisar la posición de la instancia dijkstra, 
//y resolvelr el problema de lógica de como hacerle para las adjacent tiles del current coord ya que aun no se explora.

/*
# # # # # # # # # # # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
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
# # # # # # # # # # # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . v . . . . . . # 
# . . . . . . . . . # 
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
# # # # # # # # # # # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . X . . . . . . # 
# . X v . . . . . . # 
# . . . . . . . . . # 
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
# # # # # # # # # # # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . X . . . . . . # 
# . X v . . . . . . # 
# . . . . . . . . . # 
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
# # # # # # # # # # # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . X . . . . . . # 
# . X v . . . . . . # 
# . . . . . . . . . # 
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
# # # # # # # # # # # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . X . . . . . . # 
# . X * . . . . . . # 
# . . . . . . . . . # 
# . . v . . . . . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# # # # # # # # # # # 
# # # # # # # # # # # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . X . . . . . . # 
# . X * . . . . . . # 
# . . . . . . . . . # 
# . X v X . . . . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# # # # # # # # # # # 
# # # # # # # # # # # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . X . . . . . . # 
# . X * . . . . . . # 
# . . . . . . . . . # 
# . X v X . . . . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# # # # # # # # # # # 
# # # # # # # # # # # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . X . . . . . . # 
# . X * . . . . . . # 
# . . . . . . . . . # 
# . X v X . . . . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# # # # # # # # # # # 
# # # # # # # # # # # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . X . . . . . . # 
# . X * . . . . . . # 
# . . . . . . . . . # 
# . X * X . . . . . # 
# . . . . . . . . . # 
# . . v . . . . . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# # # # # # # # # # # 
# # # # # # # # # # # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . X . . . . . . # 
# . X * . . . . . . # 
# . . . . . . . . . # 
# . X * X . . . . . # 
# . . . . . . . . . # 
# . X v X . . . . . # 
# . . X . . . . . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# # # # # # # # # # # 
# # # # # # # # # # # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . X . . . . . . # 
# . X * . . . . . . # 
# . . . . . . . . . # 
# . X * X . . . . . # 
# . . . . . . . . . # 
# . X v X . . . . . # 
# . . X . . . . . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# # # # # # # # # # # 
# # # # # # # # # # # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . X . . . . . . # 
# . X * . . . . . . # 
# . . . . . . . . . # 
# . X * X . . . . . # 
# . . . . . . . . . # 
# . X < X . . . . . # 
# . . X . . . . . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# # # # # # # # # # # 
# # # # # # # # # # # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . X . . . . . . # 
# . X * . . . . . . # 
# . . . . . . . . . # 
# . X * X . . . . . # 
# . . . . . . . . . # 
# . X ^ X . . . . . # 
# . . X . . . . . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# # # # # # # # # # # 
# # # # # # # # # # # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . X . . . . . . # 
# . X * . . . . . . # 
# . . . . . . . . . # 
# . X * X . . . . . # 
# . . . . . . . . . # 
# . X ^ X . . . . . # 
# . . X . . . . . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# # # # # # # # # # # 
# # # # # # # # # # # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . X . . . . . . # 
# . X * . . . . . . # 
# . . . . . . . . . # 
# . X * X . . . . . # 
# . . . . . . . . . # 
# . X ^ X . . . . . # 
# . . X . . . . . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# # # # # # # # # # # 
# # # # # # # # # # # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . X . . . . . . # 
# . X * . . . . . . # 
# . . . . . . . . . # 
# . X ^ X . . . . . # 
# . . . . . . . . . # 
# . X * X . . . . . # 
# . . X . . . . . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# # # # # # # # # # # 
# # # # # # # # # # # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . X . . . . . . # 
# . X * . . . . . . # 
# . . . . . . . . . # 
# . X ^ X . . . . . # 
# . . . . . . . . . # 
# . X * X . . . . . # 
# . . X . . . . . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# # # # # # # # # # # 
# # # # # # # # # # # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . X . . . . . . # 
# . X ^ . . . . . . # 
# . . . . . . . . . # 
# . X * X . . . . . # 
# . . . . . . . . . # 
# . X * X . . . . . # 
# . . X . . . . . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# # # # # # # # # # # 
# # # # # # # # # # # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . X . . . . . . # 
# . X > . . . . . . # 
# . . . . . . . . . # 
# . X * X . . . . . # 
# . . . . . . . . . # 
# . X * X . . . . . # 
# . . X . . . . . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# # # # # # # # # # # 
# # # # # # # # # # # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . X . . . . . . # 
# . X > . . . . . . # 
# . . . . . . . . . # 
# . X * X . . . . . # 
# . . . . . . . . . # 
# . X * X . . . . . # 
# . . X . . . . . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# # # # # # # # # # # 
# # # # # # # # # # # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . X . . . . . . # 
# . X > . . . . . . # 
# . . . . . . . . . # 
# . X * X . . . . . # 
# . . . . . . . . . # 
# . X * X . . . . . # 
# . . X . . . . . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# # # # # # # # # # # 
# # # # # # # # # # # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . X . . . . . . # 
# . X * . > . . . . # 
# . . . . . . . . . # 
# . X * X . . . . . # 
# . . . . . . . . . # 
# . X * X . . . . . # 
# . . X . . . . . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# # # # # # # # # # # 
# # # # # # # # # # # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . X . X . . . . # 
# . X * . > X . . . # 
# . . . . . . . . . # 
# . X * X . . . . . # 
# . . . . . . . . . # 
# . X * X . . . . . # 
# . . X . . . . . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# # # # # # # # # # # 
# # # # # # # # # # # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . X . X . . . . # 
# . X * . > X . . . # 
# . . . . . . . . . # 
# . X * X . . . . . # 
# . . . . . . . . . # 
# . X * X . . . . . # 
# . . X . . . . . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# # # # # # # # # # # 
# # # # # # # # # # # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . X . X . . . . # 
# . X * . v X . . . # 
# . . . . . . . . . # 
# . X * X . . . . . # 
# . . . . . . . . . # 
# . X * X . . . . . # 
# . . X . . . . . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# # # # # # # # # # # 
# # # # # # # # # # # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . X . X . . . . # 
# . X * . v X . . . # 
# . . . . . . . . . # 
# . X * X . . . . . # 
# . . . . . . . . . # 
# . X * X . . . . . # 
# . . X . . . . . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# # # # # # # # # # # 
# # # # # # # # # # # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . X . X . . . . # 
# . X * . v X . . . # 
# . . . . . . . . . # 
# . X * X . . . . . # 
# . . . . . . . . . # 
# . X * X . . . . . # 
# . . X . . . . . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# # # # # # # # # # # 
# # # # # # # # # # # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . X . X . . . . # 
# . X * . * X . . . # 
# . . . . . . . . . # 
# . X * X v . . . . # 
# . . . . . . . . . # 
# . X * X . . . . . # 
# . . X . . . . . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# # # # # # # # # # # 
# # # # # # # # # # # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . X . X . . . . # 
# . X * . * X . . . # 
# . . . . . . . . . # 
# . X * X v . . . . # 
# . . . . . . . . . # 
# . X * X . . . . . # 
# . . X . . . . . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# # # # # # # # # # # 
# # # # # # # # # # # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . X . X . . . . # 
# . X * . * X . . . # 
# . . . . . . . . . # 
# . X * X v . . . . # 
# . . . . . . . . . # 
# . X * X . . . . . # 
# . . X . . . . . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# # # # # # # # # # # 
# # # # # # # # # # # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . X . X . . . . # 
# . X * . * X . . . # 
# . . . . . . . . . # 
# . X * X v . . . . # 
# . . . . . . . . . # 
# . X * X . . . . . # 
# . . X . . . . . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# # # # # # # # # # # 
# # # # # # # # # # # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . X . X . . . . # 
# . X * . * X . . . # 
# . . . . . . . . . # 
# . X * X * . . . . # 
# . . . . . . . . . # 
# . X * X v . . . . # 
# . . X . . . . . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# # # # # # # # # # # 
# # # # # # # # # # # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . X . X . . . . # 
# . X * . * X . . . # 
# . . . . . . . . . # 
# . X * X * . . . . # 
# . . . . . . . . . # 
# . X * X v . . . . # 
# . . X . X . . . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# # # # # # # # # # # 
# # # # # # # # # # # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . X . X . . . . # 
# . X * . * X . . . # 
# . . . . . . . . . # 
# . X * X * . . . . # 
# . . . . . . . . . # 
# . X * X v . . . . # 
# . . X . X . . . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# # # # # # # # # # # 
# # # # # # # # # # # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . X . X . . . . # 
# . X * . * X . . . # 
# . . . . . . . . . # 
# . X * X * . . . . # 
# . . . . . . . . . # 
# . X * X > . . . . # 
# . . X . X . . . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# # # # # # # # # # # 
# # # # # # # # # # # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . X . X . . . . # 
# . X * . * X . . . # 
# . . . . . . . . . # 
# . X * X * . . . . # 
# . . . . . . . . . # 
# . X * X > . . . . # 
# . . X . X . . . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# # # # # # # # # # # 
# # # # # # # # # # # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . X . X . . . . # 
# . X * . * X . . . # 
# . . . . . . . . . # 
# . X * X * . . . . # 
# . . . . . . . . . # 
# . X * X > . . . . # 
# . . X . X . . . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# # # # # # # # # # # 
# # # # # # # # # # # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . X . X . . . . # 
# . X * . * X . . . # 
# . . . . . . . . . # 
# . X * X * . . . . # 
# . . . . . . . . . # 
# . X * X * . > . . # 
# . . X . X . . . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# # # # # # # # # # # 
# # # # # # # # # # # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . X . X . . . . # 
# . X * . * X . . . # 
# . . . . . . . . . # 
# . X * X * . . . . # 
# . . . . . . . . . # 
# . X * X * . > X . # 
# . . X . X . X . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# # # # # # # # # # # 
# # # # # # # # # # # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . X . X . . . . # 
# . X * . * X . . . # 
# . . . . . . . . . # 
# . X * X * . . . . # 
# . . . . . . . . . # 
# . X * X * . > X . # 
# . . X . X . X . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# # # # # # # # # # # 
# # # # # # # # # # # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . X . X . . . . # 
# . X * . * X . . . # 
# . . . . . . . . . # 
# . X * X * . . . . # 
# . . . . . . . . . # 
# . X * X * . ^ X . # 
# . . X . X . X . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# # # # # # # # # # # 
# # # # # # # # # # # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . X . X . . . . # 
# . X * . * X . . . # 
# . . . . . . . . . # 
# . X * X * . . . . # 
# . . . . . . . . . # 
# . X * X * . ^ X . # 
# . . X . X . X . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# # # # # # # # # # # 
# # # # # # # # # # # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . X . X . . . . # 
# . X * . * X . . . # 
# . . . . . . . . . # 
# . X * X * . . . . # 
# . . . . . . . . . # 
# . X * X * . ^ X . # 
# . . X . X . X . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# # # # # # # # # # # 
# # # # # # # # # # # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . X . X . . . . # 
# . X * . * X . . . # 
# . . . . . . . . . # 
# . X * X * . ^ . . # 
# . . . . . . . . . # 
# . X * X * . * X . # 
# . . X . X . X . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# # # # # # # # # # # 
# # # # # # # # # # # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . X . X . . . . # 
# . X * . * X . . . # 
# . . . . . . . . . # 
# . X * X * . ^ X . # 
# . . . . . . . . . # 
# . X * X * . * X . # 
# . . X . X . X . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# # # # # # # # # # # 
# # # # # # # # # # # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . X . X . . . . # 
# . X * . * X . . . # 
# . . . . . . . . . # 
# . X * X * . ^ X . # 
# . . . . . . . . . # 
# . X * X * . * X . # 
# . . X . X . X . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# # # # # # # # # # # 
# # # # # # # # # # # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . X . X . . . . # 
# . X * . * X . . . # 
# . . . . . . . . . # 
# . X * X * . ^ X . # 
# . . . . . . . . . # 
# . X * X * . * X . # 
# . . X . X . X . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# # # # # # # # # # # 
# # # # # # # # # # # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . X . X . . . . # 
# . X * . * X ^ . . # 
# . . . . . . . . . # 
# . X * X * . * X . # 
# . . . . . . . . . # 
# . X * X * . * X . # 
# . . X . X . X . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# # # # # # # # # # # 
*/