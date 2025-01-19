#include <iostream>
#include "coord.h"
#include "Tile.cpp"
using namespace std;
vector<vector<char> > RealMaze = {
        {'#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#'},
        {'#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#'}, // 1.
        {'#', ' ', '.', ' ', '.', ' ', '.', '#', '.', '#', '#'},
        {'#', ' ', '#', ' ', '#', ' ', ' ', ' ', '#', ' ', '#'}, // 3.
        {'#', ' ', '.', '#', '.', ' ', '.', ' ', '.', '#', '#'},
        {'#', ' ', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', '#'}, // 5.
        {'#', ' ', '.', '#', '.', ' ', '.', '#', '.', ' ', '#'},
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
constexpr TileDirection directions[4] = {TileDirection::kRight, TileDirection::kUp, TileDirection::kLeft, TileDirection::kDown};
char robotChar = 'v';
int robotOrientation = 0;
const int kMaxMapSize = 256;

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
int getIndex(vector<coord> positions, const coord &find) {
    for (uint8_t i = 0; i < positions.size(); ++i) {
        if (positions[i] == find) {
            return i;
        }
    }
    return -1;
}
void dijkstra(coord& start, coord& end,  vector<coord> positions, vector<Tile> tiles){
    vector<bool> explored;
    vector<int> distance;
    vector<coord> previousPositions;
    stack<coord> path;

    for (int i = positions.size() - 1; i >= 0; --i) {
        distance.push_back(INT_MAX);
        explored.push_back(false);
    }
    distance[getIndex(positions, start)] = 0;
    explored[getIndex(positions, start)] = true;

    coord currentCoord = start;
    int minDistance;
    while(!explored[getIndex(positions, end)]){
        for(TileDirection direction : directions){
            Tile& currentTile = tiles[getIndex(positions, currentCoord)];
            coord& adjacentCoord = currentTile.adjacentTiles_[static_cast<int>(direction)]->position_;
            
            if(currentTile.adjacentTiles_[static_cast<int>(direction)] != nullptr && !currentTile.hasWall(direction)){
                int weight = currentTile.weights_[static_cast<int>(direction)] + distance[getIndex(positions, currentCoord)];
                if(weight < distance[getIndex(positions, adjacentCoord)]){
                    distance[getIndex(positions, adjacentCoord)] = weight;
                    previousPositions[getIndex(positions, adjacentCoord)] = currentCoord;
                }
            }
        }
        minDistance = INT_MAX;
        for(int i = positions.size() - 1; i >= 0; --i){
            coord& current = positions[i];
            int currentDistance = distance[getIndex(positions, current)];
            if(currentDistance < minDistance && !explored[getIndex(positions, current)]){
                minDistance = currentDistance;
                currentCoord = current;
            }
        }
        explored[getIndex(positions, currentCoord)] = true;
    }
    coord current = end; 
    while (current != start){
        path.push(current);
        current = previousPositions[getIndex(positions, current)];
        path.push(start);
    }
}

void dfs(vector<coord> positions, vector<Tile> tiles){
    vector<coord> visitedMap;
    vector<bool> visited;
    stack<coord> unvisited;
    Tile* currentTile;
    bool wall;
    bool alreadyConnected;
    bool visitedFlag;
    coord nextCoord;
    TileDirection oppositeDirection;
    unvisited.push(robotCoord);

    while(!unvisited.empty()){
        coord currentCoord = unvisited.top();
        unvisited.pop();

        visitedFlag = false;
        for(int i = 0; i < visitedMap.size(); ++i){
            if(visitedMap[i] == currentCoord){
                visitedFlag = true;
                break;
            }
        }
        if (visitedFlag) {
            continue;
        }
        dijkstra(robotCoord, currentCoord, positions, tiles);
        robotCoord = currentCoord;
        visitedMap.push_back(currentCoord);
        visited.push_back(true);
        for(TileDirection direction: directions){
            wall = false;
            switch(direction) {
                case TileDirection::kRight:
                    nextCoord = coord{currentCoord.x + 2, currentCoord.y, 1}; // checkRamp(direction);
                    currentTile = &tiles[getIndex(positions, currentCoord)];
                    oppositeDirection = TileDirection::kLeft;
                    break;
                case TileDirection::kUp:
                    nextCoord = coord{currentCoord.x, currentCoord.y + 2, 1}; // checkRamp(direction);
                    currentTile = &tiles[getIndex(positions, currentCoord)];
                    oppositeDirection = TileDirection::kDown;
                    break;
                case TileDirection::kLeft:
                    nextCoord = coord{currentCoord.x - 2, currentCoord.y, 1}; // checkRamp(direction);
                    currentTile = &tiles[getIndex(positions, currentCoord)];
                    oppositeDirection = TileDirection::kRight;
                    break;
                case TileDirection::kDown:
                    nextCoord = coord{currentCoord.x, currentCoord.y - 2, 1}; // checkRamp(direction);
                    currentTile = &tiles[getIndex(positions, currentCoord)];
                    oppositeDirection = TileDirection::kUp;
                    break;
            }
            if(currentTile -> adjacentTiles_[static_cast<int>(direction)] == nullptr){
                wall = false;
                positions.push_back(nextCoord);
                tiles[getIndex(positions, nextCoord)] = Tile(nextCoord);
                Tile* nextTile = &tiles[getIndex(positions, nextCoord)];
                if(nextTile -> position_ == kInvalidPosition){
                    nextTile -> setPosition(nextCoord);
                }
                currentTile -> addAdjacentTile(direction, nextTile, wall);
                nextTile -> addAdjacentTile(oppositeDirection, currentTile, wall);
                if(!wall){
                    visitedFlag = false;
                    for(int i = 0; i < visitedMap.size(); ++i){
                        if(visitedMap[i] == nextCoord){
                            visitedFlag = true;
                            break;
                        }
                        if(!visitedFlag){
                            unvisited.push(nextCoord);
                        }
                    }
                }
            }
        }
    }
}
int main(){
    vector<coord> tilesMap;
    vector<Tile> tiles;
    tilesMap.push_back(robotCoord);
    tiles[getIndex(tilesMap, robotCoord)] = Tile(robotCoord);
    dfs(tilesMap, tiles);
}