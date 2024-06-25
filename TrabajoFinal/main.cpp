#include "model.h"
#include "view.h"
#include "controller.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>
#include <filesystem>
#include <algorithm> // Incluye el encabezado para std::find_if
#include <map>
using namespace std;
namespace fs = filesystem;

vector<string> leerArchivo(const string& nombreArchivo) {
    vector<string> lineas;
    ifstream archivo(nombreArchivo);
    if (archivo.is_open()) {
        string linea;
        while (getline(archivo, linea)) {
            lineas.push_back(linea);
        }
        archivo.close();
    } else {
        cerr << "No se pudo abrir el archivo: " << nombreArchivo << "\nRuta absoluta: " << fs::absolute(nombreArchivo) << endl;
    }
    return lineas;
}

vector<vector<string>> leerArchivoEstructurado(const string& nombreArchivo) {
    vector<vector<string>> datos;
    ifstream archivo(nombreArchivo);
    if (archivo.is_open()) {
        string linea;
        while (getline(archivo, linea)) {
            vector<string> partes;
            size_t inicio = 0, fin;
            while ((fin = linea.find(',', inicio)) != string::npos) {
                partes.push_back(linea.substr(inicio, fin - inicio));
                inicio = fin + 1;
            }
            partes.push_back(linea.substr(inicio));
            datos.push_back(partes);
        }
        archivo.close();
    } else {
        cerr << "No se pudo abrir el archivo: " << nombreArchivo << "\nRuta absoluta: " << fs::absolute(nombreArchivo) << endl;
    }
    return datos;
}

// Función para eliminar espacios en blanco al inicio y al final
string trim(const string& str) {
    size_t start = str.find_first_not_of(" \t\n\r\f\v");
    size_t end = str.find_last_not_of(" \t\n\r\f\v");
    return (start == string::npos) ? "" : str.substr(start, end - start + 1);
}

// Implementación de la clase Ciudadano
Ciudadano::Ciudadano(int dni, const string& nombres, const string& apellidos, const string& nacionalidad,
                     const string& lugar_de_nacimiento, const string& departamento, const string& provincia,
                     const string& ciudad, const string& distrito, const string& ubicacion,
                     const string& telefono_de_referencia, const string& correo_electronico, const string& estado_civil)
    : dni(dni), nombres(nombres), apellidos(apellidos), nacionalidad(nacionalidad), lugar_de_nacimiento(lugar_de_nacimiento),
      departamento(departamento), provincia(provincia), ciudad(ciudad), distrito(distrito), ubicacion(ubicacion),
      telefono_de_referencia(telefono_de_referencia), correo_electronico(correo_electronico), estado_civil(estado_civil) {}

int Ciudadano::getDni() const { return dni; }
string Ciudadano::getNombres() const { return nombres; }
string Ciudadano::getApellidos() const { return apellidos; }
string Ciudadano::getNacionalidad() const { return nacionalidad; }
string Ciudadano::getLugarDeNacimiento() const { return lugar_de_nacimiento; }
string Ciudadano::getDepartamento() const { return departamento; }
string Ciudadano::getProvincia() const { return provincia; }
string Ciudadano::getCiudad() const { return ciudad; }
string Ciudadano::getDistrito() const { return distrito; }
string Ciudadano::getUbicacion() const { return ubicacion; }
string Ciudadano::getTelefonoDeReferencia() const { return telefono_de_referencia; }
string Ciudadano::getCorreoElectronico() const { return correo_electronico; }
string Ciudadano::getEstadoCivil() const { return estado_civil; }

