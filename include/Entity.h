#ifndef ENTITY_H
#define ENTITY_H

enum EntityType { PLAYER, ENEMY_BAT, ENEMY_DRAGON, ITEM_CHALICE, PROJECTILE, FINAL_BOSS };

class Entity {
public:
    int x;
    int y;
    int currentRoom;
    EntityType type;
    bool active;
    bool hasItem; 
    int health; // Vidas para el jefe final
    
    int dirX;
    int dirY;

    Entity();
    Entity(int startX, int startY, int room, EntityType t);

    void move(int dx, int dy);
};

#endif
