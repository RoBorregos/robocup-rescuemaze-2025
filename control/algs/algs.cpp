#include <vector>
#include <queue>
#include <climits>
#include <iostream>
using namespace std;

void ahead(){}
void back(){}
void left(){}
void right(){}
bool paredAdelante(){}

int rows = 0;
int cols = 0;

void girar(int directions[4][2]){
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

// DFS algorithm
void DFS(bool visited[rows][cols], int x, int y, int directions[4][2], int rows, int cols){
    visited[x][y] = true;
    
    for (int i = 0; i<4; i++){
        int newX = x + directions[0][0];
        int newY = y + directions[0][1];
        // Serial.println(newY);
        // delay(100);
        if((newX >= 0 && newY >= 0 && newX<rows && newY <cols) && (paredAdelante() == false)){
            if(visited[newX][newY] == false){
                ahead();
                if(lineaNegra == false){
                    DFS(visited, newX, newY, directions, backstep, cnt, pathFound);
                    back();
                }
                else{
                    visited[newX][newY] = true;
                }
            }
        }
        girar(directions);
    }
}

// Node struct for the priority queue
struct Node{
    // x and y coordinates of the node, and the distance from the start
    int x, y, dist;
    // COnstructor
    Node(int x, int y, int dist) : x(x), y(y), dist(dist) {}
    // Compare nodes based on distance
    bool operator>(const Node &other) const {
        return dist > other.dist;
    }
}

// djikstra algorithm
void djikstra(bool visited[3][5], int x, int y, int directions[4][2], int rows, int cols){
    int dist[rows][cols];
    for (int i = 0; i<rows; i++){
        for (int j = 0; j<cols; j++){
            dist[i][j] = INT_MAX;
        }
    }
    priority_queue<Node, vector<Node>, greater<Node>> pq;
    dist[x][y] = 0;
    pq.push(Node(x, y, 0));

    while(!pq.empty()){
        Node current = pq.top();
        pq.pop();
        int x = current.x 
        int y = current.y
        int d = current.dist;
        if(x == rows && y == cols){
            cout << "Shortest distance from start to end: " << d << endl;
            return;
        }
        for(int i = 0; i<4; i++){
            int newX = x + directions[0][0];
            int newY = y + directions[0][1];
            if((newX >= 0 && newY >= 0 && newX < rows && newY < cols) && (paredAdelante() == false)){
                if(visited[newX][newY] == false){
                    ahead();
                    if(d + 1 < dist[newX][newY]){
                        dist[newX][newY] = d + 1;
                        pq.push(Node(newX, newY, dist[newX][newY]));
                    }
                    else{
                        visited[newX][newY] = true;
                    }
                }
            }
            girar();
        }

    }
}

//main function
void maze() {
    rows = 5;
    cols = 5;
    //adelante, derecha, atras, izquierda
    int directions[4][2] = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}};
    bool visited[rows][cols] = {{false}}; 
    int start_x = 0, start_y = 0;
    DFS(visited, start_x, start_y, directions, rows, cols);
    djikstra(visited, start_x, start_y, directions, rows, cols);
}
