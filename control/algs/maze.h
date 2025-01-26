#include <iostream>
using namespace std;

class Maze{
    public:
        Maze(vector<vector<char> > maze, int start_x, int start_y, int alg);
        void printMaze(vector<vector<char> > mazeToPrint);
        void DFS(vector<vector<char> >& maze, vector<vector<bool> > visited, int x, int y, int directions[4][2], vector<vector<int> > backstep, int& count, bool& pathFound);
        void dijkstra(vector<vector<char> >& maze, int x, int y);
        void fuga(int count, int x, int y, int Mcolor, int directions[4][2], vector<vector<int> > backstep);
        void girar(int directions[4][2]);
};