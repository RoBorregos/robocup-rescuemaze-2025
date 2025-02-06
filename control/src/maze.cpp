#include "maze.h"

coord inicio = {0,0,0};
coord robotCoord = {0,0,0};
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
            Serial.println("distra0");
            robot.rotate(270);
        } else if (next.x < robotCoord.x) {
            Serial.println("distra1");
            robot.rotate(90);
        } else if (next.y > robotCoord.y) {
            Serial.println("distra2");
            robot.rotate(0);
        } else if (next.y < robotCoord.y) {
            Serial.println("distra3");
            robot.rotate(180);
        }else{
            Serial.println("else");
        }
        Serial.println("ahead");
        robot.ahead();
    }
    Serial.println(9);
}
void maze::dijkstra(coord& start, coord& end, arrCustom<coord>& tilesMap, arrCustom<Tile> tiles){
    Stack path;
    arrCustom<bool> explored(kMaxSize, false);
    arrCustom<int> distance(kMaxSize, kMaxInt);
    arrCustom<coord> previousPositions(kMaxSize, kInvalidPosition);
    Serial.println(6);
    distance.set(tilesMap.getIndex(start), 0);
    explored.set(tilesMap.getIndex(start), true);
    
    coord current = start;
    while(!explored.getValue(tilesMap.getIndex(end))){
        int minDist = kMaxInt;
        for(const TileDirection& direction : directions){
            Tile& currentTile = tiles.getValue(tilesMap.getIndex(current));
            coord& adjacent = currentTile.adjacentTiles_[static_cast<int>(direction)]->position_;
            // find the distance to the adjacent tile
            if(currentTile.adjacentTiles_[static_cast<int>(direction)] != nullptr && !currentTile.hasWall(direction)){
                int weight = currentTile.weights_[static_cast<int>(direction)] +distance.getValue(tilesMap.getIndex(current));
                if(weight < distance.getValue(tilesMap.getIndex(adjacent))){
                    distance.set(tilesMap.getIndex(adjacent),weight);
                    previousPositions.set(tilesMap.getIndex(adjacent), current);
                }
            }
        }
        //find the minimum distance to the path line
        for(int i = tilesMap.getSize() -1; i >= 0; --i){
            coord& currentCoord = tilesMap.getValue(i);
            int currentDistance = distance.getValue(tilesMap.getIndex(currentCoord));
            if(currentDistance < minDist && !explored.getValue(tilesMap.getIndex(currentCoord))){
                minDist = currentDistance;
                current = currentCoord;
            }
        }
        explored.getValue(tilesMap.getIndex(current)) = true;
    }
    Serial.println(7);
    current = end;
    while(current != start){
        path.push(current);
        current = previousPositions.getValue(tilesMap.getIndex(current));
    }
    //first cell, avoid first ahead
    //if(start != end) path.push(start);
    Serial.println(8);
    followPath(path);
}
void maze::dfs(arrCustom<coord>& visitedMap, arrCustom<Tile>& tiles, arrCustom<coord>& tilesMap){
    Stack unvisited;
    arrCustom<bool> visited(kMaxSize, false);
    unvisited.push(robotCoord);
    Serial.println(1);
    coord next;
    Tile* currentTile;
    TileDirection oppositeDirection;
    bool visitedFlag = false;
    bool wall = false;

    while(!unvisited.empty()){
        Serial.println(2);
        //constants for the directions
        coord current = unvisited.top();
        unvisited.pop();

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
        Serial.println(3);
        dijkstra(robotCoord, current, tilesMap, tiles);
        visitedMap.push_back(current);
        visited.push_back(true);
        //ahead(current);
        robotCoord = current;
        for(const TileDirection direction: directions){
            Serial.println(4);
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
                // if the tile is not in the map
                tilesMap.push_back(next);
                tiles.getValue(tilesMap.getIndex(next)) = Tile(next);
                Tile* nextTile = &tiles.getValue(tilesMap.getIndex(next));
                
                if(nextTile->position_ == kInvalidPosition){
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
                        Serial.println(5);
                        unvisited.push(next);
                    }
                }
            }
        }
        Serial.println(10);
    }
    Serial.println(11);
    dijkstra(robotCoord, inicio, visitedMap, tiles);
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