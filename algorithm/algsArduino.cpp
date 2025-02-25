#include "coord.h"
#include "Tile.h"
#include "arrCustom.h"
#include "Stack.h"

char RealMaze[17][11] = {
        {'#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#'},
        {'#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#'}, // 1.
        {'#', ' ', '.', ' ', '.', ' ', '.', ' ', '.', ' ', '#'},
        {'#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#'}, // 3.
        {'#', ' ', '.', '#', '.', '#', '.', '#', '.', ' ', '#'},
        {'#', ' ', '#', ' ', ' ', ' ', ' ', ' ', '#', ' ', '#'}, // 5.
        {'#', ' ', '.', ' ', '.', '#', '.', '#', '.', ' ', '#'},
        {'#', ' ', '#', ' ', ' ', ' ', ' ', ' ', '#', ' ', '#'}, // 7.
        {'#', ' ', '.', ' ', '.', ' ', '.', ' ', '.', ' ', '#'},
        {'#', ' ', '#', ' ', ' ', ' ', ' ', ' ', '#', ' ', '#'}, // 9.
        {'#', ' ', '.', ' ', '.', ' ', '.', ' ', '.', ' ', '#'},
        {'#', ' ', '#', ' ', ' ', ' ', ' ', ' ', '#', ' ', '#'}, // 1.
        {'#', ' ', '.', '#', '.', '#', '.', '#', '.', ' ', '#'},
        {'#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#'}, // 3.
        {'#', ' ', '.', ' ', '.', ' ', '.', ' ', '.', ' ', '#'},
        {'#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#'}, // 5.
        {'#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#'}
};
/*Izq:'<'
Der: '>'
Arriba: '^'
Abajo: 'v'
*/
char discoverMaze[17][11] ={
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
    {'#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#'}
};

int steps = 0;
coord inicio = {3,5,1};
coord robotCoord = {3,5,1};
char robotChar = 'v';
TileDirection directions[4] = {TileDirection::kLeft,TileDirection::kDown,TileDirection::kRight,TileDirection::kUp};
int robotOrientation = 0;
const int kMaxSize = 30; 

void printMaze(char maze[17][11]){
    for(int i = 0; i < 17; ++i){
        for(int j = 0; j < 11; j++){
            printf("%c ", maze[i][j]);
            //cout << maze[i][j] << " ";
        }
        printf("\n");
    }
}

