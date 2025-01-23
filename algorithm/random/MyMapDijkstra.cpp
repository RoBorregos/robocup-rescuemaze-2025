#include <iostream>
#include <vector>
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

struct coord {
    int x;
    int y;
};
enum Direction {
    kUp,
    kDown,
    kLeft,
    kRight
};
struct Tile{
    coord position;
    vector<Tile*> adjacentTiles_;
    vector<int> weights_;  // The weight for each direction (could be 1 for normal moves)
    
    Tile(coord pos) : position(pos) {
        adjacentTiles_.resize(4, nullptr);  // 4 directions (Up, Down, Left, Right)
        weights_ = {1, 1, 1, 1};  // Default weight for each direction (1 for simplicity)
    }

    void addAdjacentTile(Direction dir, Tile* adjacentTile) {
        adjacentTiles_[dir] = adjacentTile;
    }
};
int steps = 0; 
char robotChar = 'v';
coord robotCoord = {1,1};
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


int getIndex(vector<coord>& positions, const coord& find) {
    for (int i = 0; i < positions.size(); ++i) {
        if (positions[i].x == find.x && positions[i].y == find.y) {
            return i;
        }
    }
    return -1;  // Coordinate not found
}
// Dijkstra's algorithm using a priority queue
void dijkstra(coord& start, vector<coord>& positions, vector<Tile>& tiles) {
    vector<int> distance(positions.size(), INT_MAX);  // Initialize all distances to infinity
    vector<bool> visited(positions.size(), false);   // Track if the tile is visited
    stack<coord> path;  // Store the path if needed

    int startIdx = getIndex(positions, start);
    distance[startIdx] = 0; 
    while (true) {
        int minDist = INT_MAX;
        int currentIdx = -1;
        
        for(int i = 0; i < positions.size(); ++i){
            if(!visited[i] && distance[i] < minDist){
                minDist = distance[i];
                currentIdx = i;
            }
        }
        if(currentIdx == -1) break;

        visited[currentIdx] = true;
        coord currentCoord = positions[currentIdx];

        cout << "Visiting (" << currentCoord.x << ", " << currentCoord.y << ")" << "\n";

        Tile* currentTile = &tiles[currentIdx];
        for(Direction dir = kUp; dir <= kRight; dir = static_cast<Direction>(dir + 1)){
            if(currentTile->adjacentTiles_[dir] != nullptr){
                Tile* adjacentTile = currentTile->adjacentTiles_[dir];
                int adjacentIdx = getIndex(positions, adjacentTile->position);
                if(!visited[adjacentIdx] && distance[currentIdx] + currentTile->weights_[dir] < distance[adjacentIdx]){
                    distance[adjacentIdx] = distance[currentIdx] + currentTile->weights_[dir];
                }
            }
            turnCharL(robotChar);
        }
    }
}
void solveMaze() {
    vector<coord> positions;
    vector<Tile> tiles;

    // Initialize positions and tiles
    for (int y = 0; y < discoverMaze.size(); ++y) {
        for (int x = 0; x < discoverMaze[y].size(); ++x) {
            if (discoverMaze[y][x] != '#') {
                coord pos = {x, y};
                positions.push_back(pos);
                tiles.emplace_back(pos);
            }
        }
    }

    // Set adjacent tiles
    for (Tile& tile : tiles) {
        coord pos = tile.position;
        if (pos.y > 0 && discoverMaze[pos.y - 1][pos.x] != '#') {
            tile.addAdjacentTile(kUp, &tiles[getIndex(positions, {pos.x, pos.y - 1})]);
        }
        if (pos.y < discoverMaze.size() - 1 && discoverMaze[pos.y + 1][pos.x] != '#') {
            tile.addAdjacentTile(kDown, &tiles[getIndex(positions, {pos.x, pos.y + 1})]);
        }
        if (pos.x > 0 && discoverMaze[pos.y][pos.x - 1] != '#') {
            tile.addAdjacentTile(kLeft, &tiles[getIndex(positions, {pos.x - 1, pos.y})]);
        }
        if (pos.x < discoverMaze[0].size() - 1 && discoverMaze[pos.y][pos.x + 1] != '#') {
            tile.addAdjacentTile(kRight, &tiles[getIndex(positions, {pos.x + 1, pos.y})]);
        }
    }

    // Start Dijkstra's algorithm from the robot's initial position
    dijkstra(robotCoord, positions, tiles);
}

int main() {
    solveMaze();
    return 0;
}