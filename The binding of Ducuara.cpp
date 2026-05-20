#include <iostream>
#include <vector>
#include <conio.h>
#include <chrono>
#include <thread>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <string>

const int ANCHO = 25;
const int ALTO = 15;

enum class Direccion { NINGUNA, ARRIBA, ABAJO, IZQUIERDA, DERECHA };
enum class EstadoJuego { JUGANDO, VICTORIA, DERROTA };

// --- CLASE PROYECTIL (LÁGRIMAS) ---
class Lagrima {
public:
    int x, y;
    Direccion dir;
    bool activa;

    Lagrima(int posX, int posY, Direccion d) : x(posX), y(posY), dir(d), activa(true) {}

    void Actualizar() {
        switch (dir) {
            case Direccion::ARRIBA:    y--; break;
            case Direccion::ABAJO:     y++; break;
            case Direccion::IZQUIERDA: x--; break;
            case Direccion::DERECHA:   x++; break;
            default: break;
        }
        if (x <= 0 || x >= ANCHO - 1 || y <= 0 || y >= ALTO - 1) {
            activa = false;
        }
    }
};

// --- CLASE ENTIDAD BASE ---
class Entidad {
protected:
    int x, y;
    int vida;
    int vidaMaxima;
public:
    Entidad(int posX, int posY, int v) : x(posX), y(posY), vida(v), vidaMaxima(v) {}
    virtual ~Entidad() = default;

    int GetX() const { return x; }
    int GetY() const { return y; }
    int GetVida() const { return vida; }
    int GetVidaMaxima() const { return vidaMaxima; }
    void RecibirDanio(int cantidad) { vida -= cantidad; if (vida < 0) vida = 0; }
    bool EstaVivo() const { return vida > 0; }
    void SetPosicion(int posX, int posY) { x = posX; y = posY; }
};

// --- CLASE JUGADOR (ISAAC) ---
class Jugador : public Entidad {
private:
    int danio;
public:
    Jugador(int posX, int posY) : Entidad(posX, posY, 6), danio(1) {} // 6 medios corazones = 3 corazones enteros

    int GetDanio() const { return danio; }
    void AumentarDanio(int cantidad) { danio += cantidad; }
    void Curar(int cantidad) { vida = std::min(vida + cantidad, vidaMaxima); }

    void Mover(int dx, int dy) {
        int nuevoX = x + dx;
        int nuevoY = y + dy;
        
        if (nuevoX > 0 && nuevoX < ANCHO - 1 && nuevoY > 0 && nuevoY < ALTO - 1) {
            x = nuevoX;
            y = nuevoY;
        } else {
            // Permitir avanzar si usa una puerta abierta en los bordes
            if ((nuevoX == 0 || nuevoX == ANCHO - 1) && nuevoY == ALTO / 2) { x = nuevoX; }
            if ((nuevoY == 0 || nuevoY == ALTO - 1) && nuevoX == ANCHO / 2) { y = nuevoY; }
        }
    }
};

// --- CLASE ENEMIGO BASE ---
class Enemigo : public Entidad {
protected:
    char simbolo;
public:
    Enemigo(int posX, int posY, int v, char sym) : Entidad(posX, posY, v), simbolo(sym) {}
    char GetSimbolo() const { return simbolo; }
    virtual void ActualizarIA(int jugadorX, int jugadorY, int frameCount) = 0;
};

// --- ENEMIGO NORMAL (GAPER) ---
class Gaper : public Enemigo {
public:
    Gaper(int posX, int posY) : Enemigo(posX, posY, 3, 'E') {}

    void ActualizarIA(int jugadorX, int jugadorY, int frameCount) override {
        if (frameCount % 4 != 0) return; // Movimiento lento
        if (x < jugadorX) x++; else if (x > jugadorX) x--;
        if (y < jugadorY) y++; else if (y > jugadorY) y--;
    }
};

// --- JEFE FINAL (MONSTRO) ---
class Monstro : public Enemigo {
private:
    int cooldownSalto;
public:
    Monstro(int posX, int posY) : Enemigo(posX, posY, 25, 'M'), cooldownSalto(0) {}

