#include "operacionesPartidos.hpp"
#include "../persistencia/GestorArchivos.hpp"
#include "../equipos/operacionesEquipos.hpp"
#include "../jugadores/operacionesJugadores.hpp"
#include "../utilidades/Formatos.hpp"
#include <iostream>
#include <fstream>

const char* OperacionesPartidos::RUTA_ARCHIVO = "datos/partidos.bin";

bool OperacionesPartidos::programarPartido(Partido& partido) {
    if (!partido.esValido()) return false;

    Equipo local, visitante;
    if (!OperacionesEquipos::leerEquipoPorID(partido.getIdEquipoLocal(), local)) return false;
    if (!OperacionesEquipos::leerEquipoPorID(partido.getIdEquipoVisitante(), visitante)) return false;
    if (partido.getIdEquipoLocal() == partido.getIdEquipoVisitante()) return false;

    return GestorArchivos::guardarRegistro<Partido>(RUTA_ARCHIVO, partido);
}

bool OperacionesPartidos::leerPartidoPorID(int id, Partido& resultado) {
    return GestorArchivos::leerRegistroPorID<Partido>(RUTA_ARCHIVO, id, resultado);
}

bool OperacionesPartidos::actualizarPartido(Partido& partido) {
    partido.setFechaUltimaModificacion(time(nullptr));
    return GestorArchivos::actualizarRegistro<Partido>(RUTA_ARCHIVO, partido);
}

bool OperacionesPartidos::eliminarPartidoLogico(int id) {
    Partido part;
    if (!leerPartidoPorID(id, part)) return false;
    part.setEliminado(true);
    if (!actualizarPartido(part)) return false;
    return GestorArchivos::decrementarActivos(RUTA_ARCHIVO);
}

int OperacionesPartidos::listarPartidosPorEquipo(int idEquipo, Partido resultados[], int maxResultados) {
    std::ifstream archivo(RUTA_ARCHIVO, std::ios::binary);
    if (!archivo) return 0;

    GestorArchivos::ArchivoHeader h;
    archivo.read(reinterpret_cast<char*>(&h), sizeof(GestorArchivos::ArchivoHeader));

    int count = 0;
    for (int i = 0; i < h.cantidadRegistros && count < maxResultados; i++) {
        Partido p;
        archivo.read(reinterpret_cast<char*>(&p), sizeof(Partido));
        if (!p.getEliminado() &&
            (p.getIdEquipoLocal() == idEquipo || p.getIdEquipoVisitante() == idEquipo)) {
            resultados[count++] = p;
        }
    }
    archivo.close();
    return count;
}

int OperacionesPartidos::listarPartidosPorEstado(const char* estado, Partido resultados[], int maxResultados) {
    std::ifstream archivo(RUTA_ARCHIVO, std::ios::binary);
    if (!archivo) return 0;

    GestorArchivos::ArchivoHeader h;
    archivo.read(reinterpret_cast<char*>(&h), sizeof(GestorArchivos::ArchivoHeader));

    int count = 0;
    for (int i = 0; i < h.cantidadRegistros && count < maxResultados; i++) {
        Partido p;
        archivo.read(reinterpret_cast<char*>(&p), sizeof(Partido));
        if (!p.getEliminado() && Formatos::compararCadenas(p.getEstado(), estado)) {
            resultados[count++] = p;
        }
    }
    archivo.close();
    return count;
}

int OperacionesPartidos::contarPartidosActivos() {
    return GestorArchivos::leerHeader(RUTA_ARCHIVO).registrosActivos;
}

void OperacionesPartidos::listarPartidos() {
    std::ifstream archivo(RUTA_ARCHIVO, std::ios::binary);
    if (!archivo) return;

    GestorArchivos::ArchivoHeader h;
    archivo.read(reinterpret_cast<char*>(&h), sizeof(GestorArchivos::ArchivoHeader));

    std::cout << "\n--- CRONOGRAMA GENERAL DE PARTIDOS ---\n";
    for (int i = 0; i < h.cantidadRegistros; i++) {
        Partido p;
        archivo.read(reinterpret_cast<char*>(&p), sizeof(Partido));
        if (!p.getEliminado()) {
            Equipo loc, vis;
            const char* nLoc = OperacionesEquipos::leerEquipoPorID(p.getIdEquipoLocal(), loc) ? loc.getNombre() : "N/A";
            const char* nVis = OperacionesEquipos::leerEquipoPorID(p.getIdEquipoVisitante(), vis) ? vis.getNombre() : "N/A";
            std::cout << "ID: " << p.getId() << " | " << nLoc << " " << p.getGolesLocal()
                      << " - " << p.getGolesVisitante() << " " << nVis
                      << " | Estado: " << p.getEstado() << "\n";
        }
    }
    archivo.close();
}

