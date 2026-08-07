#include "operacionesJugadores.hpp"
#include "../persistencia/GestorArchivos.hpp"
#include "../utilidades/Formatos.hpp"
#include <iostream>
#include <fstream>

const char* OperacionesJugadores::RUTA_ARCHIVO = "datos/jugadores.bin";

bool OperacionesJugadores::registrarJugador(Jugador& jugador) {
    if (!jugador.esValido()) return false;
    if (existeCedula(jugador.getCedula())) return false;
    return GestorArchivos::guardarRegistro<Jugador>(RUTA_ARCHIVO, jugador);
}

bool OperacionesJugadores::leerJugadorPorID(int id, Jugador& resultado) {
    return GestorArchivos::leerRegistroPorID<Jugador>(RUTA_ARCHIVO, id, resultado);
}

bool OperacionesJugadores::actualizarJugador(Jugador& jugador) {
    jugador.setFechaUltimaModificacion(time(nullptr));
    return GestorArchivos::actualizarRegistro<Jugador>(RUTA_ARCHIVO, jugador);
}

bool OperacionesJugadores::eliminarJugadorLogico(int id) {
    Jugador jug;
    if (!leerJugadorPorID(id, jug)) return false;
    jug.setEliminado(true);
    if (!actualizarJugador(jug)) return false;
    return GestorArchivos::decrementarActivos(RUTA_ARCHIVO);
}

int OperacionesJugadores::listarJugadoresPorEquipo(int idEquipo, Jugador resultados[], int maxResultados) {
    std::ifstream archivo(RUTA_ARCHIVO, std::ios::binary);
    if (!archivo) return 0;

    GestorArchivos::ArchivoHeader h;
    archivo.read(reinterpret_cast<char*>(&h), sizeof(GestorArchivos::ArchivoHeader));

    int count = 0;
    for (int i = 0; i < h.cantidadRegistros && count < maxResultados; i++) {
        Jugador jug;
        archivo.read(reinterpret_cast<char*>(&jug), sizeof(Jugador));
        if (!jug.getEliminado() && jug.getIdEquipo() == idEquipo) {
            resultados[count++] = jug;
        }
    }
    archivo.close();
    return count;
}

int OperacionesJugadores::contarJugadoresActivos() {
    return GestorArchivos::leerHeader(RUTA_ARCHIVO).registrosActivos;
}

void OperacionesJugadores::listarJugadores() {
    std::ifstream archivo(RUTA_ARCHIVO, std::ios::binary);
    if (!archivo) return;

    GestorArchivos::ArchivoHeader h;
    archivo.read(reinterpret_cast<char*>(&h), sizeof(GestorArchivos::ArchivoHeader));

    std::cout << "\n--- LISTADO GENERAL DE JUGADORES ---\n";
    for (int i = 0; i < h.cantidadRegistros; i++) {
        Jugador jug;
        archivo.read(reinterpret_cast<char*>(&jug), sizeof(Jugador));
        if (!jug.getEliminado()) {
            jug.mostrarBasico();
        }
    }
    archivo.close();
}

bool OperacionesJugadores::existeCedula(const char* cedula) {
    std::ifstream archivo(RUTA_ARCHIVO, std::ios::binary);
    if (!archivo) return false;

    GestorArchivos::ArchivoHeader h;
    archivo.read(reinterpret_cast<char*>(&h), sizeof(GestorArchivos::ArchivoHeader));

    for (int i = 0; i < h.cantidadRegistros; i++) {
        Jugador jug;
        archivo.read(reinterpret_cast<char*>(&jug), sizeof(Jugador));
        if (!jug.getEliminado() && Formatos::compararCadenas(jug.getCedula(), cedula)) {
            archivo.close();
            return true;
        }
    }
    archivo.close();
    return false;
}

int OperacionesJugadores::obtenerTodosLosJugadores(Jugador resultados[], int maxResultados) {
    std::ifstream archivo(RUTA_ARCHIVO, std::ios::binary);
    if (!archivo) return 0;

    GestorArchivos::ArchivoHeader h;
    archivo.read(reinterpret_cast<char*>(&h), sizeof(GestorArchivos::ArchivoHeader));

    int count = 0;
    for (int i = 0; i < h.cantidadRegistros && count < maxResultados; i++) {
        Jugador jug;
        archivo.read(reinterpret_cast<char*>(&jug), sizeof(Jugador));
        if (!jug.getEliminado()) {
            resultados[count++] = jug;
        }
    }
    archivo.close();
    return count;
}