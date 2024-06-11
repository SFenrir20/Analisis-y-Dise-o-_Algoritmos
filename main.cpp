#include <iostream>
#include <windows.h>
#include <string> 
#include <vector>
#include <cstring>
#include <stdexcept>
#include <cstdlib>     // Para rand() y srand()
#include <ctime>       // Para time()
#include <fstream>
#include <sstream>
#include <algorithm>

using namespace std;

//pruebas
//const int TABLA_SIZE = 33000000;
//const int Max_Kickous = 200;

struct Persona{
    int DNI;
    char Nombre_Apellido[50];
    char Nacionalidad[50];
    char Lugar_Nacimiento[50];
    char Direccion[100];
    char telefono[9];
    char correo[50];
    char estadoCivil[50];

   /* Persona(){
       memset(DNI, 0, 8);
       memset(Nombre_Apellido, 0, 50);
       memset(Nacionalidad, 0, 50);
       memset(Lugar_Nacimiento, 0, 50);
       memset(Direccion, 0, 100);
       memset(telefono, 0, 9);
       memset(correo, 0, 50);
       memset(estadoCivil, 0, 50);

    }
    // Sobrecarga del operador de comparación para comparar dos personas
    bool operator==(const Persona &other) const{
        return strcmp(DNI, other.DNI) == 0;
    }*/

};

int convertDNI(const string& dni){
    return stoi(dni);
}

//const int T=3; //grado minimo dedel arbol b

struct BTreeNode{
    bool isLeaf;  //si el nodo es hoja
    //int n;          // numero de claves alamacenadas
    vector<int> keys;
    vector<BTreeNode*> children; //hijos del nodo 

    // constructorpara iniciar el nodo
    BTreeNode(bool Leaf) { 
        isLeaf = Leaf;
    }
};

class BTree{
    private:
    BTreeNode* root; //Raiz del arbol B
    int t;

    void insertNonFull(BTreeNode* node, int k);
    void splitchild(BTreeNode* node, int i, BTreeNode* y);
    void traverse(BTreeNode* node);


    //insertar una clave en un nodo no lleno
    /*void insertNonFull(BTreeNode* x, const string& k){
        int i = x->n - 1;
        if(x->isLeaf){
            while(i>= 0 && k < x->keys[i]){
                x->keys[i+1] = x->keys[i];
                i--;
            }
            x->keys[i+1] = k;
            x->n = x->n + 1;
        }else {
            while(i>=0 && k < x->keys[i])
                i--;
            i++;
            if(x->children[i]->n == 2 * T - 1){
                splitchild(x, i, x->children[i]);
                if(k > x->keys[i])
                    i++;
            }
            insertNonFull(x->children[i], k);
        }
    }

    // divide un nodo hijo en dos nodos
    void splitchild(BTreeNode* x, int i, BTreeNode* y){
        BTreeNode* z = new BTreeNode(y->isLeaf);
        z->n = T - 1;
        for(int j = 0; j < T - 1; j++)
            z->keys[j] = y-> keys[j + T];
        if(!y->isLeaf){
            for(int j = 0; j < T; j++)
                z->children[j] = y->children[j + T];
        }
        y->n = T - 1;
        for(int j = x->n; j >= i + 1; j--)
            x->children[j + 1] = x->children[j];
        x->children[i + 1] = z;
        for(int j = x->n - 1; j >= i; j--)
            x->keys[j + 1] = x->keys[j];
        x->keys[i] = y->keys[T - 1];
        x->n = x->n + 1;
    }

    // Busca una clave en el árbol B
    bool search(BTreeNode* x, const string& k){
        int i = 0;
        while(i<x->n && k > x->keys[i])
            i++;
        if(i < x->n && k == x->keys[i])
            return true;
        if(x->isLeaf)
            return false;
        return search(x->children[i], k);
    }

    // Elimina un nodo recursivamente
    void removeNode(BTreeNode* x){
        for (int i = 0; i <= x->n; i++) {
            if (!x->isLeaf)
                removeNode(x->children[i]);
        }
        delete x;
    }*/
    public:
    //constructor
    BTree(int _t){
        root = nullptr;
        t = _t;
    }

    void insert(int k);
    void traverse(){
        if(root != nullptr)
            traverse(root);
    } 
};
    /*//Destructor
    ~BTree(){
        if(root)
        removeNode(root);
    }

     void insert(const string& k) {
        if (root->n == 2 * T - 1) {
            BTreeNode* s = new BTreeNode(false);
            s->children[0] = root;
            splitchild(s, 0, root);
            root = s;
        }
        insertNonFull(root, k);
    }

    // Busca una clave en el árbol B
    bool search(const string& k) {
        return search(root, k);
    }
};*/

void BTree::insert(int k){
    if(root == nullptr){
        root = new BTreeNode(true);
        root->keys.push_back(k);
    }else{
        if(root->keys.size() == 2 * t - 1){
            BTreeNode* s = new BTreeNode(false);
            s->children.push_back(root);
            splitchild(s, 0, root);
            int i = 0;
            if(s->keys[0] < k)
                i++;
            insertNonFull(s->children[i],k);
            root = s;
        }else{
            insertNonFull(root, k);
        }
    }
}

