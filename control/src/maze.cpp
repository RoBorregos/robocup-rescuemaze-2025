#include "maze.h"
#include "Arduino.h"
#include "Jetson.h"
Jetson jetson;
coord inicio = {kBaseCoord, kBaseCoord, kBaseCoord};
coord robotCoord = {kBaseCoord, kBaseCoord, kBaseCoord};
TileDirection directions[4] = {TileDirection::kLeft, TileDirection::kDown, TileDirection::kRight, TileDirection::kUp};
coord checkpointCoord = {kBaseCoord, kBaseCoord, kBaseCoord};
uint8_t robotOrientation = 0;
uint8_t level = kBaseCoord;
maze::maze(){}
// logic ---------------------------------------------------------
void changeLevel() { level += (robot.rampState == 1) - (robot.rampState == 2); robot.rampState = 0; }
void maze::followPath(Stack& path){
    while(!path.empty()){
        const coord& next = path.top();
        Serial.println(next.x);
        Serial.println(next.y);
        path.pop();
        if (next.x > robotCoord.x) {
            robot.rotate(90);
            if(robot.buttonPressed == true) break;
            // jetson.getDetection();
            if(robot.buttonPressed == true) break;
        } else if (next.x < robotCoord.x) {
            robot.rotate(270);
            if(robot.buttonPressed == true) break;
            // jetson.getDetection();
            if(robot.buttonPressed == true) break;
        } else if (next.y > robotCoord.y) {
            robot.rotate(0);
            if(robot.buttonPressed == true) break;
        } else if (next.y < robotCoord.y) {
            // jetson.getDetection();
            robot.rotate(180);
            if(robot.buttonPressed == true) break;
        }
        robot.ahead();
        if(robot.buttonPressed == true) break;
        if(robot.blackTile) continue;
        robotCoord = next;
    }
}
void maze::dijkstra(coord& start, coord& end, arrCustom<coord>& tilesMap, arrCustom<Tile>& tiles){
    Stack path;
    arrCustom<bool> explored(tilesMap.getSize(), false);
    arrCustom<int> distance(tilesMap.getSize(), INT_MAX);
    arrCustom<coord> previousPositions(tilesMap.getSize(), kInvalidPosition);

    distance.set(tilesMap.getIndex(start), 0);
    explored.set(tilesMap.getIndex(start), true);
    uint8_t minDist;
    coord current = start;
    while(!explored.getValue(tilesMap.getIndex(end))){ 
        for(const TileDirection& direction : directions){
            const Tile& currentTile = tiles.getValue(tilesMap.getIndex(current));
            const coord& adjacent = currentTile.adjacentTiles_[static_cast<int>(direction)]->position_;
            if(currentTile.adjacentTiles_[static_cast<int>(direction)] != nullptr && !currentTile.hasWall(direction) && !currentTile.hasBlackTile()){
                const int weight = currentTile.weights_[static_cast<int>(direction)] +distance.getValue(tilesMap.getIndex(current));
                int index = distance.getValue(tilesMap.getIndex(adjacent));
                if(weight < distance.getValue(tilesMap.getIndex(adjacent))){
                    distance.set(tilesMap.getIndex(adjacent),weight);
                    previousPositions.set(tilesMap.getIndex(adjacent), current);
                }
            }
        }
        minDist = kMaxInt;
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

void maze::dfs(arrCustom<coord>& visitedMap, arrCustom<Tile>& tiles, arrCustom<coord>& tilesMap){
    Stack unvisited;
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
        robot.wifiPrint("coord z",current.z);
        visitedFlag = false;
        for(int i = 0; i < visitedMap.getSize(); ++i){
            if(visitedMap.getValue(i) == current){
                visitedFlag = true;
                break;
            }
        }
        if (visitedFlag) continue;
        dijkstra(robotCoord, current, tilesMap, tiles);
        visitedMap.push_back(current);

        if(robot.blackTile){
            currentTile = &tiles.getValue(tilesMap.getIndex(current));
            currentTile -> setBlackTile();
            robot.blackTile = false;
            continue;
        }
        if(robot.checkpoint == true){
            currentTile = &tiles.getValue(tilesMap.getIndex(current));
            currentTile -> setCheckpoint();
            checkpointCoord = current;
            robot.checkpoint = false;

        }
        if(robot.victim == true){
            currentTile = &tiles.getValue(tilesMap.getIndex(current));
            currentTile -> setVictim();
            robot.victim = false;
        }
        changeLevel();
        robotCoord = current;
        
        //button checkpoint logic
        if(robot.buttonPressed){
            float orientation = robot.getAngleOrientation();
            if(orientation == 0){
                orientation = 0;
            }else if(orientation == 90){
                orientation = 1;
            }else if(orientation == 180){
                orientation = 2;
            }else if(orientation == 270){
                orientation = 3;
            }

            if(orientation == 0){
                robot.screenPrint("LoP");      
                // robotCoord = current;
                while(robot.buttonPressed) robot.screenPrint("Esperando");
                robot.screenPrint("Dale");
                robotCoord = inicio;
                robot.screenPrint("Inicio");
                dijkstra(robotCoord, current, tilesMap, tiles);
                robotCoord = inicio;
            }else{
                robot.screenPrint("LoP");      
                robotCoord = inicio;
                robot.screenPrint("Inicio");
                while(robot.buttonPressed) robot.screenPrint("Esperando");
                robot.screenPrint("Dale");
                Serial.println("resetting visitedMap");
                visitedMap.reset();
                Serial.println("resetting tilesMap");
                tilesMap.reset();
                Serial.println("resetting tiles");
                tiles.reset();
                current = robotCoord;
                unvisited.~Stack();
                unvisited.push(robotCoord);
                tilesMap.push_back(robotCoord);
                tiles.push_back(Tile(robotCoord));
                Serial.println("good");
                for(int i = 0; i < orientation; i++){
                    TileDirection temp = directions[3];
                    for(int i = 3; i > 0; i--){
                        directions[i] = directions[i-1];
                    }
                    directions[0] = temp;
                }
            }
            //option4 (setting new priority, recovering the map and applying angle setup)
        }

        for(const TileDirection direction: directions){
            wall = false; 
            if(robot.isWall(static_cast<int>(direction))) wall = true;
            switch(direction) {
                case TileDirection::kRight:
                    next = coord{static_cast<uint8_t>(current.x + 1), current.y, level};
                    currentTile = &tiles.getValue(tilesMap.getIndex(current));
                    oppositeDirection = TileDirection::kLeft;
                    break;
                case TileDirection::kUp:
                    next = coord{current.x, static_cast<uint8_t>(current.y + 1), level};
                    currentTile = &tiles.getValue(tilesMap.getIndex(current));
                    oppositeDirection = TileDirection::kDown;
                    break;
                case TileDirection::kLeft:
                    next = coord{static_cast<uint8_t>(current.x - 1), current.y, level};
                    currentTile = &tiles.getValue(tilesMap.getIndex(current));
                    oppositeDirection = TileDirection::kRight;
                    break;
                case TileDirection::kDown:
                    next = coord{current.x, static_cast<uint8_t>(current.y - 1), level};
                    currentTile = &tiles.getValue(tilesMap.getIndex(current));
                    oppositeDirection = TileDirection::kUp;
                    break;
            }
            //check for adjacentTiles and connecting them
            if(currentTile -> adjacentTiles_[static_cast<int>(direction)] == nullptr){
                int index = tilesMap.getIndex(next);
                Tile* nextTile;
                if (index == kMaxInt) { 
                    tilesMap.push_back(next);
                    tiles.set(tilesMap.getIndex(next), Tile(next));
                    nextTile = &tiles.getValue(tilesMap.getIndex(next));
                } else {  
                    nextTile = &tiles.getValue(index);
                }
            
                if (nextTile->position_ == kInvalidPosition) nextTile->setPosition(next);
                // join the tiles and if there is no wall between them
                currentTile -> addAdjacentTile(direction, nextTile, wall,false);
                if(robot.blueTile){
                    nextTile->addAdjacentTile(oppositeDirection, nextTile, wall, true);
                    robot.blueTile = false;
                }else{
                    nextTile->addAdjacentTile(oppositeDirection, currentTile, wall, false);
                }
                if(!wall){
                    visitedFlag = false;
                    for(uint8_t i = 0; i < visitedMap.getSize(); ++i){
                        if(visitedMap.getValue(i) == next){
                            visitedFlag = true;
                            break;
                        }
                    }
                    if(!visitedFlag) unvisited.push(next);
                }
            }
        }
    }
    if(robot.buttonPressed==false) dijkstra(robotCoord, inicio, tilesMap, tiles); 
}
void maze::run_algs(){
    arrCustom<coord> visitedMap(kMaxSize, kInvalidPosition);
    arrCustom<coord> tilesMap(kMaxSize, kInvalidPosition);
    arrCustom<Tile> tiles(kMaxSize, Tile(kInvalidPosition));
    tilesMap.push_back(robotCoord);
    tiles.getValue(tilesMap.getIndex(robotCoord)) = Tile(robotCoord);
    dfs(visitedMap, tiles, tilesMap);
}
