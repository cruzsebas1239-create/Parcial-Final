# Parcial - Dungeon Crawler (Inspirado en Adventure 1980)

## Integrantes
* Diego Sebastián Cruz Suarez
* Horus De James Reyes Martinez
## Desarrollos Innovadores Implementados
* **Comportamiento Troll del Murciélago (Bat AI):** El enemigo no te mata, sino que roba e interactúa con el inventario, generando situaciones impredecibles (detallado en el README interno del proyecto).

---

## Instrucciones de Compilación (Multiplataforma)

Este proyecto utiliza CMake para asegurar la compatibilidad en distintos sistemas operativos. Se requiere tener instalado un compilador de C++ (GCC/Clang/MSVC) y CMake.

### Para Windows:
1. Abrir la consola (PowerShell o CMD) en la carpeta `AdventureProject`.
2. Crear carpeta de construcción: `mkdir build` y luego `cd build`
3. Preparar CMake: `cmake ..`
4. Compilar: `cmake --build .`
5. Ejecutar: `Debug\AdventureCrawler.exe` (o `AdventureCrawler.exe` según el generador).

### Para Linux / Mac:
1. Abrir la terminal en la carpeta `AdventureProject`.
2. Crear carpeta de construcción: `mkdir build && cd build`
3. Preparar CMake: `cmake ..`
4. Compilar: `make` (o `cmake --build .`)
5. Ejecutar: `./AdventureCrawler`
