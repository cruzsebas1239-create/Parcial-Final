#include "Game.h"
#include <iostream>
#include <conio.h>   
#include <cstdlib>   

static int playerSteps = 0;

Game::Game() {
    isRunning = true;
    hasWon = false;
    
    // Aumentamos a 8 entidades (Jugador, 5 enemigos normales, 1 Jefe Final, 1 Cáliz)
    entityCount = 8; 
    
    // Habitación 0
    entities[0] = Entity(1, 1, 0, PLAYER);
    entities[1] = Entity(5, 5, 0, ENEMY_BAT);       
    
    // Habitación 1
    entities[2] = Entity(7, 7, 1, ENEMY_DRAGON);    
    entities[4] = Entity(3, 3, 1, ENEMY_BAT);       
    
    // Habitación 2
    entities[5] = Entity(5, 4, 2, ENEMY_DRAGON);    
    
    // Habitación 3
    entities[6] = Entity(4, 6, 3, ENEMY_DRAGON);    
    
    // Habitación 5 (¡La guarida del Jefe!)
    entities[7] = Entity(5, 5, 5, FINAL_BOSS);     // Creamos al Jefe Final 'J'
    entities[7].health = 3;                         // Tiene 3 vidas
    
    entities[3] = Entity(8, 5, 5, ITEM_CHALICE);    // Movimos el Cáliz al final del juego (Habitación 5)

    maxBullets = 3;
    lastDirX = 1;  
    lastDirY = 0;
    for(int i = 0; i < maxBullets; i++) {
        bullets[i].active = false;
        bullets[i].type = PROJECTILE;
    }
}

void Game::run() {
    while(isRunning) {
        render();
        processInput();
        update();
        checkCollisions();
    }
    
    system("cls"); 
    if(hasWon) {
        std::cout << "=================================================\n";
        std::cout << "  ¡VICTORIA EPICA! Derrotaste al jefe y ganaste. \n";
        std::cout << "=================================================\n";
    } else {
        std::cout << "=================================================\n";
        std::cout << "  ¡GAME OVER! El Jefe o sus secuaces te vencieron.\n";
        std::cout << "=================================================\n";
    }
}

void Game::shoot() {
    Entity* player = &entities[0];
    
    int bulletX = player->x + lastDirX;
    int bulletY = player->y + lastDirY;
    
    while(!gameMap.isWall(player->currentRoom, bulletX, bulletY)) {
        for(int i = 1; i < entityCount; i++) {
            Entity* e = &entities[i];
            if(e->active && e->type != ITEM_CHALICE && e->currentRoom == player->currentRoom && e->x == bulletX && e->y == bulletY) {
                
                // Si el impacto es contra el Jefe Final
                if(e->type == FINAL_BOSS) {
                    e->health--; // Le quitamos una vida
                    if(e->health <= 0) {
                        e->active = false; // Muere si llega a 0
                    } else {
                        // Si le quedan vidas, se teletransporta a otra esquina para esquivarte
                        e->x = (e->x == 5) ? 2 : 5;
                        e->y = (e->y == 5) ? 7 : 5;
                    }
                } else {
                    // Si es un enemigo común, muere de un tiro
                    e->active = false; 
                }
                return; 
            }
        }
        bulletX += lastDirX;
        bulletY += lastDirY;
    }
}

void Game::processInput() {
    char input = _getch(); 
    
    Entity* player = &entities[0];
    int nextX = player->x;
    int nextY = player->y;

    int currentDirX = 0;
    int currentDirY = 0;

    if(input == 'w' || input == 'W') { nextY--; currentDirY = -1; playerSteps++; }
    else if(input == 's' || input == 'S') { nextY++; currentDirY = 1; playerSteps++; }
    else if(input == 'a' || input == 'A') { nextX--; currentDirX = -1; playerSteps++; }
    else if(input == 'd' || input == 'D') { nextX++; currentDirX = 1; playerSteps++; }
    else if(input == 'f' || input == 'F') { shoot(); return; } 
    else if(input == 'q' || input == 'Q') isRunning = false;
    else if(input == 'e' || input == 'E') {
        Entity* chalice = &entities[3];
        if(!player->hasItem && chalice->currentRoom == player->currentRoom && chalice->x == player->x && chalice->y == player->y) {
            player->hasItem = true;
            chalice->active = false;
        } else if(player->hasItem) {
            player->hasItem = false;
            chalice->active = true;
            chalice->x = player->x;
            chalice->y = player->y;
            chalice->currentRoom = player->currentRoom;
        }
    }

    if(currentDirX != 0 || currentDirY != 0) {
        lastDirX = currentDirX;
        lastDirY = currentDirY;
    }

    if (nextX > 9 && player->currentRoom < 5) {
        player->currentRoom++;
        player->x = 0; 
        return;
    } else if (nextX < 0 && player->currentRoom > 0) {
        player->currentRoom--;
        player->x = 9; 
        return;
    }

    if(!gameMap.isWall(player->currentRoom, nextX, nextY)) {
        player->x = nextX;
        player->y = nextY;
    }
}

void Game::update() {
    Entity* player = &entities[0];
    
    if (playerSteps % 2 != 0) {
        return; 
    }

    for(int i = 1; i < entityCount; i++) {
        Entity* e = &entities[i];
        if(!e->active || e->type == ITEM_CHALICE) continue;

        if(e->currentRoom == player->currentRoom) {
            int dx = 0;
            int dy = 0;
            if(player->x > e->x) dx = 1;
            else if(player->x < e->x) dx = -1;
            
            if(player->y > e->y) dy = 1;
            else if(player->y < e->y) dy = -1;

            if(!gameMap.isWall(e->currentRoom, e->x + dx, e->y + dy)) {
                e->move(dx, dy);
            }
        }
    }
}

void Game::checkCollisions() {
    Entity* player = &entities[0];
    
    for(int i = 1; i < entityCount; i++) {
        Entity* e = &entities[i];
        if(!e->active || e->type == ITEM_CHALICE) continue;
        
        if(e->currentRoom == player->currentRoom && e->x == player->x && e->y == player->y) {
            isRunning = false;
        }
    }
    
    if(player->hasItem && player->currentRoom == 0 && player->x == 1 && player->y == 1) {
        hasWon = true;
        isRunning = false;
    }
}

void Game::render() const {
    system("cls"); 
    
    const Entity* p = &entities[0];
    std::cout << "===================================================\n";
    std::cout << " CONTROLES: W/A/S/D | [ F ]: Disparo Láser | E: Usar\n";
    std::cout << "===================================================\n";
    std::cout << "Habitacion actual: " << p->currentRoom;
    
    // Si estamos en la habitación del jefe y sigue vivo, mostramos su barra de vida
    if(p->currentRoom == 5 && entities[7].active) {
        std::cout << " | VIDA DEL JEFE [J]: ";
        for(int h = 0; h < entities[7].health; h++) std::cout << "X ";
    }
    std::cout << "\n";
    
    if(p->hasItem) {
        std::cout << "[Inventario: CALIZ (1/1)]\n\n";
    } else {
        std::cout << "[Inventario: Vacio (0/1)]\n\n";
    }
    
    gameMap.draw(p->currentRoom, const_cast<Entity*>(entities), entityCount);
}