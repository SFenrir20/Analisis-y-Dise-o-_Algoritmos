#ifndef MODEL_H
#define MODEL_H

#include <vector>
#include <string>
#include <map>

using namespace std;

class Ciudadano {
public:
    Ciudadano() = default;
    Ciudadano(int dni, const string& nombres, const string& apellidos, const string& nacionalidad,
              const string& lugar_de_nacimiento, const string& departamento, const string& provincia,
              const string& ciudad, const string& distrito, const string& ubicacion,
              const string& telefono_de_referencia, const string& correo_electronico, const string& estado_civil);

    int getDni() const;
    string getNombres() const;
    string getApellidos() const;
    string getNacionalidad() const;
    string getLugarDeNacimiento() const;
    string getDepartamento() const;
    string getProvincia() const;
    string getCiudad() const;
    string getDistrito() const;
    string getUbicacion() const;
    string getTelefonoDeReferencia() const;
    string getCorreoElectronico() const;
    string getEstadoCivil() const;

    void leerDesdeArchivo(ifstream& archivo);

private:
    int dni;
    string nombres;
    string apellidos;
    string nacionalidad;
    string lugar_de_nacimiento;
    string departamento;
    string provincia;
    string ciudad;
    string distrito;
    string ubicacion;
    string telefono_de_referencia;
    string correo_electronico;
    string estado_civil;
};

struct CuckooHashEntry {
    int key = 0;
    long position = 0;
    bool occupied = false;
};

class CuckooHashTable {
public:
    explicit CuckooHashTable(int size);
    bool insert(int key, long position);
    bool remove(int key);
    long search(int key) const;
    void save(const string& filename) const;
    void load(const string& filename);
    void buscarCiudadanoPorDni(int dni, const string& nombreArchivo) const;

private:
    int size;
    vector<CuckooHashEntry> table1;
    vector<CuckooHashEntry> table2;

    int hash1(int key) const;
    int hash2(int key) const;
};

class BTreeNode {
public:
    BTreeNode(int t, bool leaf);

    void traverse() const;
    BTreeNode* search(int k);
    void insertNonFull(int k);
    void splitChild(int i, BTreeNode* y);
    void remove(int k);

private:
    int t;
    bool leaf;
    vector<int> keys;
    vector<BTreeNode*> children;

    void removeFromLeaf(int idx);
    void removeFromNonLeaf(int idx);
    int getPred(int idx);
    int getSucc(int idx);
    void fill(int idx);
    void borrowFromPrev(int idx);
    void borrowFromNext(int idx);
    void merge(int idx);
    int findKey(int k);

    friend class BTree;
};

class BTree {
public:
    explicit BTree(int t);

    void traverse() const;
    BTreeNode* search(int k);
    void insert(int k);
    void remove(int k);

private:
    BTreeNode* root;
    int t;
};

#endif // MODEL_H
