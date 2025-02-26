
#include "algs/Tile.h"
#include "algs/arrCustom.h"
#include "algs/Stack.h"
#include "algs/heap.h"
#include "algs/coord.h"
#include "Encoder.h"

class maze{
    public:
        void run_algs();
        maze();
        void followPath(Stack& path);
        void dijkstra(coord& start, coord& end, arrCustom<coord>& tilesMap, arrCustom<Tile>& tiles);
        void dfs(arrCustom<coord>& visitedMap, arrCustom<Tile>& tiles, arrCustom<coord>& tilesMap);  
};