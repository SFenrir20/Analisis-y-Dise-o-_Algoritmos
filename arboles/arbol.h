#ifndef ARBOL_H
#define ARBOL_H

#include "nodo.h"
#include <iostream>

using namespace std;
class ArbolRojoNegro {
private:
    Nodo *raiz;

    void RotarIzquierda(Nodo *&, Nodo *&);
    void RotarDerecha(Nodo *&, Nodo *&);
    void Ajustar(Nodo *&, Nodo *&);
    void InsertarNodo(Nodo *&, Nodo *&);
    void Transplantar(Nodo *&, Nodo *, Nodo *);
    Nodo* Minimo(Nodo *);
    void EliminarNodo(Nodo *&, Nodo *&);
    void AjustarEliminacion(Nodo *&, Nodo *&);

public:
    ArbolRojoNegro();
    void Insertar(int);
    void Eliminar(int);
    void Buscar(int);
};

void ArbolRojoNegro::RotarIzquierda(Nodo *&raiz, Nodo *&pt) {
    Nodo *pt_derecha = pt->derecha;
    pt->derecha = pt_derecha->izquierda;

    if (pt->derecha != nullptr)
        pt->derecha->padre = pt;

    pt_derecha->padre = pt->padre;

    if (pt->padre == nullptr)
        raiz = pt_derecha;
    else if (pt == pt->padre->izquierda)
        pt->padre->izquierda = pt_derecha;
    else
        pt->padre->derecha = pt_derecha;

    pt_derecha->izquierda = pt;
    pt->padre = pt_derecha;
}

void ArbolRojoNegro::RotarDerecha(Nodo *&raiz, Nodo *&pt) {
    Nodo *pt_izquierda = pt->izquierda;
    pt->izquierda = pt_izquierda->derecha;

    if (pt->izquierda != nullptr)
        pt->izquierda->padre = pt;

    pt_izquierda->padre = pt->padre;

    if (pt->padre == nullptr)
        raiz = pt_izquierda;
    else if (pt == pt->padre->izquierda)
        pt->padre->izquierda = pt_izquierda;
    else
        pt->padre->derecha = pt_izquierda;

    pt_izquierda->derecha = pt;
    pt->padre = pt_izquierda;
}

void ArbolRojoNegro::Ajustar(Nodo *&raiz, Nodo *&pt) {
    Nodo *padre_pt = nullptr;
    Nodo *abuelo_pt = nullptr;

    while ((pt != raiz) && (pt->color != NEGRO) &&
           (pt->padre->color == ROJO)) {

        padre_pt = pt->padre;
        abuelo_pt = pt->padre->padre;

        if (padre_pt == abuelo_pt->izquierda) {

            Nodo *tio_pt = abuelo_pt->derecha;

            if (tio_pt != nullptr && tio_pt->color == ROJO) {
                abuelo_pt->color = ROJO;
                padre_pt->color = NEGRO;
                tio_pt->color = NEGRO;
                pt = abuelo_pt;
            } else {
                if (pt == padre_pt->derecha) {
                    RotarIzquierda(raiz, padre_pt);
                    pt = padre_pt;
                    padre_pt = pt->padre;
                }

                RotarDerecha(raiz, abuelo_pt);
                swap(padre_pt->color, abuelo_pt->color);
                pt = padre_pt;
            }
        } else {
            Nodo *tio_pt = abuelo_pt->izquierda;

            if ((tio_pt != nullptr) && (tio_pt->color == ROJO)) {
                abuelo_pt->color = ROJO;
                padre_pt->color = NEGRO;
                tio_pt->color = NEGRO;
                pt = abuelo_pt;
            } else {
                if (pt == padre_pt->izquierda) {
                    RotarDerecha(raiz, padre_pt);
                    pt = padre_pt;
                    padre_pt = pt->padre;
                }

                RotarIzquierda(raiz, abuelo_pt);
                swap(padre_pt->color, abuelo_pt->color);
                pt = padre_pt;
            }
        }
    }

    raiz->color = NEGRO;
}

void ArbolRojoNegro::InsertarNodo(Nodo *&raiz, Nodo *&pt) {
    if (raiz == nullptr) {
        raiz = pt;
    } else if (pt->valor < raiz->valor) {
        pt->padre = raiz;
        InsertarNodo(raiz->izquierda, pt);
    } else if (pt->valor > raiz->valor) {
        pt->padre = raiz;
        InsertarNodo(raiz->derecha, pt);
    }
}