void BTree::insertNonFull(BTreeNode* node, int k){
    int i = node->keys.size() - 1;
    if (node->isLeaf){
        node->keys.push_back(k);
        while(i >= 0 && k < node->keys[i]){
            node->keys[i+1] = node->keys[i];
            i--;
        }
        node->keys[i+1] = k;
    }else {
        while(i>=0 && k < node->keys[i])
            i--;
        i++;
        if(node->children[i+1]->keys.size() == 2 * t - 1){
            splitchild(node, i, node->children[i+1]);
            if(k > node->keys[i+1])
                i++;
        }
        insertNonFull(node->children[i+1], k);
    }
    
}

void BTree::splitchild(BTreeNode* node, int i, BTreeNode* y){
    BTreeNode* z = new BTreeNode(y->isLeaf);
    z->keys.resize(t - 1);
    
    for(int j = 0; j < t - 1; j++)
        z->keys.push_back(y->keys[j+t]);
        
    if(!y->isLeaf){
        z->children.resize(t);
        for(int j = 0; j < t; j++)
            z->children[j] = y->children[j + t];
    }

    y->keys.resize(t-1);
    
    node->children.insert(node->children.begin() + i + 1, z);
    node->keys.insert(node->keys.begin() + i, y->keys[t - 1]);
}

class CuckooHashTable {
    private:
    vector<Persona> tabla1;
    vector<Persona> tabla2;
    int size;
    //HANDLE hFile;
    //HANDLE hMapFile;
    int tablaSize;

    int hash1(const Persona &persona){
        return persona.DNI % tablaSize; 
    }

    int hash2(const Persona &persona){
        return (persona.DNI / tablaSize) % tablaSize; 
    }

    /*void rehash(){
        //opcional
        Persona* temp = tabla1;
        tabla1 = tabla2;
        tabla2 = temp;
    }*/

    public:
    //constructor
        CuckooHashTable(int size = 33000000) : size(0), tablaSize(size){

            tabla1.resize(tablaSize);
            tabla2.resize(tablaSize);


            /*tablaSize = TABLA_SIZE * sizeof(Persona);

            //Crear un archivo de mapa de memoria
            hMapFile = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, tablaSize * Max_Kickous, TEXT("CuckooHashTable"));
            if (hMapFile == NULL){
                throw runtime_error("No se pudo crear el mapeo de archivo.");*/
            }

            //Mapear las tablas de hash en memoria 
            tabla1 = (Persona*) MapViewOfFile(hMapFile, FILE_MAP_ALL_ACCESS, 0, 0, tablaSize);
            tabla2 = (Persona*) MapViewOfFile(hMapFile, FILE_MAP_ALL_ACCESS, 0, 0, tablaSize * 2);
            
            if (tabla1 == NULL || tabla2 == NULL){
                if (tabla1 != NULL) UnmapViewOfFile(tabla1);
                if (tabla2 != NULL) UnmapViewOfFile(tabla2);
                CloseHandle(hMapFile);
                throw runtime_error("No se pudo mapear la vista del archivo.");
            }

           /* memset(tabla1, 0, tablaSize);
            memset(tabla2, 0, tablaSize);*/

            // Inicializar las tablas de hash
            for (size_t i = 0; i < TABLA_SIZE; i++) {
                tabla1[i] = Persona();
                tabla2[i] = Persona();
            }
        }
        //Destructor
        ~CuckooHashTable(){
            UnmapViewOfFile(tabla1);
            UnmapViewOfFile(tabla2);
            CloseHandle(hMapFile);
        }

        // inserta una persona en la tabla de hash
        void insert(const Persona& persona){
            int count =0;
            Persona current = persona;

            while(count < Max_Kickous){
                int pos1 = hash1(current);
                if(strlen(tabla1[pos1].DNI)==0){
                    tabla1[pos1] = current;
                    size++;
                    return;
                }
                swap(current,tabla1[pos1]);

                int pos2 = hash2(current);
                if(strlen(tabla2[pos2].DNI)==0){
                    tabla2[pos2] = current;
                    size++;
                    return;
                }
                swap(current,tabla2[pos2]);
                count++;
            }

            rehash();
            insert(current);
        }

        // Busca una persona en la tabla de hash
        bool search(const Persona& persona){
            int pos1 = hash1(persona);
            if(tabla1[pos1] == persona){
                return true;
            }

            int pos2 = hash2(persona);
            if(tabla2[pos2] == persona){
                return true;
            }
            return false;
        }

        // Elimina una persona de la tabla de hash
        void remove(const Persona& persona){
            int pos1 = hash1(persona);
            if(tabla1[pos1] == persona){
                tabla1[pos1] = Persona();
                size--;
                return;
            }
            int pos2 = hash2(persona);
            if(tabla2[pos2] == persona){
                tabla2[pos2] = Persona();
                size--;
                return;
            }
        }

        // Devuelve el tamaño de la tabla de hash
        int getSize(){
            return size;
        }

};