void Ciudadano::leerDesdeArchivo(ifstream& archivo) {
    string linea;
    map<string, string> campos;
    vector<string> camposEsperados = {
        "DNI: ", "Nombres: ", "Apellidos: ", "Nacionalidad: ",
        "Lugar de Nacimiento: ", "Departamento: ", "Provincia: ",
        "Ciudad: ", "Distrito: ", "Ubicación: ", "Teléfono de Referencia: ",
        "Correo Electrónico: ", "Estado Civil: "
    };

    while (getline(archivo, linea)) {
        for (const auto& campo : camposEsperados) {
            if (linea.rfind(campo, 0) == 0) {
                campos[campo] = linea.substr(campo.length());
                break;
            }
        }
        if (linea.find(';') != string::npos) {
            break; // Final del registro
        }
    }

    try {
        if (campos.find("DNI: ") != campos.end()) {
            dni = stoi(campos["DNI: "]);
        } else {
            throw runtime_error("DNI no encontrado");
        }

        nombres = trim(campos["Nombres: "]);
        apellidos = trim(campos["Apellidos: "]);
        nacionalidad = trim(campos["Nacionalidad: "]);
        lugar_de_nacimiento = trim(campos["Lugar de Nacimiento: "]);
        departamento = trim(campos["Departamento: "]);
        provincia = trim(campos["Provincia: "]);
        ciudad = trim(campos["Ciudad: "]);
        distrito = trim(campos["Distrito: "]);
        ubicacion = trim(campos["Ubicación: "]);
        telefono_de_referencia = trim(campos["Teléfono de Referencia: "]);
        correo_electronico = trim(campos["Correo Electrónico: "]);
        estado_civil = trim(campos["Estado Civil: "]);

        // Eliminar la coma o punto y coma al final de cada campo si existe
        for (auto& [key, value] : campos) {
            if (!value.empty() && (value.back() == ',' || value.back() == ';')) {
                value.pop_back();
            }
        }

    } catch (const exception& e) {
        cerr << "Error al leer datos del ciudadano: " << e.what() << endl;
        for (const auto& [key, value] : campos) {
            cerr << key << value << endl;
        }
        throw;
    }
}

// Implementación de CuckooHashTable
CuckooHashTable::CuckooHashTable(int size) : size(size), table1(size), table2(size) {}

int CuckooHashTable::hash1(int key) const {
    return key % size;
}

int CuckooHashTable::hash2(int key) const {
    return (key / size) % size;
}

bool CuckooHashTable::insert(int key, long position) {
    int pos1 = hash1(key);
    if (!table1[pos1].occupied) {
        table1[pos1].key = key;
        table1[pos1].position = position;
        table1[pos1].occupied = true;
        return true;
    }

    int pos2 = hash2(key);
    if (!table2[pos2].occupied) {
        table2[pos2].key = key;
        table2[pos2].position = position;
        table2[pos2].occupied = true;
        return true;
    }

    // Si ambas posiciones están ocupadas, necesitamos reubicar
    return false; // Asumir fallo por simplicidad
}

bool CuckooHashTable::remove(int key) {
    int pos1 = hash1(key);
    if (table1[pos1].occupied && table1[pos1].key == key) {
        table1[pos1].occupied = false;
        return true;
    }

    int pos2 = hash2(key);
    if (table2[pos2].occupied && table2[pos2].key == key) {
        table2[pos2].occupied = false;
        return true;
    }

    return false; // No encontrado
}

long CuckooHashTable::search(int key) const {
    int pos1 = hash1(key);
    if (table1[pos1].occupied && table1[pos1].key == key) {
        return table1[pos1].position;
    }

    int pos2 = hash2(key);
    if (table2[pos2].occupied && table2[pos2].key == key) {
        return table2[pos2].position;
    }

    throw std::runtime_error("Clave no encontrada");
}

void CuckooHashTable::save(const string& filename) const {
    ofstream file(filename, ios::binary);
    if (!file.is_open()) {
        throw runtime_error("No se puede abrir el archivo para guardar");
    }
    file.write(reinterpret_cast<const char*>(&size), sizeof(size));
    file.write(reinterpret_cast<const char*>(table1.data()), sizeof(CuckooHashEntry) * size);
    file.write(reinterpret_cast<const char*>(table2.data()), sizeof(CuckooHashEntry) * size);
    file.close();
}

void CuckooHashTable::load(const string& filename) {
    ifstream file(filename, ios::binary);
    if (!file.is_open()) {
        throw runtime_error("No se puede abrir el archivo para cargar");
    }
    file.read(reinterpret_cast<char*>(&size), sizeof(size));
    table1.resize(size);
    table2.resize(size);
    file.read(reinterpret_cast<char*>(table1.data()), sizeof(CuckooHashEntry) * size);
    file.read(reinterpret_cast<char*>(table2.data()), sizeof(CuckooHashEntry) * size);
    file.close();
}

