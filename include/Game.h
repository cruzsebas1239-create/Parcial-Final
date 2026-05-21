#ifndef GAME_H
#define GAME_H
#include "Map.h"
#include "Entity.h"

class Game {
private:
    Map gameMap;
    Entity entities[10]; 
    int entityCount;

    Entity bullets[3];
    int maxBullets;
    int lastDirX;
    int lastDirY;

    bool isRunning;
    bool hasWon;

    void processInput();
    void update();
    void render() const;
    void checkCollisions();
    void shoot();

public:
    Game();
    void run();
};
