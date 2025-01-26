#include <iostream>
#include <queue>
#include <climits>

using namespace std;

// Directions: up, right, down, left
int directions[4][2] = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}};

// Struct for the priority queue (min-heap)
struct Node {
    int x, y, dist;
    Node(int x, int y, int dist) : x(x), y(y), dist(dist) {}
    bool operator>(const Node &other) const {
        return dist > other.dist;  // For min-heap based on distance
    }
};

// Dijkstra's Algorithm for solving the maze
void dijkstra(int maze[3][5], pair<int, int> start, pair<int, int> end) {
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

// sample usage

int main() {
    // Define the maze: 0 is free, 1 is wall
    int maze[3][5] = {
        {0, 1, 1, 0, 0},
        {0, 0, 0, 0, 1},
        {0, 1, 0, 0, 0}
    };
    pair<int, int> start = make_pair(0, 0);  // Start point (top-left corner)
    pair<int, int> end = make_pair(2, 4);    // End point (bottom-right corner) 

    // Run Dijkstra's algorithm
    dijkstra(maze, start, end);
    return 0;
}