void CuckooHashTable::buscarCiudadanoPorDni(int dni, const string& nombreArchivo) const {
    try {
        long posicion = search(dni);
        ifstream archivo(nombreArchivo);
        if (archivo.is_open()) {
            archivo.seekg(posicion);
            Ciudadano ciudadano;
            ciudadano.leerDesdeArchivo(archivo);
            mostrarCiudadano(ciudadano);
            archivo.close();
        } else {
            cerr << "No se pudo abrir el archivo para lectura." << endl;
        }
    } catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
    }
}

// Implementación de BTreeNode y BTree
BTreeNode::BTreeNode(int t, bool leaf) : t(t), leaf(leaf) {
    keys.reserve(2 * t - 1);
    children.reserve(2 * t);
}

void BTreeNode::traverse() const {
    for (int i = 0; i < keys.size(); i++) {
        if (!leaf) {
            children[i]->traverse();
        }
        cout << " " << keys[i];
    }
    if (!leaf) {
        children[keys.size()]->traverse();
    }
}

BTreeNode* BTreeNode::search(int k) {
    int i = 0;
    while (i < keys.size() && k > keys[i]) {
        i++;
    }
    if (i < keys.size() && keys[i] == k) {
        return this;
    }
    if (leaf) {
        return nullptr;
    }
    return children[i]->search(k);
}

void BTreeNode::insertNonFull(int k) {
    int i = keys.size() - 1;
    if (leaf) {
        keys.push_back(0);
        while (i >= 0 && keys[i] > k) {
            keys[i + 1] = keys[i];
            i--;
        }
        keys[i + 1] = k;
    } else {
        while (i >= 0 && keys[i] > k) {
            i--;
        }
        if (children[i + 1]->keys.size() == 2 * t - 1) {
            splitChild(i + 1, children[i + 1]);
            if (keys[i + 1] < k) {
                i++;
            }
        }
        children[i + 1]->insertNonFull(k);
    }
}

void BTreeNode::splitChild(int i, BTreeNode* y) {
    BTreeNode* z = new BTreeNode(y->t, y->leaf);
    z->keys.resize(t - 1);
    for (int j = 0; j < t - 1; j++) {
        z->keys[j] = y->keys[j + t];
    }
    if (!y->leaf) {
        z->children.resize(t);
        for (int j = 0; j < t; j++) {
            z->children[j] = y->children[j + t];
        }
    }
    y->keys.resize(t - 1);
    children.insert(children.begin() + i + 1, z);
    keys.insert(keys.begin() + i, y->keys[t - 1]);
}

void BTreeNode::remove(int k) {
    int idx = findKey(k);
    if (idx < keys.size() && keys[idx] == k) {
        if (leaf) {
            removeFromLeaf(idx);
        } else {
            removeFromNonLeaf(idx);
        }
    } else {
        if (leaf) {
            cerr << "El valor " << k << " no existe en el árbol\n";
            return;
        }
        bool flag = (idx == keys.size());
        if (children[idx]->keys.size() < t) {
            fill(idx);
        }
        if (flag && idx > keys.size()) {
            children[idx - 1]->remove(k);
        } else {
            children[idx]->remove(k);
        }
    }
}

void BTreeNode::removeFromLeaf(int idx) {
    keys.erase(keys.begin() + idx);
}

void BTreeNode::removeFromNonLeaf(int idx) {
    int k = keys[idx];
    if (children[idx]->keys.size() >= t) {
        int pred = getPred(idx);
        keys[idx] = pred;
        children[idx]->remove(pred);
    } else if (children[idx + 1]->keys.size() >= t) {
        int succ = getSucc(idx);
        keys[idx] = succ;
        children[idx + 1]->remove(succ);
    } else {
        merge(idx);
        children[idx]->remove(k);
    }
}

int BTreeNode::getPred(int idx) {
    BTreeNode* cur = children[idx];
    while (!cur->leaf) {
        cur = cur->children[cur->keys.size()];
    }
    return cur->keys[cur->keys.size() - 1];
}

int BTreeNode::getSucc(int idx) {
    BTreeNode* cur = children[idx + 1];
    while (!cur->leaf) {
        cur = cur->children[0];
    }
    return cur->keys[0];
}

void BTreeNode::fill(int idx) {
    if (idx != 0 && children[idx - 1]->keys.size() >= t) {
        borrowFromPrev(idx);
    } else if (idx != keys.size() && children[idx + 1]->keys.size() >= t) {
        borrowFromNext(idx);
    } else {
        if (idx != keys.size()) {
            merge(idx);
        } else {
            merge(idx - 1);
        }
    }
}

