#include "coord.h"
#include "Tile.h"
#include "arrCustom.h"
#include "Stack.h"
#include "heap.h"

// center of tile, what is at the bottom of the tile
// sides, what is at the walls of the tile.
char RealMaze[17][11] = {
        {'#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#'},
        {'#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#'}, // 1.
        {'#', ' ', '.', '#', '.', '#', '.', '#', '.', ' ', '#'},
        {'#', ' ', '#', ' ', ' ', ' ', ' ', ' ', '#', ' ', '#'}, // 3.
        {'#', ' ', '.', ' ', '.', '#', '.', '#', '.', ' ', '#'},
        {'#', ' ', '#', ' ', ' ', ' ', ' ', ' ', '#', ' ', '#'}, // 5.
        {'#', ' ', '.', ' ', '.', ' ', '.', ' ', '.', ' ', '#'},
        {'#', ' ', '#', ' ', ' ', ' ', ' ', 'B', '#', ' ', '#'}, // 7.
        {'#', ' ', '.', ' ', '.', ' ', '.', ' ', '.', ' ', '#'},
        {'#', ' ', '#', ' ', '#', ' ', ' ', 'b', '#', ' ', '#'}, // 9.
        {'#', ' ', '.', ' ', '.', ' ', '.', ' ', '.', ' ', '#'},
        {'#', ' ', '#', 'h', ' ', ' ', ' ', ' ', '#', ' ', '#'}, // 1.
        {'#', ' ', '.', '#', '.', ' ', '.', ' ', '.', ' ', '#'},
        {'#', ' ', '#', 'B', ' ', ' ', ' ', ' ', '#', ' ', '#'}, // 3.
        {'#', ' ', '.', '#', '.', '#', '.', '#', '.', ' ', '#'},
        {'#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#'}, // 5.
        {'#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#'}
};
char RealMaze2[17][11] = {
    {'#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#'},
    {'#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#'}, // 1.
    {'#', ' ', '.', '#', '.', '#', '.', '#', '.', ' ', '#'},
    {'#', ' ', '#', ' ', ' ', ' ', ' ', ' ', '#', ' ', '#'}, // 3.
    {'#', ' ', '.', ' ', '.', '#', '.', '#', '.', ' ', '#'},
    {'#', ' ', '#', ' ', ' ', ' ', ' ', ' ', '#', ' ', '#'}, // 5.
    {'#', ' ', '.', ' ', '.', ' ', '.', ' ', '.', ' ', '#'},
    {'#', ' ', '#', ' ', ' ', ' ', ' ', ' ', '#', ' ', '#'}, // 7.
    {'#', ' ', '.', ' ', '.', ' ', '.', ' ', '.', ' ', '#'},
    {'#', ' ', '#', ' ', '#', ' ', ' ', ' ', '#', ' ', '#'}, // 9.
    {'#', ' ', '.', ' ', '.', '#', '.', ' ', '.', ' ', '#'},
    {'#', ' ', '#', ' ', ' ', ' ', ' ', 'l', '#', ' ', '#'}, // 1.
    {'#', ' ', '.', '#', '.', '#', '.', ' ', '.', ' ', '#'},
    {'#', ' ', '#', ' ', ' ', ' ', ' ', ' ', '#', ' ', '#'}, // 3.
    {'#', ' ', '.', '#', '.', '#', '.', '#', '.', ' ', '#'},
    {'#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#'}, // 5.
    {'#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#'}
};
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
const int kMaxSize = 100; 
bool blackTile = false;
bool blueTile = false;
int rampState = 0;
//uint8_t, comienza en 128
int level = 0;

