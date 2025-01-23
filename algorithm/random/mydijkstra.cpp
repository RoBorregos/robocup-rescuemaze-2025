#include <iostream>
#include <vector>
#include <climits>  // for INT_MAX
#include <stack>    // for stack

using namespace std;

struct coord {
    int x, y;
};

// Directions for exploring the maze (Up, Down, Left, Right)
enum TileDirection { kUp, kDown, kLeft, kRight };

struct Tile {
    coord position;
    vector<Tile*> adjacentTiles_;
    vector<int> weights_;  // The weight for each direction (could be 1 for normal moves)
    
    Tile(coord pos) : position(pos) {
        adjacentTiles_.resize(4, nullptr);  // 4 directions (Up, Down, Left, Right)
        weights_ = {1, 1, 1, 1};  // Default weight for each direction (1 for simplicity)
    }

    void addAdjacentTile(TileDirection dir, Tile* adjacentTile) {
        adjacentTiles_[dir] = adjacentTile;
    }
};

// Get index of a coordinate in the positions list
int getIndex(vector<coord>& positions, const coord& find) {
    for (int i = 0; i < positions.size(); ++i) {
        if (positions[i].x == find.x && positions[i].y == find.y) {
            return i;
        }
    }
    return -1;  // Coordinate not found
}

void dijkstraWithoutPriorityQueue(coord& start, vector<coord>& positions, vector<Tile>& tiles) {
    vector<int> distance(positions.size(), INT_MAX);  // Initialize all distances to infinity
    vector<bool> visited(positions.size(), false);   // Track if the tile is visited
    stack<coord> path;  // Store the path if needed

    int startIdx = getIndex(positions, start);
    distance[startIdx] = 0;  // Starting point has distance 0

    // Main loop of Dijkstra's algorithm without priority queue
    while (true) {
        // Find the tile with the minimum distance that hasn't been visited
        int minDist = INT_MAX;
        int currentIdx = -1;
        for (int i = 0; i < positions.size(); ++i) {
            if (!visited[i] && distance[i] < minDist) {
                minDist = distance[i];
                currentIdx = i;
            }
        }

        // If no unvisited tile has a valid distance, break the loop
        if (currentIdx == -1) break;

        visited[currentIdx] = true;
        coord currentCoord = positions[currentIdx];

        // Explore neighbors (adjacent tiles)
        Tile* currentTile = &tiles[currentIdx];
        for (TileDirection dir = kUp; dir <= kRight; dir = static_cast<TileDirection>(dir + 1)) {
            if (currentTile->adjacentTiles_[dir] != nullptr) {
                Tile* adjacentTile = currentTile->adjacentTiles_[dir];
                int adjacentIdx = getIndex(positions, adjacentTile->position);

                if (!visited[adjacentIdx] && distance[currentIdx] + currentTile->weights_[dir] < distance[adjacentIdx]) {
                    distance[adjacentIdx] = distance[currentIdx] + currentTile->weights_[dir];
                    // Optionally store the previous position for path reconstruction
                    // previousPositions[adjacentIdx] = currentCoord;
                }
            }
        }
    }

    // Print out the distance to each tile
    for (int i = 0; i < positions.size(); ++i) {
        cout << "Distance to (" << positions[i].x << ", " << positions[i].y << ") is " << distance[i] << endl;
    }
}

int main() {
    // Example of creating a small 3x3 maze with tiles
    vector<coord> positions = {
        {0, 0}, {0, 1}, {0, 2},
        {1, 0}, {1, 1}, {1, 2},
        {2, 0}, {2, 1}, {2, 2}
    };
    vector<Tile> tiles;
    for (const auto& pos : positions) {
        tiles.push_back(Tile(pos));
    }

    // Set up adjacent tiles for each tile
    tiles[0].addAdjacentTile(kRight, &tiles[1]);
    tiles[0].addAdjacentTile(kDown, &tiles[3]);

    tiles[1].addAdjacentTile(kLeft, &tiles[0]);
    tiles[1].addAdjacentTile(kRight, &tiles[2]);
    tiles[1].addAdjacentTile(kDown, &tiles[4]);

    tiles[2].addAdjacentTile(kLeft, &tiles[1]);
    tiles[2].addAdjacentTile(kDown, &tiles[5]);

    tiles[3].addAdjacentTile(kUp, &tiles[0]);
    tiles[3].addAdjacentTile(kRight, &tiles[4]);
    tiles[3].addAdjacentTile(kDown, &tiles[6]);

    tiles[4].addAdjacentTile(kUp, &tiles[1]);
    tiles[4].addAdjacentTile(kLeft, &tiles[3]);
    tiles[4].addAdjacentTile(kRight, &tiles[5]);
    tiles[4].addAdjacentTile(kDown, &tiles[7]);

    tiles[5].addAdjacentTile(kUp, &tiles[2]);
    tiles[5].addAdjacentTile(kLeft, &tiles[4]);
    tiles[5].addAdjacentTile(kDown, &tiles[8]);

    tiles[6].addAdjacentTile(kUp, &tiles[3]);
    tiles[6].addAdjacentTile(kRight, &tiles[7]);

    tiles[7].addAdjacentTile(kUp, &tiles[4]);
    tiles[7].addAdjacentTile(kLeft, &tiles[6]);
    tiles[7].addAdjacentTile(kRight, &tiles[8]);

    tiles[8].addAdjacentTile(kUp, &tiles[5]);
    tiles[8].addAdjacentTile(kLeft, &tiles[7]);

    coord start = {0, 0};  // Starting position for the robot

    // Run Dijkstra's algorithm to find shortest paths to all tiles
    dijkstraWithoutPriorityQueue(start, positions, tiles);

    return 0;
}
