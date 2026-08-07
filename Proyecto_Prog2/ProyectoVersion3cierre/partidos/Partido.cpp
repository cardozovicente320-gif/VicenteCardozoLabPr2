#include "Partido.hpp"
#include "../utilidades/Formatos.hpp"
#include "../utilidades/Validaciones.hpp"
#include <iostream>
#include <cstring>

Partido::Partido() {
    id = 0;
    idEquipoLocal = 0;
    idEquipoVisitante = 0;
    fecha[0] = '\0';
    Formatos::copiarCadena(estado, "PROGRAMADO");
    descripcion[0] = '\0';
    golesLocal = 0;
    golesVisitante = 0;
    numGoles = 0;
    eliminado = false;
    fechaCreacion = time(nullptr);
    fechaUltimaModificacion = time(nullptr);
}

Partido::Partido(int idLocal, int idVisitante, const char* fecha, const char* descripcion) {
    id = 0;
    setIdEquipoLocal(idLocal);
    setIdEquipoVisitante(idVisitante);
    setFecha(fecha);
    Formatos::copiarCadena(estado, "PROGRAMADO");
    setDescripcion(descripcion);
    golesLocal = 0;
    golesVisitante = 0;
    numGoles = 0;
    eliminado = false;
    fechaCreacion = time(nullptr);
    fechaUltimaModificacion = time(nullptr);
}

Partido::Partido(const Partido& other) {
    id = other.id;
    idEquipoLocal = other.idEquipoLocal;
    idEquipoVisitante = other.idEquipoVisitante;
    Formatos::copiarCadena(fecha, other.fecha);
    Formatos::copiarCadena(estado, other.estado);
    Formatos::copiarCadena(descripcion, other.descripcion);
    golesLocal = other.golesLocal;
    golesVisitante = other.golesVisitante;
    numGoles = other.numGoles;
    for (int i = 0; i < numGoles; i++) {
        goles[i] = other.goles[i];
    }
    eliminado = other.eliminado;
    fechaCreacion = other.fechaCreacion;
    fechaUltimaModificacion = other.fechaUltimaModificacion;
}

Partido::~Partido() {}

int Partido::getId() const { return id; }
int Partido::getIdEquipoLocal() const { return idEquipoLocal; }
int Partido::getIdEquipoVisitante() const { return idEquipoVisitante; }
const char* Partido::getFecha() const { return fecha; }
const char* Partido::getEstado() const { return estado; }
const char* Partido::getDescripcion() const { return descripcion; }
int Partido::getGolesLocal() const { return golesLocal; }
int Partido::getGolesVisitante() const { return golesVisitante; }
const Gol* Partido::getGoles() const { return goles; }
int Partido::getNumGoles() const { return numGoles; }
bool Partido::getEliminado() const { return eliminado; }
time_t Partido::getFechaCreacion() const { return fechaCreacion; }
time_t Partido::getFechaUltimaModificacion() const { return fechaUltimaModificacion; }

bool Partido::setId(int id) {
    if (id < 0) return false;
    this->id = id;
    return true;
}

bool Partido::setIdEquipoLocal(int id) {
    if (id < 0) return false;
    idEquipoLocal = id;
    return true;
}

bool Partido::setIdEquipoVisitante(int id) {
    if (id < 0) return false;
    idEquipoVisitante = id;
    return true;
}

bool Partido::setFecha(const char* fecha) {
    if (!Validaciones::validarFecha(fecha)) return false;
    Formatos::copiarCadena(this->fecha, fecha);
    return true;
}

bool Partido::setEstado(const char* estado) {
    if (!Validaciones::validarEstadoPartido(estado)) return false;
    Formatos::copiarCadena(this->estado, estado);
    return true;
}

bool Partido::setDescripcion(const char* descripcion) {
    if (descripcion == nullptr) return false;
    Formatos::copiarCadena(this->descripcion, descripcion);
    return true;
}

bool Partido::setGolesLocal(int goles) {
    if (goles < 0) return false;
    golesLocal = goles;
    return true;
}

bool Partido::setGolesVisitante(int goles) {
    if (goles < 0) return false;
    golesVisitante = goles;
    return true;
}

bool Partido::agregarGol(const Gol& gol) {
    if (numGoles >= 22) return false;
    if (!gol.esValido()) return false;
    goles[numGoles++] = gol;
    return true;
}

bool Partido::setEliminado(bool eliminado) {
    this->eliminado = eliminado;
    return true;
}

void Partido::setFechaCreacion(time_t fecha) {
    fechaCreacion = fecha;
}

void Partido::setFechaUltimaModificacion(time_t fecha) {
    fechaUltimaModificacion = fecha;
}

bool Partido::esValido() const {
    return idEquipoLocal > 0 && idEquipoVisitante > 0 &&
           idEquipoLocal != idEquipoVisitante &&
           fecha[0] != '\0' && Validaciones::validarEstadoPartido(estado);
}

void Partido::limpiarGoles() {
    numGoles = 0;
    golesLocal = 0;
    golesVisitante = 0;
}

void Partido::mostrarBasico() const {
    std::cout << "ID: " << id << " | Local: " << idEquipoLocal
              << " " << golesLocal << " - " << golesVisitante
              << " Visitante: " << idEquipoVisitante
              << " | Estado: " << estado << "\n";
}

void Partido::mostrarCompleto() const {
    std::cout << "╔══════════════════════════════════════════════════════╗\n";
    std::cout << "║              DATOS DEL PARTIDO                       ║\n";
    std::cout << "╠══════════════════════════════════════════════════════╣\n";
    std::cout << "║  ID              : " << id << "\n";
    std::cout << "║  Local           : " << idEquipoLocal << "\n";
    std::cout << "║  Visitante       : " << idEquipoVisitante << "\n";
    std::cout << "║  Fecha           : " << fecha << "\n";
    std::cout << "║  Estado          : " << estado << "\n";
    std::cout << "║  Descripción     : " << descripcion << "\n";
    std::cout << "║  Goles Local     : " << golesLocal << "\n";
    std::cout << "║  Goles Visitante : " << golesVisitante << "\n";
    std::cout << "║  Total Goles     : " << numGoles << "\n";
    std::cout << "║  Eliminado       : " << (eliminado ? "SI" : "NO") << "\n";
    std::cout << "╠══════════════════════════════════════════════════════╣\n";
    std::cout << "║  DETALLE DE GOLES:\n";
    for (int i = 0; i < numGoles; i++) {
        std::cout << "║   " << (i+1) << ". ";
        goles[i].mostrar();
    }
    if (numGoles == 0) {
        std::cout << "║   (Sin goles registrados)\n";
    }
    std::cout << "╚══════════════════════════════════════════════════════╝\n";
}

size_t Partido::obtenerTamano() {
    return sizeof(Partido);
}