void BTreeNode::borrowFromPrev(int idx) {
    BTreeNode* child = children[idx];
    BTreeNode* sibling = children[idx - 1];
    for (int i = child->keys.size() - 1; i >= 0; --i) {
        child->keys[i + 1] = child->keys[i];
    }
    if (!child->leaf) {
        for (int i = child->keys.size(); i >= 0; --i) {
            child->children[i + 1] = child->children[i];
        }
    }
    child->keys[0] = keys[idx - 1];
    if (!leaf) {
        child->children[0] = sibling->children[sibling->keys.size()];
    }
    keys[idx - 1] = sibling->keys[sibling->keys.size() - 1];
    sibling->keys.resize(sibling->keys.size() - 1);
}

void BTreeNode::borrowFromNext(int idx) {
    BTreeNode* child = children[idx];
    BTreeNode* sibling = children[idx + 1];
    child->keys.push_back(keys[idx]);
    if (!(child->leaf)) {
        child->children[child->keys.size()] = sibling->children[0];
    }
    keys[idx] = sibling->keys[0];
    sibling->keys.erase(sibling->keys.begin());
    if (!sibling->leaf) {
        sibling->children.erase(sibling->children.begin());
    }
}

void BTreeNode::merge(int idx) {
    BTreeNode* child = children[idx];
    BTreeNode* sibling = children[idx + 1];
    child->keys.push_back(keys[idx]);
    for (int i = 0; i < sibling->keys.size(); ++i) {
        child->keys.push_back(sibling->keys[i]);
    }
    if (!child->leaf) {
        for (int i = 0; i <= sibling->keys.size(); ++i) {
            child->children.push_back(sibling->children[i]);
        }
    }
    keys.erase(keys.begin() + idx);
    children.erase(children.begin() + idx + 1);
    delete sibling;
}

int BTreeNode::findKey(int k) {
    int idx = 0;
    while (idx < keys.size() && keys[idx] < k) {
        ++idx;
    }
    return idx;
}

BTree::BTree(int t) : t(t) {
    root = new BTreeNode(t, true);
}

void BTree::traverse() const {
    if (root) {
        root->traverse();
    }
}

BTreeNode* BTree::search(int k) {
    return root ? root->search(k) : nullptr;
}

void BTree::insert(int k) {
    if (root->keys.size() == 2 * t - 1) {
        BTreeNode* s = new BTreeNode(t, false);
        s->children.push_back(root);
        s->splitChild(0, root);
        root = s;
        root->insertNonFull(k);
    } else {
        root->insertNonFull(k);
    }
}

void BTree::remove(int k) {
    if (!root) {
        cerr << "El árbol está vacío\n";
        return;
    }
    root->remove(k);
    if (root->keys.size() == 0) {
        BTreeNode* tmp = root;
        if (root->leaf) {
            root = nullptr;
        } else {
            root = root->children[0];
        }
        delete tmp;
    }
}

// Implementación de las funciones de vista
void mostrarCiudadano(const Ciudadano& ciudadano) {
    cout << "DNI: " << ciudadano.getDni() << "\n"
              << "Nombres: " << ciudadano.getNombres() << "\n"
              << "Apellidos: " << ciudadano.getApellidos() << "\n"
              << "Nacionalidad: " << ciudadano.getNacionalidad() << "\n"
              << "Lugar de Nacimiento: " << ciudadano.getLugarDeNacimiento() << "\n"
              << "Departamento: " << ciudadano.getDepartamento() << "\n"
              << "Provincia: " << ciudadano.getProvincia() << "\n"
              << "Ciudad: " << ciudadano.getCiudad() << "\n"
              << "Distrito: " << ciudadano.getDistrito() << "\n"
              << "Ubicación: " << ciudadano.getUbicacion() << "\n"
              << "Teléfono de Referencia: " << ciudadano.getTelefonoDeReferencia() << "\n"
              << "Correo Electrónico: " << ciudadano.getCorreoElectronico() << "\n"
              << "Estado Civil: " << ciudadano.getEstadoCivil() << "\n\n";
}

void mostrarBTree(const BTree& btree) {
    cout << "B-Tree contenido (DNIs): ";
    btree.traverse();
    cout << std::endl;
}

