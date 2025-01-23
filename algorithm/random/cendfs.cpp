#include <iostream>
#include <vector>
#include <stack>
#include "coord.h"
#include "Tile.cpp"
#include "TileDirection.h"

using namespace std;
// Directions for exploring the maze (Up, Down, Left, Right)
enum Direction { UP, DOWN, LEFT, RIGHT };

// Maze size and structure
std::vector<std::vector<char> > RealMaze = {
        {'#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#'},
        {'#', ' ', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', '#'}, // 1.
        {'#', ' ', '.', ' ', '.', ' ', '.', '#', '.', '#', '#'},
        {'#', ' ', '#', ' ', '#', ' ', '#', ' ', '#', ' ', '#'}, // 3.
        {'#', ' ', '.', '#', '.', ' ', '.', ' ', '.', '#', '#'},
        {'#', ' ', ' ', ' ', '#', ' ', '#', ' ', ' ', ' ', '#'}, // 5.
        {'#', '#', '.', '#', '.', '#', '.', '#', '.', ' ', '#'},
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

std::vector<std::vector<char> > discoverMaze ={
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
const int rows = 17;
const int cols = 11;
int steps = 0;
coord robotCoord = {1,1};
char robotChar = 'v';
// Directions for movement (up, right, down, left)
int directions[4][2] = {{0,2},{2,0},{0,-2},{-2,0}};
void right(){
    switch(robotChar){
        case '^':
            robotChar = '>';
            break;
        case '>':
            robotChar = 'v';
            break;
        case 'v':
            robotChar = '<';
            break;
        case '<':
            robotChar = '^';
            break;
    }
    discoverMaze[robotCoord.y][robotCoord.x] = robotChar;
}
void girar(int directions[4][2]){
    int tempx = directions[0][0];
    int tempy = directions[0][1];
    for (int i = 0; i< 3; i++){
        directions[i][0] = directions[i+1][0];
        directions[i][1] = directions[i+1][1];
    }
    directions[3][0] = tempx;
    directions[3][1] = tempy;
    right();
}

bool paredAdelante(coord next){
    if(RealMaze[next.y- (directions[0][1]/2)][next.x-(directions[0][0]/2)] == '#'){
        return true;
    }
    return false;
    
}
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
// DFS algorithm to explore the maze using a stack
void dfsExplore(coord start) {
    stack<coord> unvisited;
    vector<vector<bool> > visited(rows, vector<bool>(cols, false));

    stack.push(start); // Push the starting point to the stack

    while (!stack.empty()) {
        coord current = stack.top();
        stack.pop();

        // Skip if already visited
        if (visited[current.y][current.x]) {
            //back(current);
            continue;
        }

        // Mark the current position as visited
        visited[current.y][current.x] = true;

        // Process the current position (e.g., print it or mark it in the maze)
        cout << "Visiting: (" << current.y << ", " << current.x << ")" << endl;

        // Explore neighboring positions
        for (auto& dir : directions) {
            int newX = current.x + directions[0][0];
            int newY = current.y + directions[0][1];
            coord next = {newX, newY};
            if((next.x >= 0 && next.y >= 0 && next.x<cols && next.y <rows) && (visited[next.y][next.x] == false)){
                if((paredAdelante(next) == false)){
                    ahead(next);
                    printMaze(discoverMaze);
                    stack.push(next);
                }
                else{
                    discoverMaze[next.y- (directions[0][1]/2)][next.x-(directions[0][0]/2)] = 'X';
                    printMaze(discoverMaze);
                }
            }
            girar(directions);
            printMaze(discoverMaze);
        }
    }
    cout << "Maze exploration complete!" << endl;
}

int main() {
    coord start = {1, 1};  // Starting position in the maze
    dfsExplore(start);      // Start DFS exploration
    return 0;
}