bool OperacionesPartidos::registrarResultado(int idPartido, int golesLocal, int golesVisitante,
                                             Gol detalleGoles[], int numGoles) {
    Partido part;
    if (!leerPartidoPorID(idPartido, part)) return false;
    if (!Formatos::compararCadenas(part.getEstado(), "PROGRAMADO")) return false;

    Equipo local, visitante;
    if (!OperacionesEquipos::leerEquipoPorID(part.getIdEquipoLocal(), local)) return false;
    if (!OperacionesEquipos::leerEquipoPorID(part.getIdEquipoVisitante(), visitante)) return false;

    part.setGolesLocal(golesLocal);
    part.setGolesVisitante(golesVisitante);
    part.limpiarGoles();
    for (int i = 0; i < numGoles; i++) {
        part.agregarGol(detalleGoles[i]);
    }
    part.setEstado("JUGADO");

    bool victoriaLocal = golesLocal > golesVisitante;
    bool empate = golesLocal == golesVisitante;
    bool victoriaVisitante = golesVisitante > golesLocal;

    local.actualizarEstadisticas(golesLocal, golesVisitante, victoriaLocal, empate);
    visitante.actualizarEstadisticas(golesVisitante, golesLocal, victoriaVisitante, empate);

    local.agregarPartidoID(idPartido);
    visitante.agregarPartidoID(idPartido);

    for (int i = 0; i < numGoles; i++) {
        if (detalleGoles[i].getIdJugador() > 0) {
            Jugador jug;
            if (OperacionesJugadores::leerJugadorPorID(detalleGoles[i].getIdJugador(), jug)) {
                jug.incrementarGoles();
                OperacionesJugadores::actualizarJugador(jug);
            }
        }
    }

    return actualizarPartido(part) &&
           OperacionesEquipos::actualizarEquipo(local) &&
           OperacionesEquipos::actualizarEquipo(visitante);
}

bool OperacionesPartidos::cancelarPartidoJugado(int idPartido) {
    Partido part;
    if (!leerPartidoPorID(idPartido, part)) return false;
    if (!Formatos::compararCadenas(part.getEstado(), "JUGADO")) return false;

    Equipo local, visitante;
    if (!OperacionesEquipos::leerEquipoPorID(part.getIdEquipoLocal(), local)) return false;
    if (!OperacionesEquipos::leerEquipoPorID(part.getIdEquipoVisitante(), visitante)) return false;

    bool victoriaLocal = part.getGolesLocal() > part.getGolesVisitante();
    bool empate = part.getGolesLocal() == part.getGolesVisitante();
    bool victoriaVisitante = part.getGolesVisitante() > part.getGolesLocal();

    local.revertirEstadisticas(part.getGolesLocal(), part.getGolesVisitante(), victoriaLocal, empate);
    visitante.revertirEstadisticas(part.getGolesVisitante(), part.getGolesLocal(), victoriaVisitante, empate);

    local.eliminarPartidoID(idPartido);
    visitante.eliminarPartidoID(idPartido);

    const Gol* golesArray = part.getGoles();
    for (int i = 0; i < part.getNumGoles(); i++) {
        if (golesArray[i].getIdJugador() > 0) {
            Jugador jug;
            if (OperacionesJugadores::leerJugadorPorID(golesArray[i].getIdJugador(), jug)) {
                jug.decrementarGoles();
                OperacionesJugadores::actualizarJugador(jug);
            }
        }
    }

    part.setEstado("CANCELADO");
    part.limpiarGoles();

    return actualizarPartido(part) &&
           OperacionesEquipos::actualizarEquipo(local) &&
           OperacionesEquipos::actualizarEquipo(visitante);
}

int OperacionesPartidos::obtenerTodosLosPartidos(Partido resultados[], int maxResultados) {
    std::ifstream archivo(RUTA_ARCHIVO, std::ios::binary);
    if (!archivo) return 0;

    GestorArchivos::ArchivoHeader h;
    archivo.read(reinterpret_cast<char*>(&h), sizeof(GestorArchivos::ArchivoHeader));

    int count = 0;
    for (int i = 0; i < h.cantidadRegistros && count < maxResultados; i++) {
        Partido p;
        archivo.read(reinterpret_cast<char*>(&p), sizeof(Partido));
        if (!p.getEliminado()) {
            resultados[count++] = p;
        }
    }
    archivo.close();
    return count;
}