void mostrarPosiciones(const CuckooHashTable& cuckooHash) {
    // Mostrar las posiciones almacenadas en el Cuckoo Hash Table
    // Implementar si es necesario para depuración
}

// Implementación de funciones auxiliares y del controlador
string generarNombre() {
    const string nombres[] = {"Juan", "Pedro", "Luis", "Carlos", "Ana", "Maria", "Lucia", "Sofia"};
    return nombres[rand() % 8];
}

string generarApellido() {
    const string apellidos[] = {"Perez", "Gomez", "Rodriguez", "Lopez", "Martinez", "Garcia", "Sanchez", "Ramirez"};
    return apellidos[rand() % 8];
}

string generarStringAleatorio(const string& prefix) {
    return prefix + to_string(rand() % 1000);
}

int generarDni() {
    return 10000000 + rand() % 99999999;
}

string generarTelefono() {
    return "+51 9" + to_string(100000000 + rand() % 900000000);
}

string generarEmail(const string& nombre, const string& apellido) {
    return nombre + "." + apellido + "@example.com";
}

string generarEstadoCivil() {
    const string estados[] = {"Soltero", "Casado", "Divorciado", "Viudo"};
    return estados[rand() % 4];
}

void generarCiudadanos(int numCiudadanos, CuckooHashTable& cuckooHash) {
    srand(time(0)); // Inicializar la semilla para la generación aleatoria

    vector<string> nombres = leerArchivo("../DATA/names_list.txt");
    vector<string> apellidos = leerArchivo("../DATA/surnames_list.txt");
    vector<vector<string>> direcciones = leerArchivoEstructurado("../DATA/lima_full_structured_data.txt");

    if (nombres.empty() || apellidos.empty() || direcciones.empty()) {
        cerr << "Error: No se pudieron cargar algunos de los archivos necesarios." << endl;
        return;
    }

    vector<Ciudadano> ciudadanos;

    // Generar ciudadanos aleatorios
    ofstream archivo("ciudadanos.txt", ios::app); // Abrir el archivo en modo adjuntar
    if (archivo.is_open()) {
        for (int i = 0; i < numCiudadanos; ++i) {
            string nombre = nombres[std::rand() % nombres.size()];
            string apellido = apellidos[std::rand() % apellidos.size()];
            vector<string> direccion = direcciones[rand() % direcciones.size()];

            string ubicacionPrefix[] = {"Calle", "Av.", "Jirón"};
            string ubicacion = ubicacionPrefix[std::rand() % 3] + " " + to_string(rand() % 1000);

            Ciudadano ciudadano(generarDni(), nombre, apellido, "Peruana", direccion[2], direccion[0], direccion[1],
                                direccion[2], direccion[3], ubicacion, generarTelefono(),
                                generarEmail(nombre, apellido), generarEstadoCivil());
            ciudadanos.push_back(ciudadano);

            // Guardar la posición del archivo en el Cuckoo Hash
            long pos = archivo.tellp();
            cuckooHash.insert(ciudadano.getDni(), pos);

            // Escribir ciudadano en el archivo
            archivo << "DNI: " << ciudadano.getDni() << ",\n"
                    << "Nombres: " << ciudadano.getNombres() << ",\n"
                    << "Apellidos: " << ciudadano.getApellidos() << ",\n"
                    << "Nacionalidad: " << ciudadano.getNacionalidad() << ",\n"
                    << "Lugar de Nacimiento: " << ciudadano.getLugarDeNacimiento() << ",\n"
                    << "Departamento: " << ciudadano.getDepartamento() << ",\n"
                    << "Provincia: " << ciudadano.getProvincia() << ",\n"
                    << "Ciudad: " << ciudadano.getCiudad() << ",\n"
                    << "Distrito: " << ciudadano.getDistrito() << ",\n"
                    << "Ubicación: " << ciudadano.getUbicacion() << ",\n"
                    << "Teléfono de Referencia: " << ciudadano.getTelefonoDeReferencia() << ",\n"
                    << "Correo Electrónico: " << ciudadano.getCorreoElectronico() << ",\n"
                    << "Estado Civil: " << ciudadano.getEstadoCivil() << ";\n";
        }
        archivo.close();
    } else {
        cerr << "No se pudo abrir el archivo para escritura." << endl;
        return;
    }

    // Guardar el Cuckoo Hash en un archivo
    cuckooHash.save("cuckoo_hash.dat");
}

