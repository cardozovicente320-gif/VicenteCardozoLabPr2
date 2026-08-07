#include "operacionesEquipos.hpp"
#include "../persistencia/GestorArchivos.hpp"
#include "../utilidades/Formatos.hpp"
#include <iostream>
#include <fstream>

const char* OperacionesEquipos::RUTA_ARCHIVO = "datos/equipos.bin";

bool OperacionesEquipos::registrarEquipo(Equipo& equipo) {
    if (!equipo.esValido()) return false;
    
    Equipo control[100];
    int hallados = buscarEquiposPorNombre(equipo.getNombre(), control, 100);
    for (int i = 0; i < hallados; i++) {
        if (Formatos::compararCadenas(control[i].getNombre(), equipo.getNombre())) {
            return false;
        }
    }
    
    return GestorArchivos::guardarRegistro<Equipo>(RUTA_ARCHIVO, equipo);
}

bool OperacionesEquipos::leerEquipoPorID(int id, Equipo& resultado) {
    return GestorArchivos::leerRegistroPorID<Equipo>(RUTA_ARCHIVO, id, resultado);
}

bool OperacionesEquipos::actualizarEquipo(Equipo& equipo) {
    equipo.setFechaUltimaModificacion(time(nullptr));
    return GestorArchivos::actualizarRegistro<Equipo>(RUTA_ARCHIVO, equipo);
}

bool OperacionesEquipos::eliminarEquipoLogico(int id) {
    Equipo eq;
    if (!leerEquipoPorID(id, eq)) return false;
    eq.setEliminado(true);
    if (!actualizarEquipo(eq)) return false;
    return GestorArchivos::decrementarActivos(RUTA_ARCHIVO);
}

int OperacionesEquipos::buscarEquiposPorNombre(const char* subcadena, Equipo resultados[], int maxResultados) {
    std::ifstream archivo(RUTA_ARCHIVO, std::ios::binary);
    if (!archivo) return 0;
    
    GestorArchivos::ArchivoHeader h;
    archivo.read(reinterpret_cast<char*>(&h), sizeof(GestorArchivos::ArchivoHeader));
    
    int count = 0;
    for (int i = 0; i < h.cantidadRegistros && count < maxResultados; i++) {
        Equipo eq;
        archivo.read(reinterpret_cast<char*>(&eq), sizeof(Equipo));
        if (!eq.getEliminado() && Formatos::contieneSubcadena(eq.getNombre(), subcadena)) {
            resultados[count++] = eq;
        }
    }
    archivo.close();
    return count;
}

int OperacionesEquipos::contarEquiposActivos() {
    return GestorArchivos::leerHeader(RUTA_ARCHIVO).registrosActivos;
}

void OperacionesEquipos::listarEquipos() {
    std::ifstream archivo(RUTA_ARCHIVO, std::ios::binary);
    if (!archivo) return;
    
    GestorArchivos::ArchivoHeader h;
    archivo.read(reinterpret_cast<char*>(&h), sizeof(GestorArchivos::ArchivoHeader));
    
    std::cout << "\n--- LISTADO GENERAL DE EQUIPOS ---\n";
    for (int i = 0; i < h.cantidadRegistros; i++) {
        Equipo eq;
        archivo.read(reinterpret_cast<char*>(&eq), sizeof(Equipo));
        if (!eq.getEliminado()) {
            eq.mostrarBasico();
        }
    }
    archivo.close();
}

void OperacionesEquipos::ordenarEquipos(Equipo arr[], int tam) {
    for (int i = 0; i < tam - 1; i++) {
        for (int j = 0; j < tam - i - 1; j++) {
            if (arr[j] < arr[j+1]) {
                Equipo temp = arr[j];
                arr[j] = arr[j+1];
                arr[j+1] = temp;
            }
        }
    }
}

int OperacionesEquipos::obtenerTodosLosEquipos(Equipo resultados[], int maxResultados) {
    std::ifstream archivo(RUTA_ARCHIVO, std::ios::binary);
    if (!archivo) return 0;
    
    GestorArchivos::ArchivoHeader h;
    archivo.read(reinterpret_cast<char*>(&h), sizeof(GestorArchivos::ArchivoHeader));
    
    int count = 0;
    for (int i = 0; i < h.cantidadRegistros && count < maxResultados; i++) {
        Equipo eq;
        archivo.read(reinterpret_cast<char*>(&eq), sizeof(Equipo));
        if (!eq.getEliminado()) {
            resultados[count++] = eq;
        }
    }
    archivo.close();
    return count;
}