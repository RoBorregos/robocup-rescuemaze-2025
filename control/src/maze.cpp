#include "maze.h"
#include "Arduino.h"
coord inicio = {128, 128, 0};
coord robotCoord = {128, 128, 0};
TileDirection directions[4] = {TileDirection::kLeft, TileDirection::kDown, TileDirection::kRight, TileDirection::kUp};
coord checkpointCoord = {128, 128, 0};
arrCustom<coord> visitedMapRecover(kMaxSize, kInvalidPosition);
uint8_t robotOrientation = 0;
uint8_t level = 0;
maze::maze(){}
uint8_t maze::getLevel(){return level;}
void maze::setLevel(uint8_t level_){level = level_;}
// logic ---------------------------------------------------------
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
        robot.ahead();
        robotCoord = next;
        if(robot.buttonPressed == true){
            break;
        }
    }
}
/*
struct Node {
    coord position;
    uint8_t distance;

    bool operator<(const Node& other) const {
        return distance < other.distance; // For Min-Heap
    }

    bool operator>(const Node& other) const {
        return distance > other.distance;
    }
};

void maze::dijkstra(coord& start, coord& end, arrCustom<coord>& tilesMap, arrCustom<Tile>& tiles) {
    Stack path;
    arrCustom<bool> explored(tilesMap.getSize(), false);
    arrCustom<int> distance(tilesMap.getSize(), INT_MAX);
    arrCustom<coord> previousPositions(tilesMap.getSize(), kInvalidPosition);

    distance.set(tilesMap.getIndex(start), 0);
    explored.set(tilesMap.getIndex(start), true);

    heap<Node, kMaxStackSize> pq; 
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
                        Node newNode = {adjacent, newDist};
                        pq.insertNode(newNode);
                        //pq.insertNode({adjacent, newDist});
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
*/
void maze::dijkstra(coord& start, coord& end, arrCustom<coord>& tilesMap, arrCustom<Tile>& tiles){
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
            if(currentTile.adjacentTiles_[static_cast<int>(direction)] != nullptr && !currentTile.hasWall(direction) ){//&& currentTile.weights_[static_cast<int>(direction)] != NULL){
                const int weight = currentTile.weights_[static_cast<int>(direction)] +distance.getValue(tilesMap.getIndex(current));
                int index = distance.getValue(tilesMap.getIndex(adjacent));
                if(weight < distance.getValue(tilesMap.getIndex(adjacent))){
                    distance.set(tilesMap.getIndex(adjacent),weight);
                    previousPositions.set(tilesMap.getIndex(adjacent), current);
                    printf("llegue4");
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
        //visited.push_back(true);

        if(robot.blackTile == true){
            currentTile = &tiles.getValue(tilesMap.getIndex(current));
            currentTile -> setBlackTile();
            robot.blackTile = false;
        }
        if(robot.blueTile == true){
            currentTile = &tiles.getValue(tilesMap.getIndex(current));
            currentTile -> setObstacle();
            robot.blueTile = false;
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
        //button checkpoint logic
        if(robot.buttonPressed){
            robot.buttonPressed = false;
            for(uint8_t i = 0; i < kMaxSize; i++){
                visitedMap.set(i, visitedMapRecover.getValue(i));
            }
            while(robot.buttonPressed == false){
                //get button state, not continuing until it is true again
            }
            float orientation = robot.getAngleOrientation();
            current = checkpointCoord;
            if(orientation == 0){
                orientation = 0;
            }else if(orientation == 90){
                orientation = 1;
            }else if(orientation == 180){
                orientation = 2;
            }else if(orientation == 270){
                orientation = 3;
            }
            //clear unvisited stack 
            unvisited.~Stack();
            //set the new priority to the front of the robot
            TileDirection temp = directions[3];
            for(int i = 3; i > 0; i--){
                directions[i] = directions[i-1];
            }
            directions[0] = temp;
        }
        //ramp logic -> 1 pa arriba - 2 para abajo - 0 normal
        if(robot.rampState != 0){
            if(robot.rampState == 1){level++;}
            if(robot.rampState == 2){level--;}
            robot.rampState = 0;
            coord tempCurr = current;
            tempCurr.z = level;
            //change level in the link of tiles
            currentTile = &tiles.getValue(tilesMap.getIndex(robotCoord));
            for(int i = 0; i < 4; i++){
                if(currentTile -> adjacentTiles_[static_cast<int>(i)] != nullptr)
                    if(currentTile -> adjacentTiles_[i]->position_ == current){
                        current = {current.x, current.y, level};
                        currentTile -> adjacentTiles_[i]->setPosition(current);
                    }
                }
            }
            currentTile = &tiles.getValue(tilesMap.getIndex(current));
            for(int i = 0; i < 4; i++){
                if(currentTile -> adjacentTiles_[static_cast<int>(i)] != nullptr)
                    if(currentTile -> adjacentTiles_[i]->position_ == robotCoord){
                        current = {robotCoord.x, robotCoord.y, level};
                        currentTile -> adjacentTiles_[i]->setPosition(robotCoord);
                    }
                }
            }
            //remember the coord
            current = tempCurr;
        }
        robotCoord = current;
        
        for(const TileDirection direction: directions){
            wall = false; 
            if(robot.isWall(static_cast<int>(direction))){
                wall = true;
            }
            switch(direction) {
                case TileDirection::kRight:
                    next = coord{static_cast<uint8_t>(current.x + 1), current.y, getLevel()};
                    currentTile = &tiles.getValue(tilesMap.getIndex(current));
                    oppositeDirection = TileDirection::kLeft;
                    break;
                case TileDirection::kUp:
                    next = coord{current.x, static_cast<uint8_t>(current.y + 1), getLevel()};
                    currentTile = &tiles.getValue(tilesMap.getIndex(current));
                    oppositeDirection = TileDirection::kDown;
                    break;
                case TileDirection::kLeft:
                    next = coord{static_cast<uint8_t>(current.x - 1), current.y, getLevel()};
                    currentTile = &tiles.getValue(tilesMap.getIndex(current));
                    oppositeDirection = TileDirection::kRight;
                    break;
                case TileDirection::kDown:
                    next = coord{current.x, static_cast<uint8_t>(current.y - 1), getLevel()};
                    currentTile = &tiles.getValue(tilesMap.getIndex(current));
                    oppositeDirection = TileDirection::kUp;
                    break;
            }
            //check for adjacentTiles and connecting them
            if(currentTile -> adjacentTiles_[static_cast<int>(direction)] == nullptr){
                int index = tilesMap.getIndex(next);
                Tile* nextTile;
                if (index == 255) {  // Tile doesn't exist, create and add it
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