void cargarDNIsEnBTree(const std::string& nombreArchivo, CuckooHashTable& cuckooHash, BTree& btree) {
    ifstream archivo(nombreArchivo);
    if (archivo.is_open()) {
        string linea;
        while (getline(archivo, linea)) {
            if (linea.find("DNI: ") != string::npos) {
                int dni = stoi(linea.substr(5));
                btree.insert(dni);
            }
        }
        archivo.close();
    } else {
        cerr << "No se pudo abrir el archivo para lectura." << endl;
    }
}

void eliminarCiudadano(int dni, CuckooHashTable& cuckooHash, BTree& btree, const string& nombreArchivo) {
    // Eliminar del Cuckoo Hash
    if (cuckooHash.remove(dni)) {
        cout << "DNI eliminado del Cuckoo Hash." << endl;
    } else {
        cerr << "DNI no encontrado en el Cuckoo Hash." << endl;
        return;
    }

    // Eliminar del B-Tree
    btree.remove(dni);
    cout << "DNI eliminado del B-Tree." << endl;

    // Eliminar del archivo (reescribir el archivo sin el ciudadano eliminado)
    ifstream archivo(nombreArchivo);
    ofstream archivoTemporal("temp.txt");

    if (archivo.is_open() && archivoTemporal.is_open()) {
        string linea;
        bool skip = false;

        while (getline(archivo, linea)) {
            if (linea.find("DNI: " + to_string(dni)) != string::npos) {
                skip = true;
            }
            if (!skip) {
                archivoTemporal << linea << "\n";
            }
            if (linea.find(';') != string::npos) {
                skip = false;
            }
        }

        archivo.close();
        archivoTemporal.close();

        // Reemplazar el archivo original con el archivo temporal
        fs::remove(nombreArchivo);
        fs::rename("temp.txt", nombreArchivo);

        cout << "DNI eliminado del archivo." << endl;
    } else {
        cerr << "Error al abrir el archivo para eliminación." << endl;
    }
}

void menu() {
    const int NUM_CIUDADANOS = 10; // Número de ciudadanos a generar

    // Crear Cuckoo Hash Table y B-Tree
    CuckooHashTable cuckooHash(2 * NUM_CIUDADANOS);
    BTree btree(3); // Grado del B-Tree

    // Generar ciudadanos y guardarlos en el archivo
    generarCiudadanos(NUM_CIUDADANOS, cuckooHash);

    // Cargar DNIs desde el archivo utilizando el Cuckoo Hash en el B-Tree
    cargarDNIsEnBTree("ciudadanos.txt", cuckooHash, btree);

    // Menú
    int opcion;
    do {
        cout << "\nMenu:\n";
        cout << "1. Buscar Ciudadano por DNI\n";
        cout << "2. Eliminar Ciudadano por DNI\n";
        cout << "3. Insertar nuevo Ciudadano\n";
        cout << "4. Mostrar B-Tree\n";
        cout << "5. Salir\n";
        cout << "Seleccione una opcion: ";
        cin >> opcion;

        switch (opcion) {
            case 1: {
                int dniBuscado;
                cout << "Ingrese el DNI que desea buscar: ";
                cin >> dniBuscado;
                cuckooHash.buscarCiudadanoPorDni(dniBuscado, "ciudadanos.txt");
                break;
            }
            case 2: {
                int dniEliminar;
                cout << "Ingrese el DNI que desea eliminar: ";
                cin >> dniEliminar;
                eliminarCiudadano(dniEliminar, cuckooHash, btree, "ciudadanos.txt");
                break;
            }
            case 3: {
                // Generar y añadir un nuevo ciudadano
                generarCiudadanos(1, cuckooHash);
                cargarDNIsEnBTree("ciudadanos.txt", cuckooHash, btree);
                break;
            }
            case 4: {
                // Mostrar el B-Tree
                mostrarBTree(btree);
                break;
            }
            case 5: {
                cout << "Saliendo...\n";
                break;
            }
            default:
                cout << "Opcion no valida. Intente de nuevo.\n";
        }
    } while (opcion != 5);
}

int main() {
    menu();
    return 0;
}