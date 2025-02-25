#include "maze.h"
#include "Arduino.h"
coord inicio = {128,128,0};
coord robotCoord = {128,128,0};
TileDirection directions[4] = {TileDirection::kLeft,TileDirection::kDown,TileDirection::kRight,TileDirection::kUp};
int robotOrientation = 0;
maze::maze(){

}
//comienza logica ---------------------------------------------------------
void maze::followPath(Stack& path){
    while(!path.empty()){
        const coord& next = path.top();
        Serial.println(next.x);
        Serial.println(next.y);
        path.pop();
        if (next.x > robotCoord.x) {
            robot.rotate(90);
        } else if (next.x < robotCoord.x) {
            robot.rotate(270);
        } else if (next.y > robotCoord.y) {
            robot.rotate(0);
        } else if (next.y < robotCoord.y) {
            robot.rotate(180);
        }
        robot.ahead_ultra();
    }
}

struct Node {
    coord position;
    int distance;

    bool operator<(const Node& other) const {
        return distance < other.distance; // For Min-Heap
    }

    bool operator>(const Node& other) const {
        return distance > other.distance;
    }
};

void maze::dijkstra(coord& start, coord& end, arrCustom<coord>& tilesMap, arrCustom<Tile>& tiles) {
    Stack path;
    // Initialization
    arrCustom<bool> explored(tilesMap.getSize(), false);
    arrCustom<int> distance(tilesMap.getSize(), INT_MAX);
    arrCustom<coord> previousPositions(tilesMap.getSize(), kInvalidPosition);

    // Set starting node
    distance.set(tilesMap.getIndex(start), 0);
    explored.set(tilesMap.getIndex(start), true);

    heap<Node, 300> pq; 
    pq.insertNode({start, 0});

    while (!pq.isEmpty()) {
        Node currentNode = pq.extractMin();
        coord current = currentNode.position;

        if (current == end) {
            break;
        }
        int currentIndex = tilesMap.getIndex(current);
        if (currentIndex == -1) {
            return;
        }    
        const Tile* currentTile = &tiles.getValue(currentIndex);
        for (const TileDirection direction : directions) {
            if (currentTile->adjacentTiles_[static_cast<int>(direction)] != nullptr &&
                !currentTile->hasWall(direction)) {

                const coord& adjacent = currentTile->adjacentTiles_[static_cast<int>(direction)]->position_;
                int weight = currentTile->weights_[static_cast<int>(direction)];
                int newDist = distance.getValue(currentIndex) + weight;

                int adjIndex = tilesMap.getIndex(adjacent);
                if (explored.getValue(adjIndex)) {
                    continue;
                    if (adjIndex == -1) {
                        continue;
                    }

                    if (!explored.getValue(adjIndex) && newDist < distance.getValue(adjIndex)) {
                        distance.set(adjIndex, newDist);
                        previousPositions.set(adjIndex, current);
                        pq.insertNode({adjacent, newDist});
                    }
                }
            }
            explored.set(currentIndex, true);
        }
    }
    coord current = end;
    while(current != start){
        path.push(current);
        current = previousPositions.getValue(tilesMap.getIndex(current));
    }
    followPath(path);
}
void maze::dfs(arrCustom<coord>& visitedMap, arrCustom<Tile>& tiles, arrCustom<coord>& tilesMap){
    Stack unvisited;
    arrCustom<bool> visited(kMaxSize, false);
    unvisited.push(robotCoord);
    coord next;
    Tile* currentTile;
    TileDirection oppositeDirection;
    bool visitedFlag = false;
    bool wall = false;

    while(!unvisited.empty()){
        //constants for the directions
        coord current = unvisited.top();
        unvisited.pop();
        robot.wifiPrint("coord x",current.x);
        robot.wifiPrint("coord y",current.y);
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
            if(robot.isWall(static_cast<int>(direction))){//robot.isWall(static_cast<int>(direction))
                wall = true;
            }
            switch(direction) {
                case TileDirection::kRight:
                    next = coord{current.x + 1, current.y, 0};
                    currentTile = &tiles.getValue(tilesMap.getIndex(current));
                    oppositeDirection = TileDirection::kLeft;
                    break;
                case TileDirection::kUp:
                    next = coord{current.x, current.y + 1, 0};
                    currentTile = &tiles.getValue(tilesMap.getIndex(current));
                    oppositeDirection = TileDirection::kDown;
                    break;
                case TileDirection::kLeft:
                    next = coord{current.x - 1, current.y, 0};
                    currentTile = &tiles.getValue(tilesMap.getIndex(current));
                    oppositeDirection = TileDirection::kRight;
                    break;
                case TileDirection::kDown:
                    next = coord{current.x, current.y - 1, 0};
                    currentTile = &tiles.getValue(tilesMap.getIndex(current));
                    oppositeDirection = TileDirection::kUp;
                    break;
            }
            //check for adjacentTiles and connecting them
            if(currentTile -> adjacentTiles_[static_cast<int>(direction)] == nullptr){
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
                // join the tiles and if there is no wall between them
                currentTile -> addAdjacentTile(direction, nextTile, wall);
                nextTile -> addAdjacentTile(oppositeDirection, currentTile, wall);
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
            }
        }
    }
    dijkstra(robotCoord, inicio, tilesMap, tiles);
}
void maze::run_algs(){
    arrCustom<coord> visitedMap(kMaxSize, kInvalidPosition);
    arrCustom<coord> tilesMap(kMaxSize, kInvalidPosition);
    arrCustom<Tile> tiles(kMaxSize, Tile(kInvalidPosition));
    tilesMap.push_back(robotCoord);
    tiles.getValue(tilesMap.getIndex(robotCoord)) = Tile(robotCoord);
    dfs(visitedMap, tiles, tilesMap);
}
/*
 *  Executing task: C:\Users\UsX\.platformio\penv\Scripts\platformio.exe device monitor 

--- Terminal on COM3 | 115200 8-N-1
--- Available filters and text transformations: colorize, debug, default, direct, esp32_exception_decoder, hexlify, log2file, nocontrol, printable, send_on_enter, time
--- More details at https://bit.ly/pio-monitor-filters
--- Quit: Ctrl+C | Menu: Ctrl+T | Help: Ctrl+T followed by Ctrl+H
0x1 (POWERON_RESET),boot:0x13 (SPI_FAST_FLASH_BOOT)
configsip: 0, SPIWP:0xee
clk_drv:0x00,q_drv:0x00,d_drv:0x00,cs0_drv:0x00,hd_drv:0x00,wp_drv:0x00
mode:DIO, clock div:1
load:0x3fff0030,len:1344
load:0x40078000,len:13964
load:0x40080400,len:3600
entry 0x400805f0
Running...
1
2
3
6
7
8
9
4
5
4
5
4
5
4
5
10
2
3
6
7
8
0
1
distra2
ahead
9
4
5
4
4
5
4
5
10
2
3
6
7
8
0
2
distra2
ahead
9
4
5
4
4
5
4
5
10
2
3
6
7
8
0
3
distra2
ahead
9
4
5
4
4
5
4
5
10
2
3
6
7
8
0
4
distra2
ahead
9
4
5
4
4
5
4
5
10
2
3
6
7
8
0
5
distra2
ahead
9
4
5
4
4
5
4
5
10
2
3
6
7
8
0
6
distra2
ahead
9
4
5
4
4
5
4
5
10
2
3
6
7
8
0
7
distra2
ahead
9
4
5
4
4
5
4
5
10
2
3
6
7
8
0
8
distra2
ahead
9
4
5
4
4
5
4
5
10
2
3
6
7
8
0
9
distra2
ahead
9
4
5
4
4
5
4
5
10
2
3
6
7
8
0
10
distra2
ahead
9
4
5
4
4
5
4
5
10
2
3
6
7
8
0
11
distra2
ahead
9
4
5
4
4
5
4
5
10
2
3
6
7
8
0
12
distra2
ahead
9
4
5
4
4
5
4
5
10
2
3
6
7
8
0
13
distra2
ahead
9
4
5
4
4
5
4
5
10
2
3
6
7
8
0
14
distra2
ahead
9*/
