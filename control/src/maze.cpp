#include "maze.h"

coord inicio = {0,0,0};
coord robotCoord = {0,0,0};
TileDirection directions[4] = {TileDirection::kLeft,TileDirection::kDown,TileDirection::kRight,TileDirection::kUp};
int robotOrientation = 0;

//comentar en las pruebas
/*
void maze::right(){};
void maze::left(){};
void maze::ahead(){};
bool maze::vlx(int num){return true;};

void maze::rotate(const int targetOrientation) {
    int difference = targetOrientation - robotOrientation;
    if (difference == 0) {
        return;
    }
    if (difference == 90 || difference == -270) {
        robot.right();
        robotOrientation = (robotOrientation + 90) % 360;
    } else if (difference == 270 || difference == -90) {
        robot.left();
        robotOrientation = (robotOrientation + 270) % 360;
    } else if (difference == 180|| difference == -180) {
        robot.right();
        robot.right();
        robotOrientation = (robotOrientation + 180) % 360;
    }
}
bool maze::isWall(const TileDirection& direction) {
    switch(direction) {
        case TileDirection::kUp:
            return vlx(static_cast<int>(TileDirection::kUp));
        case TileDirection::kRight:
            return vlx(static_cast<int>(TileDirection::kRight));
        case TileDirection::kDown:
            return vlx(static_cast<int>(TileDirection::kDown));
        case TileDirection::kLeft:
            return vlx(static_cast<int>(TileDirection::kLeft));
        default: 
            break;
    }
}
*/
//comienza logica ---------------------------------------------------------
void maze::followPath(Stack& path){
    while(!path.empty()){
        const coord& next = path.top();
        path.pop();
        if (next.x > robotCoord.x) {
            robot.rotate(270);
        } else if (next.x < robotCoord.x) {
            robot.rotate(90);
        } else if (next.y > robotCoord.y) {
            robot.rotate(0);
        } else if (next.y < robotCoord.y) {
            robot.rotate(180);
        }
        robot.ahead_ultra();
    }
}
void maze::dijkstra(coord& start, coord& end, arrCustom<coord>& tilesMap, arrCustom<Tile> tiles){
    Stack path;
    arrCustom<bool> explored(kMaxSize, false);
    arrCustom<int> distance(kMaxSize, kMaxInt);
    arrCustom<coord> previousPositions(kMaxSize, kInvalidPosition);

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
    current = end;
    while(current != start){
        path.push(current);
        current = previousPositions.getValue(tilesMap.getIndex(current));
    }
    //first cell, avoid first ahead
    if(start != end) path.push(start);
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
            if(robot.isWall(static_cast<int>(direction))){
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
                        unvisited.push(next);
                    }
                }
            }
        }
    }
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
