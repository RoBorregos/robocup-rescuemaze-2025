#include <functional>

struct coord{
    int x;
    int y;
    int z; 
    bool const operator==(const coord &other) const{
        return x == other.x && y == other.y && z == other.z;
    }
    bool const operator!=(const coord &other) const{
        return x != other.x || y != other.y || z != other.z;
    }
    bool const operator<(const coord &other) const{
        return x < other.x || (x == other.x && y < other.y) || (x == other.x && y == other.y && z < other.z);
    }
};
/*
namespace etl{
    template <>
    struct hash<coord>{
        size_t operator()(const coord &c) const{
            return hash<int>()(c.x) ^ hash<int>()(c.y) ^ hash<int>()(c.z);
        }
    };
}
*/
namespace std{
    template <>
    struct hash<coord>{
        size_t operator()(const coord &c) const{
            return std::hash<int>()(c.x) ^ std::hash<int>()(c.y) ^ std::hash<int>()(c.z);
        }
    };
}