    void ActualizarIA(int jugadorX, int jugadorY, int frameCount) override {
        cooldownSalto++;
        // Monstro salta directo hacia ti cada ciertos frames
        if (cooldownSalto >= 15) {
            x = jugadorX + (rand() % 3 - 1); 
            y = jugadorY + (rand() % 3 - 1);
            
            if (x <= 0) x = 1; if (x >= ANCHO - 1) x = ANCHO - 2;
            if (y <= 0) y = 1; if (y >= ALTO - 1) y = ALTO - 2;
            
            cooldownSalto = 0;
        }
    }
};

// --- CLASE HABITACIÓN ---
class Habitacion {
public:
    int id;
    std::vector<std::unique_ptr<Enemigo>> enemigos;
    bool itemRecogido;
    std::string nombreItem;

    Habitacion(int identificador) : id(identificador), itemRecogido(false), nombreItem("") {
        if (id == 1) {
            enemigos.push_back(std::make_unique<Gaper>(5, 4));
            enemigos.push_back(std::make_unique<Gaper>(20, 10));
        } else if (id == 2) {
            nombreItem = "The Sad Onion (+Danio)";
            enemigos.push_back(std::make_unique<Gaper>(4, 4));
            enemigos.push_back(std::make_unique<Gaper>(20, 4));
            enemigos.push_back(std::make_unique<Gaper>(12, 11));
        } else if (id == 3) {
            nombreItem = "Breakfast (+Vida)";
            enemigos.push_back(std::make_unique<Gaper>(5, 7));
            enemigos.push_back(std::make_unique<Gaper>(19, 7));
        } else if (id == 4) {
            enemigos.push_back(std::make_unique<Monstro>(12, 5));
        }
    }

    bool EstaLimpia() const {
        return enemigos.empty();
    }
};

// --- MOTOR PRINCIPAL ---
class JuegoIsaac {
private:
    Jugador isaac;
    std::vector<Habitacion> mazmorra;
    size_t habitacionActual;
    std::vector<Lagrima> lagrimas;
    EstadoJuego estado;
    int frameCount;

    void ProcesarEntrada() {
        if (!_kbhit()) return;
        int ch = _getch();
        
        if (ch == 0 || ch == 224) { // Flechas para disparar
            switch (_getch()) {
                case 72: lagrimas.emplace_back(isaac.GetX(), isaac.GetY() - 1, Direccion::ARRIBA); break;
                case 80: lagrimas.emplace_back(isaac.GetX(), isaac.GetY() + 1, Direccion::ABAJO); break;
                case 75: lagrimas.emplace_back(isaac.GetX() - 1, isaac.GetY(), Direccion::IZQUIERDA); break;
                case 77: lagrimas.emplace_back(isaac.GetX() + 1, isaac.GetY(), Direccion::DERECHA); break;
            }
        } else { // WASD para moverse
            switch (ch) {
                case 'w': case 'W': isaac.Mover(0, -1); break;
                case 's': case 'S': isaac.Mover(0, 1); break;
                case 'a': case 'A': isaac.Mover(-1, 0); break;
                case 'd': case 'D': isaac.Mover(1, 0); break;
            }
        }
    }

    void ActualizarLogica() {
        frameCount++;
        Habitacion& sala = mazmorra[habitacionActual];

        // 1. Actualizar lágrimas
        for (auto& l : lagrimas) l.Actualizar();

        // 2. Lógica de enemigos
        for (auto& e : sala.enemigos) {
            e->ActualizarIA(isaac.GetX(), isaac.GetY(), frameCount);
            
            if (e->GetX() == isaac.GetX() && e->GetY() == isaac.GetY()) {
                isaac.RecibirDanio(1);
                if (!isaac.EstaVivo()) estado = EstadoJuego::DERROTA;
            }
        }

        // 3. Colisión Lágrimas -> Enemigos
        for (auto& l : lagrimas) {
            if (!l.activa) continue;
            for (auto& e : sala.enemigos) {
                if (e->EstaVivo() && l.x == e->GetX() && l.y == e->GetY()) {
                    e->RecibirDanio(isaac.GetDanio());
                    l.activa = false;
                    break;
                }
            }
        }

        // 4. Limpieza de elementos muertos
        lagrimas.erase(std::remove_if(lagrimas.begin(), lagrimas.end(), [](const Lagrima& l) { return !l.activa; }), lagrimas.end());
        sala.enemigos.erase(std::remove_if(sala.enemigos.begin(), sala.enemigos.end(), [](const std::unique_ptr<Enemigo>& e) { return !e->EstaVivo(); }), sala.enemigos.end());

        if (habitacionActual == 3 && sala.EstaLimpia()) {
            estado = EstadoJuego::VICTORIA;
        }

        // 5. Recoger Item
        if (sala.EstaLimpia() && !sala.nombreItem.empty() && !sala.itemRecogido) {
            if (isaac.GetX() == ANCHO / 2 && isaac.GetY() == ALTO / 2) {
                sala.itemRecogido = true;
                if (habitacionActual == 1) isaac.AumentarDanio(1); 
                if (habitacionActual == 2) isaac.Curar(2);       
            }
        }

        // 6. Cambio de habitación por puerta derecha
        if (sala.EstaLimpia()) {
            if (isaac.GetX() >= ANCHO - 1 && isaac.GetY() == ALTO / 2 && habitacionActual < 3) {
                habitacionActual++;
                isaac.SetPosicion(1, ALTO / 2); 
                lagrimas.clear();
            }
        }
    }

