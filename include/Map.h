#ifndef MAP_H
#define MAP_H
#include "Entity.h"

class Map {
private:
    char grid[6][10][10]; 

public:
    Map();
    bool isWall(int room, int x, int y) const;
    void draw(int currentRoom, Entity* entities, int entityCount) const;
};

#endif
