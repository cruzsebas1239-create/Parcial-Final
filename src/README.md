# AdventureCrawler - Dungeon Crawler 2D (Versión Extendida e Innovada)

Este es un juego de exploración, acción y combate en 2D basado en consola (caracteres ASCII). Está desarrollado en C++ utilizando los fundamentos de la Programación Orientada a Objetos (POO) y una gestión estricta de memoria estática (arreglos de tamaño fijo). El proyecto está fuertemente inspirado en el clásico *Adventure* (1980) de Atari.

---

## Controles del Juego

El juego responde de manera síncrona e instantánea al presionar las teclas. No es necesario presionar la tecla *Enter* para confirmar cada acción:

* **`W`**: Moverse hacia ARRIBA.
* **`S`**: Moverse hacia ABAJO.
* **`A`**: Moverse hacia la IZQUIERDA.
* **`D`**: Moverse hacia la DERECHA.
* **`F`**: **Disparo Láser Fulminante** (Dispara un rayo instantáneo en la última dirección en la que caminó el jugador).
* **`E`**: Recoger / Soltar el Cáliz Sagrado.
* **`Q`**: Salir del juego inmediatamente.

---

##  Elementos del Mapa (Leyenda)

* `#` : **Paredes / Muros** (Bloquean por completo el paso de jugadores, enemigos y proyectiles).
* `.` : **Suelo vacío** (Camino transitable y libre para el movimiento).
* `P` : **Jugador** (Tu personaje protagonista).
* `B` : **Murciélago (Bat)** (Enemigo común ágil, se mueve a velocidad reducida).
* `D` : **Dragón** (Enemigo pesado de persecución que custodia zonas clave).
* `J` : **Jefe Final (Final Boss)** (Monstruo de gran tamaño situado en la Habitación 5. Posee múltiples vidas y habilidades de teletransportación).
* `C` : **Cáliz (Chalice)** (El objeto sagrado y tesoro principal que debes robar).

---

##  Objetivo y Condición de Victoria

Para completar el juego con éxito, se debe ejecutar una estrategia completa de incursión, combate y escape:

1.  **Infiltración:** Cruza las habitaciones de izquierda a derecha (de la Habitación 0 a la 5) abriéndote paso entre los secuaces.
2.  **Duelo de Jefe:** En la Habitación 5, enfréntate al Jefe Final (`J`). Debes acertarle 3 disparos con la tecla `F` para erradicarlo.
3.  **Asegurar el Botín:** Camina exactamente sobre la casilla del Cáliz (`C`) y presiona la tecla `E`. El indicador de inventario en pantalla pasará a registrar: `[Inventario: CALIZ (1/1)]`.
4.  **La Retirada:** Emprende el viaje de regreso cruzando todas las habitaciones hacia la izquierda hasta retornar de forma segura a la **Habitación 0**.
5.  **Extracción Exitosa (Victoria):** Dirígete a la esquina superior izquierda de la Habitación 0 (**Coordenadas exactas X=1, Y=1**). Al pisar esa casilla llevando el Cáliz contigo, el juego finalizará desplegando el letrero de ¡VICTORIA ÉPICA!.

*Nota de Derrota: Si cualquier enemigo activo (incluyendo al jefe) intersecta tu misma coordenada antes de que logres escapar, el juego terminará inmediatamente en Game Over.*

---

## Innovaciones y Mejoras de esta Versión

Con respecto a los requerimientos base tradicionales de este laboratorio, se integraron soluciones técnicas avanzadas para enriquecer la jugabilidad y demostrar el dominio del lenguaje C++:

1. **Entrada de Teclado No Bloqueante (`<conio.h>`)**: 
   Sustitución del uso restrictivo de `std::cin` por la función nativa `_getch()`. Esto permite que el personaje avance continuamente y de forma fluida mientras el usuario presiona las teclas de dirección, eliminando la interrupción de presionar *Enter*.
   
2. **Refresco Dinámico de Consola (`system("cls")`)**:
   Implementación de limpieza de búfer en cada ciclo del *Game Loop*. Esto renderiza el mapa estáticamente en la terminal simulando el comportamiento de un motor de videojuegos 2D real, en lugar de imprimir líneas infinitas hacia abajo.

3. **Mecánica de Raycast para Disparo Instantáneo**:
   Desarrollo de un sistema de proyectiles que calcula en tiempo real un vector en línea recta basado en la última dirección registrada del jugador. El disparo viaja e impacta instantáneamente a los enemigos en su trayectoria, deteniéndose de manera lógica si colisiona con un muro `#`.

4. **Jefe Final de Múltiples Fases (`FINAL_BOSS`)**:
   Inclusión de un enemigo mayor con un atributo de salud (`health = 3`). El código detecta los impactos del proyectil, descuenta vida y gatilla una mecánica defensiva de teletransportación a posiciones aleatorias o alternas dentro de su sala para evitar ser eliminado fácilmente.

5. **Modulador de Ticks de Velocidad (Dificultad Balanceada)**:
   A causa del movimiento continuo del jugador, los enemigos podían volverse letales. Se resolvió creando un algoritmo de control de pasos (`playerSteps % 2`) que restringe la actualización de la IA enemiga a la mitad de la velocidad del jugador, logrando un balance de dificultad óptimo y estratégico.

6. **Transición Fluida de Portales Horizontales**:
   Optimización matemática de los límites perimetrales del escenario. Al