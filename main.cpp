#include <iostream>
#include <vector>
#include <cstring>
#include <string>
#include <stdexcept>
#include <windows.h> 

using namespace std;



const int TablaSize = 33000000;
const int Max_Kickous = 500;

struct Persona{
    char DNI[8];
    char Nombre_Apellido[50];
    char Nacionalidad[50];
    char Lugar_Nacimiento[50];
    char Direccion[100];
    char telefono[9];
    char correo[50];
    char estadoCivil[50];

    Persona(){
       memset(DNI, 0, 8);
       memset(Nombre_Apellido, 0, 50);
       memset(Nacionalidad, 0, 50);
       memset(Lugar_Nacimiento, 0, 50);
       memset(Direccion, 0, 100);
       memset(telefono, 0, 9);
       memset(correo, 0, 50);
       memset(estadoCivil, 0, 50);

    }

    bool operator==(const Persona &other) const{
        return strcmp(DNI, other.DNI) == 0;
    }

};


const int T=3; //grado minimo dedel arbol b

struct BTreeNode{
    bool isLeaf;  //si el nodo es hoja
    int n;          // numero de claves alamacenadas
    vector<string> keys;
    vector<BTreeNode*> children; //hijos del nodo 

    // constructorpara iniciar el nodo
    BTreeNode(bool Leaf) : isLeaf(Leaf), n(0){ 
        keys.resize(2*T-1);
        children.resize(2*T);
    }
};

class BTree{
    private:
    BTreeNode* root; //Raiz del arbol B

    //insertar una clave en un nodo no lleno
    void insertNonFull(BTreeNode* X, const string& k){

    }

    // divide un nodo hijo en dos nodos
    void splitchild(BTreeNode* x, int i, BTreeNode* y){

    }

    // Busca una clave en el árbol B
    bool search(BTreeNode* x, const string& k){

    }

    // Elimina un nodo recursivamente
    void removeNode(BTreeNode* x){

    }
    public:
    //constructor
    BTree() : root(new BTreeNode(true)) {}

    //Destructor
    ~BTree(){
        if(root)
        removeNode(root);
    }

    // Busca una clave en el árbol B
    bool search(const string& k) {
    return search(root, k);
    }



};

class CuckooHashTable {
    
};


int main(){
    try
    {
        CuckooHashTable cuckooTable;  // Crear tabla de hash
        BTree bTree;                  // Crear árbol B
        int choice;
        string dni;
        Persona persona;
        
         while (true) {
            cout << "Menú:" << endl;
            cout << "1. Buscar" << endl;
            cout << "2. Agregar" << endl;
            cout << "3. Eliminar" << endl;
            cout << "4. Salir" << endl;
            cout << "Seleccione una opción: ";
            cin >> choice;
        
             switch (choice)
            {
                case 1:
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

                case 2:
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

                case 3:
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

                case 4:
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