    void Renderizar() {
        std::system("cls");
        Habitacion& sala = mazmorra[habitacionActual];
        std::vector<std::string> mapa(ALTO, std::string(ANCHO, ' '));

        // Dibujar paredes y puertas
        for (int i = 0; i < ALTO; ++i) {
            for (int j = 0; j < ANCHO; ++j) {
                if (i == 0 || i == ALTO - 1 || j == 0 || j == ANCHO - 1) {
                    if (sala.EstaLimpia() && j == ANCHO - 1 && i == ALTO / 2 && habitacionActual < 3) {
                        mapa[i][j] = ' '; 
                    } else {
                        mapa[i][j] = '#';
                    }
                }
            }
        }

        if (sala.EstaLimpia() && !sala.nombreItem.empty() && !sala.itemRecogido) {
            mapa[ALTO / 2][ANCHO / 2] = '?';
        }

        for (const auto& l : lagrimas) if (l.activa) mapa[l.y][l.x] = 'o';
        for (const auto& e : sala.enemigos) mapa[e->GetY()][e->GetX()] = e->GetSimbolo();
        mapa[isaac.GetY()][isaac.GetX()] = 'I';

        for (const auto& linea : mapa) std::cout << linea << "\n";

        // UI
        std::cout << "\n=========================================\n";
        std::cout << " HABITACION: " << (habitacionActual + 1) << " / 4  ";
        if (habitacionActual == 3) std::cout << "[¡SALA DEL JEFE!]";
        std::cout << "\n VIDA ISAAC: ";
        for (int i = 0; i < isaac.GetVida(); i += 2) std::cout << "<3 ";
        std::cout << " (Danio: " << isaac.GetDanio() << ")\n";

        if (habitacionActual == 3 && !sala.enemigos.empty()) {
            std::cout << " HP MONSTRO: [";
            for (int i = 0; i < sala.enemigos[0]->GetVida(); ++i) std::cout << "|";
            std::cout << "]\n";
        }
        
        if (sala.EstaLimpia() && !sala.itemRecogido && !sala.nombreItem.empty()) {
            std::cout << "\n¡Item en el centro! Pasa sobre el '?' para recoger: " << sala.nombreItem << "\n";
        } else if (sala.EstaLimpia() && habitacionActual < 3) {
            std::cout << "\n¡Habitacion limpia! Cruza la puerta derecha ->\n";
        }
    }

public:
    JuegoIsaac() : isaac(3, ALTO / 2), habitacionActual(0), estado(EstadoJuego::JUGANDO), frameCount(0) {
        for (int i = 1; i <= 4; ++i) {
            mazmorra.emplace_back(i);
        }
    }

    void Ejecutar() {
        while (estado == EstadoJuego::JUGANDO) {
            Renderizar();
            ProcesarEntrada();
            ActualizarLogica();
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
        }

        std::system("cls");
        if (estado == EstadoJuego::VICTORIA) {
            std::cout << "=========================================\n";
            std::cout << "   ¡FELICIDADES! ¡DESTRUIDO CRACK!     \n";
            std::cout << "         ¡HAS GANADO! ¡SOLO MILLOS!      \n";
            std::cout << "=========================================\n";
        } else {
            std::cout << "=========================================\n";
            std::cout << "               GAME OVER                 \n";
            std::cout << "          Ducuara ha muerto...             \n";
            std::cout << "=========================================\n";
        }
    }
};

int main() {
    srand(static_cast<unsigned int>(time(nullptr)));
    JuegoIsaac juego;
    juego.Ejecutar();
    return 0;
}