#include "maze.h"
#include "Arduino.h"
#include "Jetson.h"
Jetson jetson;
coord inicio = {kBaseCoord, kBaseCoord, kBaseCoord};
coord robotCoord = {kBaseCoord, kBaseCoord, kBaseCoord};
TileDirection directions[4] = {TileDirection::kLeft, TileDirection::kDown, TileDirection::kRight, TileDirection::kUp};
coord checkpointCoord = {kBaseCoord, kBaseCoord, kBaseCoord};
arrCustom<coord> visitedMapRecover(kMaxSize, kInvalidPosition);
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
            jetson.getDetection();
        } else if (next.x < robotCoord.x) {
            robot.rotate(270);
            jetson.getDetection();
        } else if (next.y > robotCoord.y) {
            robot.rotate(0);
        } else if (next.y < robotCoord.y) {
            robot.rotate(180);
        }
        robot.ahead();
        jetson.getDetection();
        if(robot.blackTile){
            continue;
        }
        robotCoord = next;
        
        if(robot.buttonPressed == true){
            break;
        }
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
    //arrCustom<bool> visited(kMaxSize, false);
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
        if (visitedFlag) {
            continue;
        }
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
            // set the recovery map in the checkpoint position
            for(uint8_t i = 0; i < kMaxSize; i++){
                visitedMapRecover.set(i, visitedMap.getValue(i));
            }
        }
        if(robot.victim == true){
            currentTile = &tiles.getValue(tilesMap.getIndex(current));
            currentTile -> setVictim();
            robot.victim = false;
        }
        changeLevel();
        // jetson.getDetection();
        robotCoord = current;
        
        //button checkpoint logic
        if(robot.buttonPressed){
            //break;
            robot.screenPrint("LoP");
            //delay(2000);
            //option 1 (starting on 0 with past priority)
            
            robotCoord = inicio;
            robot.screenPrint("Inicio");
            while(robot.buttonPressed == true){
                robot.screenPrint("Esperando");
            }
            robot.screenPrint("Dale");
            // dijkstra(robotCoord, current, tilesMap, tiles);
            // robotCoord = current;
            
            // arrCustom<coord> visitedMapR(kMaxSize, kInvalidPosition);
            // arrCustom<coord> tilesMapR(kMaxSize, kInvalidPosition);
            // arrCustom<Tile> tilesR(kMaxSize, Tile(kInvalidPosition));
            // visitedMap = visitedMapR;
            // tilesMap = tilesMapR;
            // tiles = tilesR;
            
            //tilesMap.push_back(robotCoord);
            //tiles.getValue(tilesMap.getIndex(robotCoord)) = Tile(robotCoord);
            
            //for(uint8_t j = 0; j < kMaxSize; j++){
                // visitedMap.getValue(j) = kInvalidPosition;
                //visitedMap.set(j, kInvalidPosition);
                Serial.println("resetting visitedMap");
                visitedMap.reset();
                // tilesMap.getValue(j) = kInvalidPosition;
                //tilesMap.set(j, kInvalidPosition);
                Serial.println("resetting tilesMap");
                tilesMap.reset();
                // tiles.getValue(j) = Tile(kInvalidPosition);
                //tiles.set(j, Tile(kInvalidPosition));
                Serial.println("resetting tiles");
                tiles.reset();
            //}
            // tilesMap.push_back(robotCoord);
            current = robotCoord;
            unvisited.~Stack();
            unvisited.push(robotCoord);
            tilesMap.push_back(robotCoord);
            tiles.push_back(Tile(robotCoord));
            Serial.println("good");
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
            for(int i = 0; i < orientation; i++){
                TileDirection temp = directions[3];
                for(int i = 3; i > 0; i--){
                    directions[i] = directions[i-1];
                }
                directions[0] = temp;
            }
            //tiles.getValue(tilesMap.getIndex(robotCoord))=Tile(robotCoord);
            //continue;
            //option2 (starting on 0 with past priority and setting new priority)
            
            
            // current = inicio;
            // for(uint8_t i = 0; i < visitedMap.getSize(); i++){
            //     visitedMap.set(i, kInvalidPosition);
            //     visitedMap.reset();
            //     tilesMap.set(i, kInvalidPosition);
            //     tilesMap.reset();
            //     tiles.set(i, Tile(kInvalidPosition));
            //     tiles.reset();
            // }
            // tilesMap.push_back(robotCoord);
            // tiles.set(tilesMap.getIndex(robotCoord),Tile(robotCoord));

            // float orientation = robot.getAngleOrientation();
            // current = checkpointCoord;
            // if(orientation == 0){
            //     orientation = 0;
            // }else if(orientation == 90){
            //     orientation = 1;
            // }else if(orientation == 180){
            //     orientation = 2;
            // }else if(orientation == 270){
            //     orientation = 3;
            // }
            // for(int i = 0; i < orientation; i++){
            //     TileDirection temp = directions[3];
            //     for(int i = 3; i > 0; i--){
            //         directions[i] = directions[i-1];
            //     }
            //     directions[0] = temp;
            // }
            
            //option3 (setting new priority and recovering the map)
            
            // for(uint8_t i = 0; i < visitedMapRecover.getSize(); i++){
            //     visitedMap.set(i, visitedMapRecover.getValue(i));
            //     visitedMap.reset();
            // }
            // unvisited.~Stack();
            // unvisited.push(checkpointCoord);
            // float orientation = robot.getAngleOrientation();
            // current = checkpointCoord;
            // if(orientation == 0){
            //     orientation = 0;
            // }else if(orientation == 90){
            //     orientation = 1;
            // }else if(orientation == 180){
            //     orientation = 2;
            // }else if(orientation == 270){
            //     orientation = 3;
            // }
            // for(int i = 0; i < orientation; i++){
            //     TileDirection temp = directions[3];
            //     for(int i = 3; i > 0; i--){
            //         directions[i] = directions[i-1];
            //     }
            //     directions[0] = temp;
            // }
            
            //working on it

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
            
                if (nextTile->position_ == kInvalidPosition) {
                    nextTile->setPosition(next);
                }
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
                    if(!visitedFlag){ 
                        unvisited.push(next);
                    }
                }
            }
        }
    }
    if(robot.buttonPressed==false){
        dijkstra(robotCoord, inicio, tilesMap, tiles);
    }
    
}
void maze::run_algs(){
    arrCustom<coord> visitedMap(kMaxSize, kInvalidPosition);
    arrCustom<coord> tilesMap(kMaxSize, kInvalidPosition);
    arrCustom<Tile> tiles(kMaxSize, Tile(kInvalidPosition));
    tilesMap.push_back(robotCoord);
    tiles.getValue(tilesMap.getIndex(robotCoord)) = Tile(robotCoord);
    dfs(visitedMap, tiles, tilesMap);
}
// void maze::getDetectionJetson(){
//     jetson.getDetection()
// }
