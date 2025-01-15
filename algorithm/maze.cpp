#include <iostream> 
#include <stack>
#include <unordered_map>
#include <vector>
#include <climits>

using namespace std; 

#include "Tile.cpp"

constexpr Directions directions[] = {Directions::up, Directions::down, Directions::left, Directions::right};

vector<vector<char> > maze = {
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
        //{'#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#'}
        {'#', ' ', '#', ' ', '#', ' ', '#', ' ', '#', ' ', '#'},
        {'#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#'}, // 1.
        {'#', ' ', '#', '#', '#', ' ', '#', '#', '#', ' ', '#'},
        {'#', ' ', ' ', 'r', '#', ' ', '#', 'l', ' ', ' ', '#'}, // 3.
        {'#', ' ', '#', '#', '#', ' ', '#', '#', '#', ' ', '#'},
        {'#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#'}, // 5.
        {'#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#'}
};

vector<vector<char> > mazeSecondLevel = {
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
    //{'#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#'}
    {'#', ' ', '#', ' ', '#', ' ', '#', ' ', '#', ' ', '#'},
    {'#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#'}, // 1.
    {'#', ' ', ' ', ' ', '#', '#', '#', ' ', ' ', ' ', '#'},
    {'#', ' ', ' ', 'r', ' ', ' ', ' ', 'l', ' ', ' ', '#'}, // 3.
    {'#', ' ', ' ', ' ', '#', '#', '#', ' ', ' ', ' ', '#'},
    {'#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#'}, // 5.
    {'#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#'}
};


void printMaze(const vector<vector<char> >& maze){
    for(int i = 0; i < maze.size(); ++i){
        for(int j = 0; j < maze[i].size(); j++){
            cout << maze[i][j] << " ";
        }
        cout << endl;
    }
}
void printPath(stack<coord> path){
    vector<vector<char> > newMaze = maze;
    int lastX = path.top().x;
    int lastY = path.top().y;
    path.pop();
    newMaze[lastY][lastX] = 'S';
    while(!path.empty()){
        int x = path.top().x;
        int y = path.top().y;
        if(x == lastX){
            if(y > lastY){
                newMaze[lastY+1][lastX] = 'v';
            }else{
                newMaze[lastY-1][lastX] = '^';
            }
        }else{
            if(x > lastX){
                newMaze[lastY][lastX+1] = '>';
            }else{
                newMaze[lastY][lastX-1] = '<';
            }
        }
        lastX = x;
        lastY = y;
        path.pop();
    }
    newMaze[lastY][lastX]= 'E';
    printMaze(newMaze);
    return;
}
bool checkForWall(const vector<vector<char> >& maze, const Directions& direction, const coord& currTile){
    int x = currTile.x;
    int y = currTile.y;
    switch(direction){
        case Directions::up:
            return maze[y+1][x] == '#';
        case Directions::down:
            return maze[y-1][x] == '#';
        case Directions::left:
            return maze[y][x-1] == '#';
        case Directions::right:
            return maze[y][x+1] == '#';
    }
}


