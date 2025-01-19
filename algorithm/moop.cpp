#include <iostream>
#include <vector>
#include <stack>
#include <unordered_map>
#include "Tile.cpp"
#include "Map.cpp"
using namespace std;


vector<vector<char> > RealMaze = {
        {'#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#'},
        {'#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#'}, // 1.
        {'#', ' ', '#', ' ', '#', ' ', '#', '#', '#', '#', '#'},
        {'#', ' ', '#', ' ', '#', ' ', ' ', ' ', '#', ' ', '#'}, // 3.
        {'#', ' ', '#', '#', '#', ' ', '#', ' ', '#', '#', '#'},
        {'#', ' ', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', '#'}, // 5.
        {'#', ' ', '#', '#', '#', ' ', '#', '#', ' ', ' ', '#'},
        {'#', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#'}, // 7.
        {'#', ' ', '#', ' ', '#', ' ', ' ', ' ', ' ', ' ', '#'},
        {'#', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', '#'}, // 9.
        {'#', ' ', '#', ' ', '#', ' ', '#', ' ', '#', ' ', '#'},
        {'#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#'}, // 1.
        {'#', ' ', '#', '#', '#', ' ', '#', '#', '#', ' ', '#'},
        {'#', ' ', ' ', ' ', '#', ' ', '#', ' ', ' ', ' ', '#'}, // 3.
        {'#', ' ', '#', '#', '#', ' ', '#', '#', '#', ' ', '#'},
        {'#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#'}, // 5.
        {'#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#'}
};

vector<vector<char> > UndescoverdMazeSecondLevel = {
    {'#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#'},
    {'#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#'}, // 1.
    {'#', ' ', '#', ' ', '#', ' ', '#', '#', '#', '#', '#'},
    {'#', ' ', '#', ' ', '#', ' ', ' ', ' ', '#', ' ', '#'}, // 3.
    {'#', ' ', '#', '#', '#', ' ', '#', ' ', '#', '#', '#'},
    {'#', ' ', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', '#'}, // 5.
    {'#', ' ', '#', '#', '#', ' ', '#', '#', ' ', ' ', '#'},
    {'#', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#'}, // 7.
    {'#', ' ', '#', ' ', '#', ' ', ' ', ' ', ' ', ' ', '#'},
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
    {'.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.'},
    {'.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.'}, // 1.
    {'.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.'},
    {'.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.'}, // 3.
    {'.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.'},
    {'.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.'}, // 5.
    {'.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.'},
    {'.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.'}, // 7.
    {'.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.'},
    {'.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.'}, // 9.
    {'.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.'},
    {'.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.'}, // 1.
    {'.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.'},
    {'.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.'}, // 3.
    {'.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.'},
    {'.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.'}, // 5.
    {'.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.'}
};
int robotOrientation = 0;
coord robotCoord = coord{1, 1, 1};
char robotChar = 'v';
constexpr TileDirection directions[] = {TileDirection::kUp, TileDirection::kDown, TileDirection::kLeft, TileDirection::kRight};

void turnCharL(const char robo){
    if(robo == '<'){
        robotChar = 'v';
    }else if(robo == 'v'){
        robotChar = '>';
    }else if(robo == '>'){
        robotChar = '^';
    }else if(robo == '^'){
        robotChar = '<';
    }
}
void turnCharR(const char robo){
    if(robo == '<'){
        robotChar = '^';
    }else if(robo == '^'){
        robotChar = '>';
    }else if(robo == '>'){
        robotChar = 'v';
    }else if(robo == 'v'){
        robotChar = '<';
    }
}
void turnRobot(const int targetOrientation) {
    int difference = targetOrientation - robotOrientation;
    if (difference == 0) {
        return;
    }
    if (difference == 90) {
        turnCharR(robotChar);
        robotOrientation = (robotOrientation + 90) % 360;
    } else if (difference == 270) {
        turnCharL(robotChar);
        robotOrientation = (robotOrientation + 270) % 360;
    } else if (difference == 180) {
        turnCharR(robotChar);
        turnCharR(robotChar);
        robotOrientation = (robotOrientation + 180) % 360;
    }
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
        robotCoord = next;
        discoverMaze[robotCoord.y][robotCoord.x] = robotChar;
    }
}
bool checkForWall(const vector<vector<char> >& maze, const TileDirection& direction, const coord& currentTileCoord) {
    switch(direction) {
        case TileDirection::kRight:
            return maze[currentTileCoord.y][currentTileCoord.x + 1] == '#';
        case TileDirection::kUp:
            return maze[currentTileCoord.y + 1][currentTileCoord.x] == '#';
        case TileDirection::kLeft:
            return maze[currentTileCoord.y][currentTileCoord.x - 1] == '#';
        case TileDirection::kDown:
            return maze[currentTileCoord.y - 1][currentTileCoord.x] == '#';
    }
}
/*
int checkRamp(const vector<vector<char> >& maze, const Directions& direction, const coord& currentTileCoord) {
    int z = currentTileCoord.z;
    switch(direction) {
        case Directions::right:
            if (maze[currentTileCoord.y][currentTileCoord.x] == 'r') {
                z++;
            } else if (currentTileCoord.x+2 <= 10 && maze[currentTileCoord.y][currentTileCoord.x+2] == 'l' && currentTileCoord.z > 1) {
                z--;
            }
            break;
        case Directions::up:
            if (maze[currentTileCoord.y][currentTileCoord.x] == 'u') {
                z++;
            } else if (currentTileCoord.y+2 <= 6 && maze[currentTileCoord.y+2][currentTileCoord.x] == 'd' && currentTileCoord.z > 1) {
                z--;
            }
            break;
        case Directions::left:
            if (maze[currentTileCoord.y][currentTileCoord.x] == 'l') {
                z++;
            } else if (currentTileCoord.x-2 >= 0 && maze[currentTileCoord.y][currentTileCoord.x-2] == 'r' && currentTileCoord.z > 1) {
                z--;
            }
            break;
        case Directions::down:
            if (maze[currentTileCoord.y][currentTileCoord.x] == 'd') {
                z++;
            } else if (currentTileCoord.y-2 >= 0 && maze[currentTileCoord.y-2][currentTileCoord.x] == 'u' && currentTileCoord.z > 1) {
                z--;
            }
            break;
    }
    return z;
}*/
void dijsktra(const coord& start, const coord& end, const Map& tilesMap, const vector<Tile>& tiles) {
    vector<bool> explored;
    vector<int> distance;
    vector<coord> previousPositions;
    stack<coord> path;
    // initialize distance.
    for (int i = tilesMap.positions.size() - 1; i >= 0; --i) {
        distance.push_back(INT_MAX);
        explored.push_back(false);
        //previousPositions.push_back(coord{0, 0, 0});
    }
    distance[tilesMap.getIndex(start)] = 0;
    explored[tilesMap.getIndex(start)] = true;
    coord currentCoord = start;
    int minDistance;
    while (!explored[tilesMap.getIndex(end)]) {
        // update distance.
        for (const TileDirection& direction : directions) {
            const Tile& currentTile = tiles[tilesMap.getIndex(currentCoord)];
            const coord& adjacentCoord = currentTile.adjacentTiles_[static_cast<int>(direction)]->position_;
            // check if there's an adjecent tile and there's no wall.
            //TODO: check if the tile to explore is black
            if (currentTile.adjacentTiles_[static_cast<int>(direction)] != NULL && !currentTile.hasWall(direction)) {
                const int weight = currentTile.weights_[static_cast<int>(direction)] + distance[tilesMap.getIndex(currentCoord)];
                // check if the new weight to visit the adjecent tile is less than the current weight.
                if (weight < distance[tilesMap.getIndex(adjacentCoord)]) {
                    distance[tilesMap.getIndex(adjacentCoord)] = weight;
                    previousPositions[tilesMap.getIndex(adjacentCoord)] = currentCoord;
                }
            }
        }
        // find next tile.
        minDistance = INT_MAX;
        for (int i = tilesMap.positions.size() - 1; i >= 0; --i) {
            const coord& current = tilesMap.positions[i];
            const int currentDistance = distance[tilesMap.getIndex(current)];
            if (currentDistance < minDistance && !explored[tilesMap.getIndex(current)]) {
                minDistance = currentDistance;
                currentCoord = current;
            }
        }         explored[tilesMap.getIndex(currentCoord)] = true;
    }
    // find path.
    coord current = end;
    while (current != start) {
        path.push(current);
        current = previousPositions[tilesMap.getIndex(current)];
    }    path.push(start);
     // followPath(path);
}

void dfsSearch(Map& tilesMap, vector<Tile> tiles){
    cout<<robotCoord.x;
    Map visitedMap = Map();
    vector<bool> visited;
    stack<coord> unvisited;
    Tile* currentTile;
    bool wall;
    bool alreadyConnected;
    bool visitedFlag;
    coord nextCoord;
    TileDirection oppositeDir;
    unvisited.push(robotCoord);
    #if DEBUG_ALGORITHM
    cout<<"Inicio DFS ";
    #endif
    
    while(!unvisited.empty()){
        coord currentCoord = unvisited.top();
        unvisited.pop();
        visitedFlag = false;
        for (int i = 0; i < visitedMap.positions.size(); ++i) {
            if (visitedMap.positions[i] == currentCoord) {
                visitedFlag = true;
                break;
            }
        }
        if (visitedFlag) {
            continue;
        }
        dijsktra(robotCoord, currentCoord, tilesMap, tiles);
        #if DEBUG_ALGORITHM
        cout<<"Current coord" << currentCoord.x << " " << currentCoord.y << " " << currentCoord.z << endl;
        #endif

        robotCoord = currentCoord;
        visitedMap.positions.push_back(currentCoord);
        visited.push_back(true);
        for(const TileDirection direction: directions){
            wall = false;
            switch(direction) {
                case TileDirection::kRight:
                    nextCoord = coord{currentCoord.x + 2, currentCoord.y, 1}; // checkRamp(direction);
                    currentTile = &tiles[tilesMap.getIndex(currentCoord)];
                    oppositeDir = TileDirection::kLeft;
                    break;
                case TileDirection::kUp:
                    nextCoord = coord{currentCoord.x, currentCoord.y + 2, 1}; // checkRamp(direction);
                    currentTile = &tiles[tilesMap.getIndex(currentCoord)];
                    oppositeDir = TileDirection::kDown;
                    break;
                case TileDirection::kLeft:
                    nextCoord = coord{currentCoord.x - 2, currentCoord.y, 1}; // checkRamp(direction);
                    currentTile = &tiles[tilesMap.getIndex(currentCoord)];
                    oppositeDir = TileDirection::kRight;
                    break;
                case TileDirection::kDown:
                    nextCoord = coord{currentCoord.x, currentCoord.y - 2, 1}; // checkRamp(direction);
                    currentTile = &tiles[tilesMap.getIndex(currentCoord)];
                    oppositeDir = TileDirection::kUp;
                    break;
            if(currentTile->adjacentTiles_[static_cast<int>(direction)] == NULL){
                wall = false;
                tilesMap.positions.push_back(nextCoord);
                tiles[tilesMap.getIndex(nextCoord)] = Tile(nextCoord);
                Tile* nextTile = &tiles[tilesMap.getIndex(nextCoord)];
                if(nextTile->position_ == kInvalidPosition){
                    nextTile->setPosition(nextCoord);
                }

                currentTile->addAdjacentTile(direction, nextTile, wall);
                nextTile->addAdjacentTile(oppositeDir, currentTile, wall);

                if(!wall){
                    visitedFlag = false;
                    for(int i = 0; i < visitedMap.positions.size(); ++i){
                        if(visitedMap.positions[i] == nextCoord){
                            visitedFlag = true;
                            break;
                        }
                    }
                    if(!visitedFlag){
                        unvisited.push(nextCoord);
                    }
                }
                if(!visitedFlag){
                    #if DEBUG_ALGORITHM
                    cout<< "finished loop";
                    #endif
                    unvisited.push(nextCoord);
                }
                //comments this 
                if (currentCoord.z == 1 && nextCoord.z == 1) {
                    wall = checkForWall(RealMaze, direction, currentCoord);
                }
                tilesMap.positions.push_back(nextCoord);
                tiles[tilesMap.getIndex(nextCoord)] = Tile(nextCoord);
                Tile* nextTile = &tiles[tilesMap.getIndex(nextCoord)];
                if (nextTile->position_ == kInvalidPosition) {
                    nextTile->setPosition(nextCoord);
                }
                currentTile->addAdjacentTile(direction, nextTile, wall);
                nextTile->addAdjacentTile(oppositeDir, currentTile, wall);
                if (!wall) {
                    visitedFlag = false;
                    for (int i = 0; i < visitedMap.positions.size(); ++i) {
                        if (visitedMap.positions[i] == nextCoord) {
                            visitedFlag = true;
                            break;
                        }
                    }
                    if (!visitedFlag) {
                        unvisited.push(nextCoord);
                    }
                }
                
            }
        }
    }
    #if DEBUG_ALGORITHM
    cout<<"Finished DFS";
    #endif
    }
}

void startAlgorithm(){
    Map tilesMap = Map();
    vector<Tile> tiles = vector<Tile>(100);
    tilesMap.positions.push_back(robotCoord);
    //tiles[tilesMap.getIndex(robotCoord)] = Tile(robotCoord);
    int index = tilesMap.getIndex(robotCoord);
    if (index >= 0 && index < tiles.size()) {
        tiles[index] = Tile(robotCoord);
    }

    #if DEBUG_ALGORITHM
    cout<<"Start DFS";
    #endif
    dfsSearch(tilesMap, tiles);
}