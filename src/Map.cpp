#include "Map.h"
#include <iostream>

Map::Map() {
    for(int r = 0; r < 6; r++) {
        for(int y = 0; y < 10; y++) {
            for(int x = 0; x < 10; x++) {
                if(y == 0 || y == 9 || x == 0 || x == 9) {
                    if (r < 5 && x == 9 && y == 5) {
                        grid[r][y][x] = ' ';
                    } else if (r > 0 && x == 0 && y == 5) {
                        grid[r][y][x] = ' ';
                    } else {
                        grid[r][y][x] = '#';
                    }
                } else {
                    grid[r][y][x] = '.';
                }
            }
        }
    }
}

bool Map::isWall(int room, int x, int y) const {
    if(x < 0 || x > 9 || y < 0 || y > 9) return true;
    return grid[room][y][x] == '#';
}

void Map::draw(int currentRoom, Entity* entities, int entityCount) const {
    for(int y = 0; y < 10; y++) {
        for(int x = 0; x < 10; x++) {
            char toDraw = grid[currentRoom][y][x];
            
            for(int i = 0; i < entityCount; i++) {
                Entity* e = &entities[i];
                if(e->active && e->currentRoom == currentRoom && e->x == x && e->y == y) {
                    if(e->type == PLAYER) toDraw = 'P';
                    else if(e->type == ENEMY_BAT) toDraw = 'B';
                    else if(e->type == ENEMY_DRAGON) toDraw = 'D';
                    else if(e->type == ITEM_CHALICE) toDraw = 'C';
                    else if(e->type == PROJECTILE) toDraw = '*';
                    else if(e->type == FINAL_BOSS) toDraw = 'J'; // Representación del Jefe Final
                }
            }
            std::cout << toDraw << ' ';
        }
        std::cout << '\n';
    }
}