void ArbolRojoNegro::Insertar(int valor) {
    Nodo *pt = new Nodo(valor);
    InsertarNodo(raiz, pt);
    Ajustar(raiz, pt);
}

Nodo* ArbolRojoNegro::Minimo(Nodo *nodo) {
    while (nodo->izquierda != nullptr)
        nodo = nodo->izquierda;
    return nodo;
}

void ArbolRojoNegro::Transplantar(Nodo *&raiz, Nodo *u, Nodo *v) {
    if (u->padre == nullptr)
        raiz = v;
    else if (u == u->padre->izquierda)
        u->padre->izquierda = v;
    else
        u->padre->derecha = v;

    if (v != nullptr)
        v->padre = u->padre;
}

void ArbolRojoNegro::EliminarNodo(Nodo *&raiz, Nodo *&nodo) {
    Nodo *y = nodo;
    Nodo *x;
    Color color_original = y->color;

    if (nodo->izquierda == nullptr) {
        x = nodo->derecha;
        Transplantar(raiz, nodo, nodo->derecha);
    } else if (nodo->derecha == nullptr) {
        x = nodo->izquierda;
        Transplantar(raiz, nodo, nodo->izquierda);
    } else {
        y = Minimo(nodo->derecha);
        color_original = y->color;
        x = y->derecha;

        if (y->padre == nodo)
            x->padre = y;
        else {
            Transplantar(raiz, y, y->derecha);
            y->derecha = nodo->derecha;
            y->derecha->padre = y;
        }

        Transplantar(raiz, nodo, y);
        y->izquierda = nodo->izquierda;
        y->izquierda->padre = y;
        y->color = nodo->color;
    }

    delete nodo;

    if (color_original == NEGRO)
        AjustarEliminacion(raiz, x);
}

void ArbolRojoNegro::Eliminar(int valor) {
    Nodo *nodo = raiz;
    Nodo *z = nullptr;

    while (nodo != nullptr) {
        if (nodo->valor == valor) {
            z = nodo;
            break;
        }
        if (nodo->valor <= valor)
            nodo = nodo->derecha;
        else
            nodo = nodo->izquierda;
    }

    if (z == nullptr) {
        cout << "No se encontró el valor en el árbol.\n";
        return;
    }

    EliminarNodo(raiz, z);
}

void ArbolRojoNegro::AjustarEliminacion(Nodo *&raiz, Nodo *&x) {
    while (x != raiz && x->color == NEGRO) {
        if (x == x->padre->izquierda) {
            Nodo *w = x->padre->derecha;

            if (w->color == ROJO) {
                w->color = NEGRO;
                x->padre->color = ROJO;
                RotarIzquierda(raiz, x->padre);
                w = x->padre->derecha;
            }

            if (w->izquierda->color == NEGRO && w->derecha->color == NEGRO) {
                w->color = ROJO;
                x = x->padre;
            } else {
                if (w->derecha->color == NEGRO) {
                    w->izquierda->color = NEGRO;
                    w->color = ROJO;
                    RotarDerecha(raiz, w);
                    w = x->padre->derecha;
                }

                w->color = x->padre->color;
                x->padre->color = NEGRO;
                w->derecha->color = NEGRO;
                RotarIzquierda(raiz, x->padre);
                x = raiz;
            }
        } else {
            Nodo *w = x->padre->izquierda;

            if (w->color == ROJO) {
                w->color = NEGRO;
                x->padre->color = ROJO;
                RotarDerecha(raiz, x->padre);
                w = x->padre->izquierda;
            }

            if (w->izquierda->color == NEGRO && w->derecha->color == NEGRO) {
                w->color = ROJO;
                x = x->padre;
            } else {
                if (w->izquierda->color == NEGRO) {
                    w->derecha->color = NEGRO;
                    w->color = ROJO;
                    RotarIzquierda(raiz, w);
                    w = x->padre->izquierda;
                }

                w->color = x->padre->color;
                x->padre->color = NEGRO;
                w->izquierda->color = NEGRO;
                RotarDerecha(raiz, x->padre);
                x = raiz;
            }
        }
    }
    x->color = NEGRO;
}

void ArbolRojoNegro::Buscar(int valor) {
    Nodo *nodo = raiz;
    while (nodo != nullptr) {
        if (nodo->valor == valor) {
            cout << "Valor encontrado en el árbol.\n";
            return;
        }
        if (nodo->valor < valor)
            nodo = nodo->derecha;
        else
            nodo = nodo->izquierda;
    }
    cout << "Valor no encontrado en el árbol.\n";
}

#endif // ARBOL_H