//------------------------------------------------------------------------------------
void dijShortPath(const coord& start, const coord& end, const unordered_map<coord,Tile>& map){
    unordered_map<coord, bool> visited;
    unordered_map<coord, int> distance;
    unordered_map<coord, coord> previousPos;
    stack<coord> path;

    for(auto it = map.begin(); it != map.end(); ++it){
        distance[it->first] = INT_MAX;
        visited[it->first] = false;
    }

    distance[start] = 0;
    visited[start] = true;


    coord currCoord = start;
    int minDist;

    // find the shortest path
    while(!visited[end]){

        for (int i = 0; i < 4; i++){
            const Directions& dir = directions[i];
            const Tile& currTile = map.at(currCoord);
            const coord& adjacentCoord= currTile.adjacentTiles_[static_cast<int>(dir)]->position_; 

            if (currTile.adjacentTiles_[static_cast<int>(dir)] != NULL && !currTile.hasWall(dir)) {
                const int weight = currTile.weights_[static_cast<int>(dir)] + distance[currCoord];
                // check if the new weight to visit the adjecent tile is less than the current weight.
                if (weight < distance[adjacentCoord]) {
                    distance[adjacentCoord] = weight;
                    previousPos[adjacentCoord] = currCoord;
                }
            }
        }
        minDist = INT_MAX;
        // find the minimum distance
        // how to recognize the shrtpath if it hasn't been visited?
        for(auto it = distance.begin(); it != distance.end(); ++it){
            const coord& current = it-> first;
            const int currDist = it->second;

            if(currDist < minDist && !visited[current]){
                minDist = currDist;
                currCoord = current;
            }
        }
        visited[currCoord] = true;
    }
    //find path
    coord curr = end;
    while(curr != start){
        path.push(curr);
        curr = previousPos[curr];
    }
    path.push(start);
    printPath(path);

    return;
}
int checkRamp(const vector<vector<char>>& maze, const Directions& direction, const coord& currentTileCoord) {
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
}

//------------------------------------------------------------------------------------
void dfs(unordered_map<coord, Tile>& map){
    unordered_map<coord, bool> visited;
    stack<coord> unvisited;
    Tile* currTile;
    coord robotCoord = coord {1,1,1};
    map[robotCoord] = Tile(robotCoord);
    unvisited.push(robotCoord);
    bool wall; 
    bool alrConnected;

    coord nextCoord;
    Directions oppositeDir;

    while(!unvisited.empty()){
        coord currCoord = unvisited.top();
        unvisited.pop();
        if(visited.find(currCoord) != visited.end()){
            continue;
        }

        dijShortPath(robotCoord, currCoord, map);
        robotCoord = currCoord;
        visited[currCoord] = true;

        for(const Directions& dir : directions){
            wall = false; 
            switch(dir){
                case Directions::up:
                    nextCoord = coord{currCoord.x, currCoord.y + 2, checkRamp(maze, dir, currCoord)};
                    currTile = &map[currCoord];
                    oppositeDir = Directions::down;
                    break;
                case Directions::down:
                    nextCoord = coord{currCoord.x, currCoord.y - 2, checkRamp(maze, dir, currCoord)};
                    currTile = &map[currCoord];
                    oppositeDir = Directions::up;
                    break;
                case Directions::left:
                    nextCoord = coord{currCoord.x - 2, currCoord.y, checkRamp(maze, dir, currCoord)};
                    currTile = &map[currCoord];
                    oppositeDir = Directions::right;
                    break;
                case Directions::right:
                    nextCoord = coord{currCoord.x + 2, currCoord.y, checkRamp(maze, dir, currCoord)};
                    currTile = &map[currCoord];
                    oppositeDir = Directions::left;
                    break;
            }
            if(currTile->adjacentTiles_[static_cast<int>(dir)] == NULL){
                if (currCoord.z == 1 && nextCoord.z == 1){
                    wall =  checkForWall(maze, dir, currCoord);
                }else{
                    wall = checkForWall(mazeSecondLevel, dir, currCoord);
                }

                Tile* nextTile = &map[nextCoord];
                if(nextTile->position_ == InvalidPosition){
                    map[nextCoord].setPosition(nextCoord);
                }
                //link adjacent Tiles
                currTile->addAdjacentTile(dir, nextTile, wall);
                nextTile->addAdjacentTile(oppositeDir, currTile, wall);

                if(!wall){
                    if( visited.find(nextCoord) == visited.end()){
                        unvisited.push(nextCoord);
                    }
                }
            }
        }
    }
    return;
}

int main(){
    unordered_map<coord, Tile> map;
    dfs(map);
    return 0;
}

/*

11,12,13 Jan:
-adapting Rescue Maze 2024 Test
*/