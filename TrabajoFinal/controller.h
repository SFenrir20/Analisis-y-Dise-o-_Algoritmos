#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <cstdlib>
#include <ctime>
#include "model.h"
#include "view.h"

using namespace std;

// Funciones auxiliares para generar datos aleatorios
string generarNombre();
string generarApellido();
string generarStringAleatorio(const string& prefix);
int generarDni();
string generarTelefono();
string generarEmail(const string& nombre, const string& apellido);
string generarEstadoCivil();

void generarCiudadanos(int numCiudadanos, CuckooHashTable& cuckooHash);
void cargarDNIsEnBTree(const string& nombreArchivo, CuckooHashTable& cuckooHash, BTree& btree);
void eliminarCiudadano(int dni, CuckooHashTable& cuckooHash, BTree& btree, const string& nombreArchivo);

#endif // CONTROLLER_H