void printMaze(char maze[17][11]){
    for(int i = 0; i < 17; ++i){
        for(int j = 0; j < 11; j++){
            printf("%c ", maze[i][j]);
        }
        printf("\n");
    }
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
// logic for algorithm
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
int checkRamp(const char maze[17][11], const coord& robotCoord){
    //high, low ramps- if h, the robot went up, it l, the robot when down in that movement
    if(RealMaze[robotCoord.y][robotCoord.x] == 'h'){
        return 1;
    }
    else if(RealMaze[robotCoord.y][robotCoord.x]== 'l'){
        return 2; 
    }
    return 0;
}
int checkBlackBlue( const char maze[17][11], const coord& robotCoord){
    // Black (B), blue(b)
    if(maze[robotCoord.y][robotCoord.x] == 'B'){
        return 1;
    }else if(maze[robotCoord.y][robotCoord.x] == 'b'){
        return 2;
    }
    return 0;
}
bool checkVictim( const char maze[17][11], const TileDirection& direction, const coord& robotCoord){
    // if victim therefore wall
    switch(direction){
        case TileDirection::kRight:
            return maze[robotCoord.y][robotCoord.x + 1] == 'v';
        case TileDirection::kUp:
            return maze[robotCoord.y + 1][robotCoord.x] == 'v';
        case TileDirection::kLeft:
            return maze[robotCoord.y][robotCoord.x - 1] == 'v';
        case TileDirection::kDown:
            return maze[robotCoord.y - 1][robotCoord.x] == 'v';
        default:
            break;
    }
}
void ahead(coord next){
    if(checkBlackBlue(RealMaze, next) == 1){
        blackTile = true;
    }else{
        if(checkBlackBlue(RealMaze, next) == 2){
            blueTile = true;
            printf("Blue Tile, waiting for 5 seconds \n");
        }
        if(checkRamp(RealMaze, next) == 1){
            rampState = 1;
        }else if(checkRamp(RealMaze, next) == 2){
            rampState = 2;
        }
        discoverMaze[robotCoord.y][robotCoord.x] = '*';
        robotCoord = next;
        discoverMaze[robotCoord.y][robotCoord.x] = robotChar;
        
        printMaze(discoverMaze);
        steps++;
    }
    
}
void changeLevel(){
    if(rampState == 1){
        level++;
    }else if(rampState == 2){
        level--;
    }
    rampState = 0;
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
        if(blackTile){
            continue;
        }
        robotCoord = next;
        //printMaze(discoverMaze);
    }

}
void dijkstra(coord& start, coord& end, arrCustom<coord>& tilesMap, arrCustom<Tile>& tiles){
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
            if(currentTile.adjacentTiles_[static_cast<int>(direction)] != nullptr && !currentTile.hasWall(direction) && !currentTile.hasBlackTile() ){//&& currentTile.weights_[static_cast<int>(direction)] != NULL){
                const int weight = currentTile.weights_[static_cast<int>(direction)] +distance.getValue(tilesMap.getIndex(current));
                int index = distance.getValue(tilesMap.getIndex(adjacent));
                if(weight < distance.getValue(tilesMap.getIndex(adjacent))){
                    distance.set(tilesMap.getIndex(adjacent),weight);
                    previousPositions.set(tilesMap.getIndex(adjacent), current);
                }
            }
        }
        minDist = INT_MAX;
        //find the minimum distance to the path line
        for(int i = 0; i < tilesMap.getSize(); i++){
            const coord& currentCoord = tilesMap.getValue(i);
            const int currentDistance = distance.getValue(tilesMap.getIndex(currentCoord));           
            if(currentDistance < minDist && !explored.getValue(tilesMap.getIndex(currentCoord))){
                minDist = currentDistance;
                current = currentCoord;
            }
        }
        explored.set(tilesMap.getIndex(current),true);
    }
    current = end;
    while(current != start){
        path.push(current);
        current = previousPositions.getValue(tilesMap.getIndex(current));
    }
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
        if(blackTile){
            currentTile = &tiles.getValue(tilesMap.getIndex(current));
            currentTile->setBlackTile();
            blackTile = false;
            continue;
        }
        //change level if ramp
        changeLevel();
        robotCoord = current;
        printf("Current Position: %d %d %d \n", current.x, current.y, current.z);
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
                    wallCoord = {current.x + 1, current.y, level};
                    oppositeDirection = TileDirection::kLeft;
                    break;
                case TileDirection::kUp:
                    next = coord{current.x, current.y + 2, level};
                    currentTile = &tiles.getValue(tilesMap.getIndex(current));
                    wallCoord = {current.x, current.y + 1, 1};
                    oppositeDirection = TileDirection::kDown;
                    break;
                case TileDirection::kLeft:
                    next = coord{current.x - 2, current.y, level};
                    currentTile = &tiles.getValue(tilesMap.getIndex(current));
                    wallCoord = {current.x - 1, current.y, 1};
                    oppositeDirection = TileDirection::kRight;
                    break;
                case TileDirection::kDown:
                    next = coord{current.x, current.y - 2, level};
                    currentTile = &tiles.getValue(tilesMap.getIndex(current));
                    wallCoord = {current.x, current.y - 1, level};
                    oppositeDirection = TileDirection::kUp;
                    break;
            }
            //right();
            //printMaze(discoverMaze);
            //end of regular dfs search
            //start of second level dfs search check if has wall   
            //check for adjacentTiles and connecting them
            
            if (currentTile->adjacentTiles_[static_cast<int>(direction)] == nullptr) {
                int index = tilesMap.getIndex(next);
            
                Tile* nextTile;
                if (index == -1) {  // Tile doesn't exist, create and add it
                    tilesMap.push_back(next);
                    tiles.set(tilesMap.getIndex(next), Tile(next));
                    nextTile = &tiles.getValue(tilesMap.getIndex(next));
                } else {  // Tile exists, retrieve it
                    nextTile = &tiles.getValue(index);
                }
            
                if (nextTile->position_ == kInvalidPosition) {
                    nextTile->setPosition(next);
                }
            
                currentTile->addAdjacentTile(direction, nextTile, wall, false);

                if(blueTile){
                    nextTile->addAdjacentTile(oppositeDirection, nextTile, wall, true);
                    blueTile = false;
                }else{
                    nextTile->addAdjacentTile(oppositeDirection, currentTile, wall, false);
                }
                
            
                if (!wall) {
                    bool visitedFlag = false;
                    for (int i = 0; i < visitedMap.getSize(); ++i) {
                        if (visitedMap.getValue(i) == next) {
                            visitedFlag = true;
                            break;
                        }
                    }
                    if (!visitedFlag) {
                        unvisited.push(next);
                    }
                } else {
                    if (RealMaze[wallCoord.y][wallCoord.x] == '#') {
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