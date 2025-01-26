#include <iostream>
#include <map>
#include <vector>
#include <climits>
using namespace std;

#include "maze.h"


Maze::Maze(vector<vector<char> > maze, int start_x, int start_y, int alg){
    //initialize maze and vectors for logic
    vector<vector<bool> > visited;
    vector<vector<int> > backstep;
    for(int i = 0; i < maze.size(); i++){
        vector<bool> temp;
        vector<int> temp2;
        for(int j = 0; j < maze[0].size(); j++){
            temp.push_back(false);
            temp2.push_back(INT_MAX);
        }
        visited.push_back(temp);
        backstep.push_back(temp2);
    }

    //initialize directions: up, left, down, right
    int directions[4][2] = {{0, 1}, {-1, 0}, {0, -1}, {1, 0}};

    //initialize count and pathFound
    int count = 0;
    bool pathFound = false;

    if (alg == 1){
        DFS(maze, visited, start_x, start_y, directions, backstep, count, pathFound);
    }else if(alg == 2){ 
        dijkstra(maze, start_x, start_y);
    }
}

void Maze::printMaze(vector<vector<char> > mazeToPrint) {
    for (int i = 0; i < mazeToPrint.size(); ++i) {
        for (int j = 0; j < mazeToPrint[i].size(); j++) {
            cout << mazeToPrint[i][j] << " ";
        }
        cout << endl;
    }
}
void Maze::DFS(vector<vector<char> >& maze, vector<vector<bool> > visited, int x, int y, int directions[4][2], vector<vector<int> > backstep, int& count, bool& pathFound){
    int rows = maze.size();
    int cols = maze[0].size();
    visited[x][y] = true;
    if(!pathFound){
        count++;
        backstep[x][y] = count;
    }
    if(x== rows-2 && y == cols-2){
        pathFound = true; 
    }
    for (int i = 0; i<4; i++){
        int newX = x + directions[0][0];
        int newY = y + directions[0][1];
        if((newX >= 0 && newY >=0 && newX<=rows && newY <= cols) && (paredAdelante() == false)){
            if (visited[newX][newY] == false){
                DFS(maze, visited, newX, newY, directions, backstep, count, pathFound);
            }else{
                visited[newX][newY] = true;
            }
        }
        girar(directions);
    }
}
//revisar 
void Maze::fuga(int count, int x, int y, int Mcolor, int directions[4][2], vector<vector<int> > backstep){
    for(int i = 0; i < count; i++){
        for (int j = 0; j < 4; j++){
            int newX = x + directions[0][0];
            int newY = y + directions[0][1];
            if (newX >= 0 && newY >= 0 && newX < 5 && newY < 3 && backstep[newX][newY] == i+1){
                ahead();
                j=4;
            }else{
                girar(directions);
            }
        }
    }
    ahead();
}
void Maze::girar(int directions[4][2]){
    int tempx = directions[0][0];
    int tempy = directions[0][1];
    for (int i = 0; i< 3; i++){
        directions[i][0] = directions[i+1][0];
        directions[i][1] = directions[i+1][1];
    }
    directions[3][0] = tempx;
    directions[3][1] = tempy;
    left();
}

// Struct for the priority queue (min-heap)
struct Node {
    int x, y, dist;
    Node(int x, int y, int dist) : x(x), y(y), dist(dist) {}
    bool operator>(const Node &other) const {
        return dist > other.dist;  // For min-heap based on distance
    }
};

// Dijkstra's Algorithm for solving the maze
void Maze::dijkstra(vector<vector<char> >& maze, int x, int y) {
    int rows = 3;
    int cols = 5;
    // Distance matrix initialized to infinity
    int dist[rows][cols];
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            dist[i][j] = INT_MAX;
        }
    }
    // Descending priority queue for the nodes to visit
    priority_queue<Node, vector<Node>, greater<Node> > pq;
    // Start point with 0 distance
    dist[start.first][start.second] = 0;
    pq.push(Node(start.first, start.second, 0));
    // Dijkstra's algorithm
    while (!pq.empty()) {
        // Get the current node
        Node current = pq.top();
        pq.pop();
        // Get the coordinates and distance of the current node
        int x = current.x, y = current.y, d = current.dist;

        // Print the current node being visited
        cout << "Visiting node (" << x << ", " << y << ") with distance " << d << endl;

        // If we reached the destination, print the distance and exit
        if (x == end.first && y == end.second) {
            cout << "Shortest distance from start to end: " << d << endl;
            return;
        }
        
        // Explore the four possible directions (right, down, left, up)
        for (int i = 0; i < 4; ++i) {
            int nx = x + directions[i][0];
            int ny = y + directions[i][1];

            // Check bounds and if the cell is open (0)
            if (nx >= 0 && ny >= 0 && nx < rows && ny < cols && maze[nx][ny] == 0) {
                // If a shorter path is found
                if (d + 1 < dist[nx][ny]) {
                    dist[nx][ny] = d + 1;
                    pq.push(Node(nx, ny, dist[nx][ny]));
                }
            }
        }
    }
    cout << "No path found from start to end." << endl;
}