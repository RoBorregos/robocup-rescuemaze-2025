
#include "algs/Tile.h"
#include "algs/arrCustom.h"
#include "algs/Stack.h"
#include "algs/heap.h"
#include "algs/coord.h"
#include "Encoder.h"
#include "Jetson.h"
class maze{
    public:
        void run_algs();
        maze();
        // void getDetectionJetson();
        // Jetson jetson;
    private:
        void followPath(Stack& path, arrCustom<Tile>& tiles, arrCustom<coord>& tilesMap);
        void dijkstra(coord& start, coord& end, arrCustom<coord>& tilesMap, arrCustom<Tile>& tiles);
        void dfs(arrCustom<coord>& visitedMap, arrCustom<Tile>& tiles, arrCustom<coord>& tilesMap);  
};