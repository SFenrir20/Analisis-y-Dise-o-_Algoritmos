#include "arbol.h"
#include <iostream>
using namespace std;

int main() {
    ArbolRojoNegro arbol;
    arbol.Insertar(7);
    arbol.Insertar(3);
    arbol.Insertar(18);
    arbol.Insertar(10);
    arbol.Insertar(22);
    arbol.Insertar(8);
    arbol.Insertar(11);
    arbol.Insertar(26);

    arbol.Buscar(18);
    arbol.Eliminar(18);
    arbol.Buscar(18);

    return 0;
}