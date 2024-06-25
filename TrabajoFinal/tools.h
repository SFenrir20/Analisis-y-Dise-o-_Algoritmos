#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>

std::vector<std::string> leerArchivo(const std::string& nombreArchivo) {
    std::vector<std::string> lineas;
    std::ifstream archivo(nombreArchivo);
    if (archivo.is_open()) {
        std::string linea;
        while (std::getline(archivo, linea)) {
            lineas.push_back(linea);
        }
        archivo.close();
    } else {
        std::cerr << "No se pudo abrir el archivo: " << nombreArchivo << std::endl;
    }
    return lineas;
}

std::vector<std::vector<std::string>> leerArchivoEstructurado(const std::string& nombreArchivo) {
    std::vector<std::vector<std::string>> datos;
    std::ifstream archivo(nombreArchivo);
    if (archivo.is_open()) {
        std::string linea;
        while (std::getline(archivo, linea)) {
            std::vector<std::string> partes;
            std::size_t inicio = 0, fin;
            while ((fin = linea.find(',', inicio)) != std::string::npos) {
                partes.push_back(linea.substr(inicio, fin - inicio));
                inicio = fin + 1;
            }
            partes.push_back(linea.substr(inicio));
            datos.push_back(partes);
        }
        archivo.close();
    } else {
        std::cerr << "No se pudo abrir el archivo: " << nombreArchivo << std::endl;
    }
    return datos;
}