// Función para generar datos aleatorios
void generateRandomData(CuckooHashTable &cuckooTable, BTree &bTree, int count) {
    const char alphanum[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    srand((unsigned)time(NULL));

    for (int i = 0; i < count; i++) {
        Persona persona;

        for (int j = 0; j < 8; j++) {
            persona.DNI[j] = alphanum[rand() % (sizeof(alphanum) - 1)];
        }

        for (int j = 0; j < 50; j++) {
            persona.Nombre_Apellido[j] = alphanum[rand() % (sizeof(alphanum) - 1)];
            persona.Nacionalidad[j] = alphanum[rand() % (sizeof(alphanum) - 1)];
            persona.Lugar_Nacimiento[j] = alphanum[rand() % (sizeof(alphanum) - 1)];
            persona.correo[j] = alphanum[rand() % (sizeof(alphanum) - 1)];
            persona.estadoCivil[j] = alphanum[rand() % (sizeof(alphanum) - 1)];
        }

        for (int j = 0; j < 100; j++) {
            persona.Direccion[j] = alphanum[rand() % (sizeof(alphanum) - 1)];
        }

        for (int j = 0; j < 9; j++) {
            persona.telefono[j] = alphanum[rand() % (sizeof(alphanum) - 1)];
        }

        cuckooTable.insert(persona);
        bTree.insert(persona.DNI);
    }
}


int main(){
    try
    {
        
        CuckooHashTable cuckooTable;  // Crear tabla de hash
        BTree bTree;                  // Crear árbol B
        int choice;
        string dni;
        Persona persona;
        bool dataGenerated = false;   // boleano para rastrear si los datos han sido generados

         while (true) {
            cout << "Menú:" << endl;
            cout << "1. Generar datos aleatorios" << endl;
            cout << "2. Buscar" << endl;
            cout << "3. Agregar" << endl;
            cout << "4. Eliminar" << endl;
            cout << "5. Salir" << endl;
            cout << "Seleccione una opción: ";
            cin >> choice;
        
             switch (choice)
            {
                  case 1:
                    if (!dataGenerated) {
                        cout << "Generando datos aleatorios..." << endl;
                        generateRandomData(cuckooTable, bTree, 33000000);
                        dataGenerated = true;
                        cout << "Datos generados." << endl;
                    } else {
                        cout << "Los datos ya han sido generados." << endl;
                    }
                    break;
                case 2:
                    cout << "Ingrese el DNI del ciudadano a buscar: ";
                    cin >> dni;
                    if (bTree.search(dni)) {
                        Persona searchPersona;
                        strncpy(searchPersona.DNI, dni.c_str(), 8);
                        if (cuckooTable.search(searchPersona)) {
                            cout << "Ciudadano encontrado." << endl;
                        } else {
                            cout << "Ciudadano no encontrado." << endl;
                        }
                    } else {
                        cout << "Ciudadano no encontrado." << endl;
                    }
                break;

                case 3:
                    cout << "Ingrese el DNI del ciudadano: ";
                    cin >> persona.DNI;
                    cout << "Ingrese el nombre y apellido del ciudadano: ";
                    cin.ignore();
                    cin.getline(persona.Nombre_Apellido, 50);
                    cout << "Ingrese la nacionalidad del ciudadano: ";
                    cin.getline(persona.Nacionalidad, 50);
                    cout << "Ingrese el lugar de nacimiento del ciudadano: ";
                    cin.getline(persona.Lugar_Nacimiento, 50);
                    cout << "Ingrese la dirección del ciudadano: ";
                    cin.getline(persona.Direccion, 100);
                    cout << "Ingrese el teléfono del ciudadano: ";
                    cin.getline(persona.telefono, 9);
                    cout << "Ingrese el correo del ciudadano: ";
                    cin.getline(persona.correo, 50);
                    cout << "Ingrese el estado civil del ciudadano: ";
                    cin.getline(persona.estadoCivil, 50);

                    cuckooTable.insert(persona);
                    bTree.insert(persona.DNI);
                    cout << "Ciudadano agregado." << endl;
                break;

                case 4:
                    cout << "Ingrese el DNI del ciudadano a eliminar: ";
                    cin >> dni;
                    if (bTree.search(dni)) {
                            Persona deletePersona;
                            strncpy(deletePersona.DNI, dni.c_str(), 8);
                            cuckooTable.remove(deletePersona);
                            cout << "Ciudadano eliminado." << endl;
                        } else {
                            cout << "Ciudadano no encontrado." << endl;
                        }
                break;

                case 5:
                    return 0;
                default:
                    cout << "Opción inválida. intente de nuevo" << endl;
                 break;
            }
        }  
    } 
    catch(const exception &e){
        cerr << "Error: " << e.what() << endl;
    }
    return 0;
    
}