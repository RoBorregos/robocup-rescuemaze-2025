#include <iostream>
#include "coord.h"
#include "Tile.cpp"
#include "Map.cpp"
using namespace std;
vector<vector<char> > RealMaze = {
        /*
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
        */
        {'#', '#', '#', '#', '#', '#', '#'},
        {'#', ' ', '#', ' ', ' ', ' ', '#'},
        {'#', ' ', '.', ' ', '.', ' ', '#'},
        {'#', ' ', '#', ' ', ' ', ' ', '#'},
        {'#', ' ', '.', '#', '.', ' ', '#'},
        {'#', ' ', ' ', ' ', ' ', ' ', '#'},
        {'#', '#', '#', '#', '#', '#', '#'}
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
long iterations = 0;
coord robotCoord = {1,1,1};
constexpr TileDirection directions[4] = {TileDirection::kRight, TileDirection::kUp, TileDirection::kLeft, TileDirection::kDown};
char robotChar = 'v';
int robotOrientation = 0;
const int kMaxSize = 256;

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
    if (difference == 90 || difference == -270) {
        turnCharR(robotChar);
        robotOrientation = (robotOrientation + 90) % 360;
    } else if (difference == 270 || difference == -90) {
        turnCharL(robotChar);
        robotOrientation = (robotOrientation + 270) % 360;
    } else if (difference == 180|| difference == -180) {
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
        ahead(next);
        robotCoord = next;
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


void dijkstra(coord& start, coord& end,  Map& tilesMap, vector<Tile> tiles){
    vector<bool> explored(tilesMap.positions.size(), false);
    vector<int> distance(tilesMap.positions.size(), INT_MAX);
    vector<coord> previousPositions;
    stack<coord> path;

    distance[tilesMap.getIndex(start)] = 0;
    explored[tilesMap.getIndex(start)] = true;
    coord currentCoord = start;
    int minDistance;
    while(!explored[tilesMap.getIndex(end)]){
        for(const TileDirection direction : directions){
            const Tile& currentTile = tiles.at(tilesMap.getIndex(currentCoord));
            const coord& adjacentCoord = currentTile.adjacentTiles_[static_cast<int>(direction)]->position_;
            
            if(currentTile.adjacentTiles_[static_cast<int>(direction)] != nullptr && !currentTile.hasWall(direction)){
                const int weight = currentTile.weights_[static_cast<int>(direction)] + distance.at(tilesMap.getIndex(currentCoord));
                if(weight < distance.at(tilesMap.getIndex(adjacentCoord))){
                    distance[tilesMap.getIndex(adjacentCoord)] = weight;
                    previousPositions[tilesMap.getIndex(adjacentCoord)] = currentCoord;
                }
            }
        }
        minDistance = INT_MAX;
        for(int i = tilesMap.positions.size() - 1; i >= 0; --i){
            const coord& current = tilesMap.positions.at(i);
            const int currentDistance = distance[tilesMap.getIndex(current)];
            if (currentDistance < minDistance && !explored[tilesMap.getIndex(current)]) {
                minDistance = currentDistance;
                currentCoord = current;
            }
        }
        explored[tilesMap.getIndex(currentCoord)] = true;
    }
    coord current = end; 
    while (current != start){
        path.push(current);
        current = previousPositions[tilesMap.getIndex(current)];
    }
    path.push(start);
    followPath(path);
}

void dfs(){
    cout << "Initial Maze" << endl;
    //vector<coord> positions;
    //vector<Tile> tiles;
    //positions.push_back(robotCoord);
    //tiles.push_back(Tile(robotCoord));
    Map tilesMap = Map();
    vector<Tile> tiles;
    tilesMap.positions.push_back(robotCoord);
    int idx = tilesMap.getIndex(robotCoord);
    if (idx == -1) {
        tiles.push_back(Tile(robotCoord));
        idx = tiles.size() - 1;
    }
    tiles[idx] = Tile(robotCoord);


    Map visitedMap = Map();
    vector<bool> visited;
    
    stack<coord> unvisited;
    Tile* currentTile;
    coord nextCoord;
    TileDirection oppositeDirection;
    unvisited.push(robotCoord);


    bool wall;
    bool alreadyConnected;
    bool visitedFlag;

    cout << "Initial DFS" << endl;

    while(!unvisited.empty()){
        coord currentCoord = unvisited.top();
        unvisited.pop();
        
        cout << "Visiting (" << currentCoord.x << ", " << currentCoord.y << ")\n";

        visitedFlag = false;
        // Check if the current coordinate has been visited
        for(int i = 0; i < visitedMap.positions.size(); ++i){
            if(visitedMap.positions[i] == currentCoord){
                visitedFlag = true;
                break;
            }
        }
        if (visitedFlag) {
            continue;
        }

        dijkstra(robotCoord, currentCoord, visitedMap, tiles);
        robotCoord = currentCoord;
        visitedMap.positions.push_back(currentCoord);
        visited.push_back(true);
        for(const TileDirection direction: directions){
            wall = false;
            switch(direction) {
                case TileDirection::kRight:
                    nextCoord = coord{currentCoord.x + 2, currentCoord.y, 1}; // checkRamp(direction);
                    currentTile = &tiles.at(tilesMap.getIndex(currentCoord));
                    oppositeDirection = TileDirection::kLeft;
                    break;
                case TileDirection::kUp:
                    nextCoord = coord{currentCoord.x, currentCoord.y + 2, 1}; // checkRamp(direction);
                    currentTile = &tiles.at(tilesMap.getIndex(currentCoord));
                    oppositeDirection = TileDirection::kDown;
                    break;
                case TileDirection::kLeft:
                    nextCoord = coord{currentCoord.x - 2, currentCoord.y, 1}; // checkRamp(direction);
                    currentTile = &tiles.at(tilesMap.getIndex(currentCoord));
                    oppositeDirection = TileDirection::kRight;
                    break;
                case TileDirection::kDown:
                    nextCoord = coord{currentCoord.x, currentCoord.y - 2, 1}; // checkRamp(direction);
                    currentTile = &tiles.at(tilesMap.getIndex(currentCoord));
                    oppositeDirection = TileDirection::kUp;
                    break;
            }
            // Check if the next coordinate has a wall
            if(currentTile -> adjacentTiles_[static_cast<int>(direction)] == nullptr){
                wall = false;
                tilesMap.positions.push_back(nextCoord);
                tiles[tilesMap.getIndex(nextCoord)] = Tile(nextCoord);
                Tile* nextTile = &tiles[tilesMap.getIndex(nextCoord)];
                if(nextTile->position_ == kInvalidPosition){
                    nextTile->setPosition(nextCoord);
                }
                currentTile -> addAdjacentTile(direction, nextTile, wall);
                nextTile -> addAdjacentTile(oppositeDirection, currentTile, wall);
                if(!wall){
                    visitedFlag = false;
                    for(int i = 0; i < visitedMap.positions.size(); ++i){
                        if(visitedMap.positions.at(i) == nextCoord){
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
        printMaze(discoverMaze);
    }
}

//save tilesMap and tiles, end should be the last robotCoord saved, save all this variables when a new checkpoint has been found
void rememberPath(coord end, Map& visitedTiles, vector<Tile>& tiles, int amountWalls){
    for(int i =0; i < tiles.size(); i++){
        //coord curr = tilesMap.positions[i];
        //Tile check = tiles[tilesMap.getIndex(curr)];
        Tile check = tiles[i];
        if(check.hasCheckpoint()){
            int walls = 0;
            for(int i =0; i< 4; i++){
                if(check.hasWall(static_cast<TileDirection>(i))){
                    walls++;
                }
            }
            if(walls == amountWalls){
                coord checkPoint = check.position_;
                dijkstra(checkPoint, end, visitedTiles, tiles);
                robotCoord = checkPoint;
                break;
            }
            
        }
    }
    //tiles[tilesMap.getIndex(nextCoord)] = Tile(nextCoord);
}

int main(){

    cout << "Initial Maze" << endl;
    bool button = false;
    if(!button){
        dfs();
    }else{
        //get the last position of the robot, tilesMap and tiles
        coord lastCoord = robotCoord;
        Map lastVisitedTiles = Map();
        vector<Tile> lastTiles;
        rememberPath(lastCoord, lastVisitedTiles, lastTiles, 3);
        dfs();
    }
    dfs();
    cout << "Final Maze" << endl;
    printMaze(discoverMaze);
}