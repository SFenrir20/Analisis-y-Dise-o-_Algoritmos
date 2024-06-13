#ifndef NODO_H
#define NODO_H

#include <iostream>
using namespace std;

enum Color { ROJO, NEGRO };

struct Nodo {
    int valor;
    Color color;
    Nodo *izquierda, *derecha, *padre;

    Nodo(int val) : valor(val), color(ROJO), izquierda(nullptr), derecha(nullptr), padre(nullptr) {}
};

#endif // NODO_H