void ahead(coord next){
    discoverMaze[robotCoord.y][robotCoord.x] = '*';
    robotCoord = next;
    discoverMaze[robotCoord.y][robotCoord.x] = robotChar;
    printMaze(discoverMaze);
    steps++;
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
bool checkForWall(const char maze[17][11], const TileDirection& direction, const coord& robotCoord) {
    switch(direction) {
        case TileDirection::kRight:
            return maze[robotCoord.y][robotCoord.x + 1] == '#';
        case TileDirection::kUp:
            return maze[robotCoord.y + 1][robotCoord.x] == '#';
        case TileDirection::kLeft:
            return maze[robotCoord.y][robotCoord.x - 1] == '#';
        case TileDirection::kDown:
            return maze[robotCoord.y - 1][robotCoord.x] == '#';
        default: 
            break;
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
    steps++;
}
void followPath(Stack& path){
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
        robotCoord = next;
        //printMaze(discoverMaze);
    }
}
void dijkstra(coord& start, coord& end, arrCustom<coord>& tilesMap, arrCustom<Tile> tiles){
    Stack path;
    
    arrCustom<bool> explored(tilesMap.getSize(), false);
    arrCustom<int> distance(tilesMap.getSize(), INT_MAX);
    arrCustom<coord> previousPositions(tilesMap.getSize(), kInvalidPosition);

    distance.set(tilesMap.getIndex(start), 0);
    explored.set(tilesMap.getIndex(start), true);
    int minDist;
    coord current = start;
    while(!explored.getValue(tilesMap.getIndex(end))){
        
        
        for(const TileDirection& direction : directions){
            const Tile& currentTile = tiles.getValue(tilesMap.getIndex(current));
            const coord& adjacent = currentTile.adjacentTiles_[static_cast<int>(direction)]->position_;
            // find the distance to the adjacent tile
            //printf("llegue");
            if(currentTile.adjacentTiles_[static_cast<int>(direction)] != nullptr && !currentTile.hasWall(direction) ){//&& currentTile.weights_[static_cast<int>(direction)] != NULL){
                const int weight = currentTile.weights_[static_cast<int>(direction)] +distance.getValue(tilesMap.getIndex(current));
                //int adjacentIndex = tilesMap.getIndex(adjacent);
                //if(adjacentIndex != -1){
                //if(adjacent != kInvalidPosition){
                    int index = distance.getValue(tilesMap.getIndex(adjacent));
                    if(weight < distance.getValue(tilesMap.getIndex(adjacent))){
                        distance.set(tilesMap.getIndex(adjacent),weight);
                        previousPositions.set(tilesMap.getIndex(adjacent), current);
                        printf("llegue4");
                    }
                //}
                //}
            }
        }
        minDist = INT_MAX;
        //find the minimum distance to the path line
        for(int i = tilesMap.getSize() -1; i >= 0; i--){
            const coord& currentCoord = tilesMap.getValue(i);
            const int currentDistance = distance.getValue(tilesMap.getIndex(currentCoord));
            /*
            if(currentCoord == current){
                continue;
            }
            */
            if(currentDistance < minDist && !explored.getValue(tilesMap.getIndex(currentCoord))){
                minDist = currentDistance;
                current = currentCoord;
            } else {
                //printf("llegue3");

            }
        }
        explored.set(tilesMap.getIndex(current),true);
    }
    current = end;
    while(current != start){
        path.push(current);
        current = previousPositions.getValue(tilesMap.getIndex(current));
    }
    for (int i = 0; i < 10; ++i) {
        coord pos = tilesMap.getValue(i);
        coord prev = previousPositions.getValue(i);
        printf("Tile at (%d, %d) comes from (%d, %d)\n", pos.x, pos.y, prev.x, prev.y);
    }
    
    //path.push(start);
    followPath(path);
}

void dfs(arrCustom<coord>& visitedMap, arrCustom<Tile>& tiles, arrCustom<coord>& tilesMap){
    Stack unvisited;
    arrCustom<bool> visited(visitedMap.getSize(), false);
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
        for(int i = 0; i < visitedMap.getSize(); ++i){
            if(visitedMap.getValue(i) == current){
                visitedFlag = true;
                break;
            }
        }
        if (visitedFlag) {
            continue;
        }
        dijkstra(robotCoord, current, tilesMap, tiles);
        visitedMap.push_back(current);
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
                    currentTile = &tiles.getValue(tilesMap.getIndex(current));
                    wallCoord = {current.x + 1, current.y, 1};
                    oppositeDirection = TileDirection::kLeft;
                    break;
                case TileDirection::kUp:
                    next = coord{current.x, current.y + 2, 1};
                    currentTile = &tiles.getValue(tilesMap.getIndex(current));
                    wallCoord = {current.x, current.y + 1, 1};
                    oppositeDirection = TileDirection::kDown;
                    break;
                case TileDirection::kLeft:
                    next = coord{current.x - 2, current.y, 1};
                    currentTile = &tiles.getValue(tilesMap.getIndex(current));
                    wallCoord = {current.x - 1, current.y, 1};
                    oppositeDirection = TileDirection::kRight;
                    break;
                case TileDirection::kDown:
                    next = coord{current.x, current.y - 2, 1};
                    currentTile = &tiles.getValue(tilesMap.getIndex(current));
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
                tilesMap.push_back(next);
                tiles.set(tilesMap.getIndex(next), Tile(next));
                Tile* nextTile = &tiles.getValue(tilesMap.getIndex(next));
                
                if(nextTile->position_ == kInvalidPosition){
                    nextTile->setPosition(next);
                }
                
                int weight = 1; // Default weight for normal movement
                if (wall) {
                    weight = 100; // High weight for walls (impossible to pass)
                }
                // join the tiles and if there is no wall between them
                currentTile -> addAdjacentTile(direction, nextTile, wall);
                nextTile -> addAdjacentTile(oppositeDirection, currentTile, wall);
                currentTile->weights_[static_cast<int>(direction)] = weight;
                nextTile->weights_[static_cast<int>(oppositeDirection)] = weight;
                if(!wall){
                    visitedFlag = false;
                    for(int i = 0; i < visitedMap.getSize(); ++i){
                        if(visitedMap.getValue(i) == next){
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
    dijkstra(robotCoord, inicio, tilesMap, tiles);
}
int main(){

    arrCustom<coord> visitedMap(kMaxSize, kInvalidPosition);
    arrCustom<coord> tilesMap(kMaxSize, kInvalidPosition);
    arrCustom<Tile> tiles(kMaxSize, Tile(kInvalidPosition));
    tilesMap.push_back(robotCoord);
    tiles.set(tilesMap.getIndex(robotCoord),Tile(robotCoord));
    printf("DFS Iniciado");
    dfs(visitedMap, tiles, tilesMap);
    printf("DFS Finalizaado");  
    printf("DFS Finalizaado con %d movimienots", steps); 
    return 0;
}

/*
Output:
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
# . X * X . . . . . # 
# . . . . . . . . . # 
# . . v . . . . . . # 
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
# . X * X . . . . . # 
# . . . . . . . . . # 
# . . v . . . . . . # 
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
# . X * X . . . . . # 
# . . . . . . . . . # 
# . X v . . . . . . # 
# . . X . . . . . . # 
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
# . X * X . . . . . # 
# . . . . . . . . . # 
# . X v . . . . . . # 
# . . X . . . . . . # 
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
# . X * X . . . . . # 
# . . . . . . . . . # 
# . X > . . . . . . # 
# . . X . . . . . . # 
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
# . X * X . . . . . # 
# . . . . . . . . . # 
# . X * . > . . . . # 
# . . X . . . . . . # 
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
# . X * X . . . . . # 
# . . . . . . . . . # 
# . X * . > . . . . # 
# . . X . . . . . . # 
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
# . X * X . . . . . # 
# . . . . X . . . . # 
# . X * . > . . . . # 
# . . X . X . . . . # 
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
# . X * X . . . . . # 
# . . . . X . . . . # 
# . X * . > . . . . # 
# . . X . X . . . . # 
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
# . X * X . . . . . # 
# . . . . X . . . . # 
# . X * . * . > . . # 
# . . X . X . . . . # 
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
# . X * X . . . . . # 
# . . . . X . . . . # 
# . X * . * . > . . # 
# . . X . X . . . . # 
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
# . X * X . . . . . # 
# . . . . X . . . . # 
# . X * . * . > X . # 
# . . X . X . X . . # 
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
# . X * X . . . . . # 
# . . . . X . . . . # 
# . X * . * . > X . # 
# . . X . X . X . . # 
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
# . X * X . . . . . # 
# . . . . X . . . . # 
# . X * . * . ^ X . # 
# . . X . X . X . . # 
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
# . X * X . . ^ . . # 
# . . . . X . . . . # 
# . X * . * . * X . # 
# . . X . X . X . . # 
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
# . X * X . . ^ . . # 
# . . . . X . . . . # 
# . X * . * . * X . # 
# . . X . X . X . . # 
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
# . X * X . . ^ X . # 
# . . . . X . . . . # 
# . X * . * . * X . # 
# . . X . X . X . . # 
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
# . X * X . . ^ X . # 
# . . . . X . . . . # 
# . X * . * . * X . # 
# . . X . X . X . . # 
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
# . X * X . . < X . # 
# . . . . X . . . . # 
# . X * . * . * X . # 
# . . X . X . X . . # 
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
# . X * X < . * X . # 
# . . . . X . . . . # 
# . X * . * . * X . # 
# . . X . X . X . . # 
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
# . X * X < . * X . # 
# . . . . X . . . . # 
# . X * . * . * X . # 
# . . X . X . X . . # 
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
# . X * X < . * X . # 
# . . . . X . . . . # 
# . X * . * . * X . # 
# . . X . X . X . . # 
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
# . X * X < . * X . # 
# . . . . X . . . . # 
# . X * . * . * X . # 
# . . X . X . X . . # 
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
# . X * X ^ . * X . # 
# . . . . X . . . . # 
# . X * . * . * X . # 
# . . X . X . X . . # 
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
# . X * X ^ . . . . # 
# . . . . . . . . . # 
# . X * X * . * X . # 
# . . . . X . . . . # 
# . X * . * . * X . # 
# . . X . X . X . . # 
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
# . X * X ^ . . . . # 
# . . . . . . . . . # 
# . X * X * . * X . # 
# . . . . X . . . . # 
# . X * . * . * X . # 
# . . X . X . X . . # 
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
# . X * X ^ . . . . # 
# . . . . . . . . . # 
# . X * X * . * X . # 
# . . . . X . . . . # 
# . X * . * . * X . # 
# . . X . X . X . . # 
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
# . X * X ^ . . . . # 
# . . . . . . . . . # 
# . X * X * . * X . # 
# . . . . X . . . . # 
# . X * . * . * X . # 
# . . X . X . X . . # 
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
# . X * X > . . . . # 
# . . . . . . . . . # 
# . X * X * . * X . # 
# . . . . X . . . . # 
# . X * . * . * X . # 
# . . X . X . X . . # 
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
# . X * X * . > . . # 
# . . . . . . . . . # 
# . X * X * . * X . # 
# . . . . X . . . . # 
# . X * . * . * X . # 
# . . X . X . X . . # 
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
# . X * X * . > . . # 
# . . . . . . . . . # 
# . X * X * . * X . # 
# . . . . X . . . . # 
# . X * . * . * X . # 
# . . X . X . X . . # 
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
# . X * X * . > X . # 
# . . . . . . . . . # 
# . X * X * . * X . # 
# . . . . X . . . . # 
# . X * . * . * X . # 
# . . X . X . X . . # 
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
# . X * X * . > X . # 
# . . . . . . . . . # 
# . X * X * . * X . # 
# . . . . X . . . . # 
# . X * . * . * X . # 
# . . X . X . X . . # 
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
# . X * X * . ^ X . # 
# . . . . . . . . . # 
# . X * X * . * X . # 
# . . . . X . . . . # 
# . X * . * . * X . # 
# . . X . X . X . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# # # # # # # # # # # 
# # # # # # # # # # # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . X . . . . . . # 
# . X * . . . ^ . . # 
# . . . . . . . . . # 
# . X * X * . * X . # 
# . . . . . . . . . # 
# . X * X * . * X . # 
# . . . . X . . . . # 
# . X * . * . * X . # 
# . . X . X . X . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# # # # # # # # # # # 
# # # # # # # # # # # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . X . . . . . . # 
# . X * . . . ^ . . # 
# . . . . . . . . . # 
# . X * X * . * X . # 
# . . . . . . . . . # 
# . X * X * . * X . # 
# . . . . X . . . . # 
# . X * . * . * X . # 
# . . X . X . X . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# # # # # # # # # # # 
# # # # # # # # # # # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . X . . . X . . # 
# . X * . . X ^ X . # 
# . . . . . . . . . # 
# . X * X * . * X . # 
# . . . . . . . . . # 
# . X * X * . * X . # 
# . . . . X . . . . # 
# . X * . * . * X . # 
# . . X . X . X . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# # # # # # # # # # # 
# # # # # # # # # # # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . X . . . X . . # 
# . X * . . X ^ X . # 
# . . . . . . . . . # 
# . X * X * . * X . # 
# . . . . . . . . . # 
# . X * X * . * X . # 
# . . . . X . . . . # 
# . X * . * . * X . # 
# . . X . X . X . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# # # # # # # # # # # 
# # # # # # # # # # # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . X . . . X . . # 
# . X * . . X v X . # 
# . . . . . . . . . # 
# . X * X * . * X . # 
# . . . . . . . . . # 
# . X * X * . * X . # 
# . . . . X . . . . # 
# . X * . * . * X . # 
# . . X . X . X . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# # # # # # # # # # # 
# # # # # # # # # # # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . X . . . X . . # 
# . X * . . X * X . # 
# . . . . . . . . . # 
# . X * X * . v X . # 
# . . . . . . . . . # 
# . X * X * . * X . # 
# . . . . X . . . . # 
# . X * . * . * X . # 
# . . X . X . X . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# # # # # # # # # # # 
# # # # # # # # # # # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . X . . . X . . # 
# . X * . . X * X . # 
# . . . . . . . . . # 
# . X * X * . v X . # 
# . . . . . . . . . # 
# . X * X * . * X . # 
# . . . . X . . . . # 
# . X * . * . * X . # 
# . . X . X . X . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# # # # # # # # # # # 
# # # # # # # # # # # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . X . . . X . . # 
# . X * . . X * X . # 
# . . . . . . . . . # 
# . X * X * . < X . # 
# . . . . . . . . . # 
# . X * X * . * X . # 
# . . . . X . . . . # 
# . X * . * . * X . # 
# . . X . X . X . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# # # # # # # # # # # 
# # # # # # # # # # # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . X . . . X . . # 
# . X * . . X * X . # 
# . . . . . . . . . # 
# . X * X < . * X . # 
# . . . . . . . . . # 
# . X * X * . * X . # 
# . . . . X . . . . # 
# . X * . * . * X . # 
# . . X . X . X . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# # # # # # # # # # # 
# # # # # # # # # # # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . X . . . X . . # 
# . X * . . X * X . # 
# . . . . . . . . . # 
# . X * X < . * X . # 
# . . . . . . . . . # 
# . X * X * . * X . # 
# . . . . X . . . . # 
# . X * . * . * X . # 
# . . X . X . X . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# # # # # # # # # # # 
# # # # # # # # # # # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . X . . . X . . # 
# . X * . . X * X . # 
# . . . . . . . . . # 
# . X * X ^ . * X . # 
# . . . . . . . . . # 
# . X * X * . * X . # 
# . . . . X . . . . # 
# . X * . * . * X . # 
# . . X . X . X . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# # # # # # # # # # # 
# # # # # # # # # # # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . X . . . X . . # 
# . X * . ^ X * X . # 
# . . . . . . . . . # 
# . X * X * . * X . # 
# . . . . . . . . . # 
# . X * X * . * X . # 
# . . . . X . . . . # 
# . X * . * . * X . # 
# . . X . X . X . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# # # # # # # # # # # 
# # # # # # # # # # # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . X . . . X . . # 
# . X * . ^ X * X . # 
# . . . . . . . . . # 
# . X * X * . * X . # 
# . . . . . . . . . # 
# . X * X * . * X . # 
# . . . . X . . . . # 
# . X * . * . * X . # 
# . . X . X . X . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# # # # # # # # # # # 
DFS Finalizaado con 59 movimienots% 
*/