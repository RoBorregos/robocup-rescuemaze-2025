#include <iostream>
#include <vector>
#include <stack>

using namespace std;

struct coord {
    int x, y;
};

// Direcciones para explorar el laberinto (Arriba, Abajo, Izquierda, Derecha)
enum Direction { UP, RIGHT, DOWN, LEFT };

// Laberinto y sus características
const int ROWS = 20;
const int COLS = 20;
vector<vector<char>> maze = {
    
        {'#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#'},
        {'#', ' ', ' ', ' ', '#', ' ', '#', ' ', ' ', ' ', '#'}, // 1.
        {'#', ' ', '.', ' ', '.', ' ', '.', '#', '.', '#', '#'},
        {'#', ' ', '#', ' ', ' ', ' ', '#', ' ', '#', ' ', '#'}, // 3.
        {'#', ' ', '.', ' ', '.', ' ', '.', ' ', '.', '#', '#'},
        {'#', ' ', '#', ' ', ' ', ' ', '#', ' ', ' ', ' ', '#'}, // 5.
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
vector<vector<char> > discoverMaze ={
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

vector<coord> directions = {
    {0, -2}, // Arriba
    {-2, 0},  // Derecha
    {0, 2},  // Abajo
    {2, 0} // Izquierda
    
};
int steps = 0;
char robotChar = 'v';
coord robotCoord = {1, 1};
// Comprobar si la posición es válida y dentro de los límites
bool isValid(int x, int y, const vector<vector<bool>>& visited) {
    return (x >= 0 && x < ROWS && y >= 0 && y < COLS && maze[x][y] != '#' && !visited[x][y]);
}
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
    discoverMaze[robotCoord.x][robotCoord.y] = robotChar;
}
void ahead(coord next){
    discoverMaze[robotCoord.x][robotCoord.y] = '*';
    robotCoord = next;
    discoverMaze[robotCoord.x][robotCoord.y] = robotChar;
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

// DFS para recorrer completamente el laberinto
void dfsExplore(coord start) {
    stack<coord> stack;
    vector<vector<bool>> visited(ROWS, vector<bool>(COLS, false));

    stack.push(start); // Push la posición inicial al stack

    while (!stack.empty()) {
        coord current = stack.top();
        stack.pop();

        // Si ya fue visitado, continuar con el siguiente
        if (visited[current.x][current.y]) {
            continue;
        }

        // Marcar la posición como visitada
        visited[current.x][current.y] = true;

        // Procesar la celda actual (por ejemplo, imprimirla)
        cout << "Visiting: (" << current.x << ", " << current.y << ")" << endl;
        ahead(current);

        // Explorar las celdas vecinas (arriba, abajo, izquierda, derecha)
        for (const auto& dir : directions) {
            int newX = current.x + dir.x;
            int newY = current.y + dir.y;
            int wallX = current.x + dir.x/2;
            int wallY = current.y + dir.y/2;

            if (newX >= 0 && newX < ROWS && newY >= 0 && newY < COLS && maze[wallX][wallY] != '#' && !visited[newX][newY]) {
                stack.push({newX, newY});  // Push las posiciones vecinas válidas al stack
            }else{
                if(maze[wallX][wallY] == '#'){
                    discoverMaze[wallX][wallY] = 'X';
                }
                
            }
            right();
            printMaze(discoverMaze);
        }
    }
    cout << "Maze exploration complete!" << endl;
}

int main() {
    coord start = {1, 1};  // Posición inicial en el laberinto
    dfsExplore(start); 
    cout<< steps;     // Comienza la exploración DFS
    return 0;
}
//output
/*
# X # X # X # X # X # 
X * . * . * . * . * X 
# . . . . . . X . X # 
X * X * X * . * X . # 
# . . X . . . . . X # 
X * . * . * X * . * X 
# . . X . . . X . . # 
X * X * . * . * . * X 
# . . . . . . . . . # 
X * . * X * . * . * X 
# . . . . . . . . . # 
X * . * . * . * . * X 
# . . X . . . X . . # 
X * . v X * X * . * X 
# . . X . . . X . . # 
X * . * . * . * . * X 
# X # X # X # X # X # 
Maze exploration complete!
156%  
# X # X # X # # # # # 
X * . * X v X . . . # 
# . . . . . . . . . # 
X * X * . * X . . . # 
# . . . . . . . . . # 
X * X * . * X . . . # 
# X . X . X . . . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# . . . . . . . . . # 
# # # # # # # # # # # 
Maze exploration complete!
36% 

*/
