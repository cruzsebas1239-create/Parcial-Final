#include "Entity.h"

Entity::Entity() {
    x = 0; y = 0; currentRoom = 0; type = PLAYER; active = false; hasItem = false;
}

Entity::Entity(int startX, int startY, int room, EntityType t) {
    x = startX; y = startY; currentRoom = room; type = t; active = true; hasItem = false;
}

void Entity::move(int dx, int dy) {
    x += dx;
    y += dy;
}