
#include "algs/Tile.h"
#include "algs/arrCustom.h"
#include "algs/Stack.h"
#include "algs/heap.h"
#include "algs/coord.h"
#include "Encoder.h"

class maze{
    public:
        /*
        void ahead();
        void right();
        void left();
        void rotate(const int targetOrientation);
        bool vlx(int num);
        bool isWall(const TileDirection& direction);
        */
        maze();
        void followPath(Stack& path);
        void dijkstra(coord& start, coord& end, arrCustom<coord>& tilesMap, arrCustom<Tile>& tiles);
        void dfs(arrCustom<coord>& visitedMap, arrCustom<Tile>& tiles, arrCustom<coord>& tilesMap);
        
        
        void run_algs();
};