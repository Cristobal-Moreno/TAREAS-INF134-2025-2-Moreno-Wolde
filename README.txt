# Tarea 2 - Balatro

Nombres: 
- Cristobal Moreno 

Rol: 202473591-8 

Pararelo: 200
- Mateo Wolde

Rol: 202404546-6

Paralelo: 201

## Características Técnicas
- **Sistema Operativo**: Linux
- **Compilador**: g++
- **Versión de C++**: C++11
- **Compilación**: Usar `make` para compilar

## Características de Funcionamiento
- El juego implementa las 3 ciegas: Pequeña (250), Grande (350) y Jefe (500)
- Modificador implementado: **La serpiente** (siempre se reparten 3 cartas extra después de jugar/descartar)
- Estructuras implementadas:
  - 4 árboles binarios de búsqueda (uno por pinta)
  - Lista enlazada para mazo barajado
  - Lista enlazada para mano del jugador

## Compilación y Ejecución
```bash
make        # Compilar
make run    # Compilar y ejecutar
make clean  # Limpiar archivos compilados