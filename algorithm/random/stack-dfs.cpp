#include <iostream>
#include <stack>
#include <vector>
#include <tuple>  // for std::tuple

using namespace std;

const int N = 5;  // Size of the 5x5 maze
int maze[N][N] = {
    {0, 1, 0, 0, 0},
    {0, 1, 0, 1, 0},
    {0, 1, 0, 0, 0},
    {0, 0, 0, 1, 0},
    {0, 1, 0, 0, 0}
};

bool visited[N][N] = {false};  // Visited cells

// Directions for moving in the maze (up, down, left, right)
int dirX[] = {-1, 1, 0, 0};  // Row movement: up(-1), down(1), left(0), right(0)
int dirY[] = {0, 0, -1, 1};  // Column movement: up(0), down(0), left(-1), right(1)

// Check if a position is inside the maze and not a wall
bool isValid(int x, int y) {
    return x >= 0 && x < N && y >= 0 && y < N && maze[x][y] == 0 && !visited[x][y];
}

// DFS using a stack
void dfs(int startX, int startY) {
    stack<pair<int, int>> s;  // Stack to store the current position (row, column)
    s.push({startX, startY});
    visited[startX][startY] = true;

    while (!s.empty()) {
        int x, y;
        tie(x, y) = s.top();  // Get the current position from the stack
        s.pop();

        cout << "Visiting (" << x << ", " << y << ")\n";

        // Check if we have reached the goal (bottom-right corner)
        if (x == N-1 && y == N-1) {
            cout << "Goal reached!" << endl;
            return;
        }

        // Explore neighbors (up, down, left, right)
        for (int i = 0; i < 4; ++i) {
            int newX = x + dirX[i];
            int newY = y + dirY[i];

            // If the move is valid, mark the position as visited and push to stack
            if (isValid(newX, newY)) {
                visited[newX][newY] = true;
                s.push({newX, newY});
            }
        }
    }

    cout << "No path to goal.\n";  // If no path found
}

int main() {
    // Starting position is (0, 0)
    dfs(0, 0);

    return 0;
}
