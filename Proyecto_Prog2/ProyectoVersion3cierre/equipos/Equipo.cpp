#include "Equipo.hpp"
#include "../utilidades/Formatos.hpp"
#include "../utilidades/Validaciones.hpp"
#include <iostream>
#include <cstring>

Equipo::Equipo() {
    id = 0;
    nombre[0] = '\0';
    ciudad[0] = '\0';
    entrenador[0] = '\0';
    puntos = 0;
    victorias = 0;
    empates = 0;
    derrotas = 0;
    golesAFavor = 0;
    golesEnContra = 0;
    cantidadPartidos = 0;
    eliminado = false;
    fechaCreacion = time(nullptr);
    fechaUltimaModificacion = time(nullptr);
}

Equipo::Equipo(const char* nombre, const char* ciudad, const char* entrenador) {
    id = 0;
    setNombre(nombre);
    setCiudad(ciudad);
    setEntrenador(entrenador);
    puntos = 0;
    victorias = 0;
    empates = 0;
    derrotas = 0;
    golesAFavor = 0;
    golesEnContra = 0;
    cantidadPartidos = 0;
    eliminado = false;
    fechaCreacion = time(nullptr);
    fechaUltimaModificacion = time(nullptr);
}

Equipo::Equipo(const Equipo& other) {
    id = other.id;
    Formatos::copiarCadena(nombre, other.nombre);
    Formatos::copiarCadena(ciudad, other.ciudad);
    Formatos::copiarCadena(entrenador, other.entrenador);
    puntos = other.puntos;
    victorias = other.victorias;
    empates = other.empates;
    derrotas = other.derrotas;
    golesAFavor = other.golesAFavor;
    golesEnContra = other.golesEnContra;
    cantidadPartidos = other.cantidadPartidos;
    for (int i = 0; i < cantidadPartidos; i++) {
        partidosIDs[i] = other.partidosIDs[i];
    }
    eliminado = other.eliminado;
    fechaCreacion = other.fechaCreacion;
    fechaUltimaModificacion = other.fechaUltimaModificacion;
}

Equipo::~Equipo() {}

int Equipo::getId() const { return id; }
const char* Equipo::getNombre() const { return nombre; }
const char* Equipo::getCiudad() const { return ciudad; }
const char* Equipo::getEntrenador() const { return entrenador; }
int Equipo::getPuntos() const { return puntos; }
int Equipo::getVictorias() const { return victorias; }
int Equipo::getEmpates() const { return empates; }
int Equipo::getDerrotas() const { return derrotas; }
int Equipo::getGolesAFavor() const { return golesAFavor; }
int Equipo::getGolesEnContra() const { return golesEnContra; }
const int* Equipo::getPartidosIDs() const { return partidosIDs; }
int Equipo::getCantidadPartidos() const { return cantidadPartidos; }
bool Equipo::getEliminado() const { return eliminado; }
time_t Equipo::getFechaCreacion() const { return fechaCreacion; }
time_t Equipo::getFechaUltimaModificacion() const { return fechaUltimaModificacion; }

bool Equipo::setId(int id) {
    if (id < 0) return false;
    this->id = id;
    return true;
}

bool Equipo::setNombre(const char* nombre) {
    if (nombre == nullptr || strlen(nombre) == 0) return false;
    Formatos::copiarCadena(this->nombre, nombre);
    return true;
}

bool Equipo::setCiudad(const char* ciudad) {
    if (ciudad == nullptr || strlen(ciudad) == 0) return false;
    Formatos::copiarCadena(this->ciudad, ciudad);
    return true;
}

bool Equipo::setEntrenador(const char* entrenador) {
    if (entrenador == nullptr || strlen(entrenador) == 0) return false;
    Formatos::copiarCadena(this->entrenador, entrenador);
    return true;
}

bool Equipo::setPuntos(int puntos) {
    if (puntos < 0) return false;
    this->puntos = puntos;
    return true;
}

bool Equipo::setVictorias(int victorias) {
    if (victorias < 0) return false;
    this->victorias = victorias;
    return true;
}

bool Equipo::setEmpates(int empates) {
    if (empates < 0) return false;
    this->empates = empates;
    return true;
}

bool Equipo::setDerrotas(int derrotas) {
    if (derrotas < 0) return false;
    this->derrotas = derrotas;
    return true;
}

bool Equipo::setGolesAFavor(int goles) {
    if (goles < 0) return false;
    golesAFavor = goles;
    return true;
}

bool Equipo::setGolesEnContra(int goles) {
    if (goles < 0) return false;
    golesEnContra = goles;
    return true;
}

bool Equipo::agregarPartidoID(int idPartido) {
    if (cantidadPartidos >= 50) return false;
    partidosIDs[cantidadPartidos++] = idPartido;
    return true;
}

bool Equipo::eliminarPartidoID(int idPartido) {
    for (int i = 0; i < cantidadPartidos; i++) {
        if (partidosIDs[i] == idPartido) {
            for (int j = i; j < cantidadPartidos - 1; j++) {
                partidosIDs[j] = partidosIDs[j + 1];
            }
            cantidadPartidos--;
            return true;
        }
    }
    return false;
}

bool Equipo::setEliminado(bool eliminado) {
    this->eliminado = eliminado;
    return true;
}

void Equipo::setFechaCreacion(time_t fecha) {
    fechaCreacion = fecha;
}

void Equipo::setFechaUltimaModificacion(time_t fecha) {
    fechaUltimaModificacion = fecha;
}

bool Equipo::esValido() const {
    return nombre[0] != '\0' && ciudad[0] != '\0' && entrenador[0] != '\0';
}

void Equipo::actualizarEstadisticas(int golesFavor, int golesContra, bool victoria, bool empate) {
    golesAFavor += golesFavor;
    golesEnContra += golesContra;
    if (victoria) {
        victorias++;
        puntos += 3;
    } else if (empate) {
        empates++;
        puntos += 1;
    } else {
        derrotas++;
    }
}

void Equipo::revertirEstadisticas(int golesFavor, int golesContra, bool victoria, bool empate) {
    golesAFavor -= golesFavor;
    golesEnContra -= golesContra;
    if (victoria) {
        victorias--;
        puntos -= 3;
    } else if (empate) {
        empates--;
        puntos -= 1;
    } else {
        derrotas--;
    }
}

void Equipo::mostrarBasico() const {
    std::cout << "ID: " << id << " | " << nombre << " | " << ciudad << " | Puntos: " << puntos << "\n";
}

void Equipo::mostrarCompleto() const {
    std::cout << "╔══════════════════════════════════════════════════════╗\n";
    std::cout << "║              DATOS DEL EQUIPO                        ║\n";
    std::cout << "╠══════════════════════════════════════════════════════╣\n";
    std::cout << "║  ID          : " << id << "\n";
    std::cout << "║  Nombre      : " << nombre << "\n";
    std::cout << "║  Ciudad      : " << ciudad << "\n";
    std::cout << "║  Entrenador  : " << entrenador << "\n";
    std::cout << "║  Puntos      : " << puntos << "\n";
    std::cout << "║  Victorias   : " << victorias << "\n";
    std::cout << "║  Empates     : " << empates << "\n";
    std::cout << "║  Derrotas    : " << derrotas << "\n";
    std::cout << "║  Goles Favor : " << golesAFavor << "\n";
    std::cout << "║  Goles Contra: " << golesEnContra << "\n";
    std::cout << "║  Partidos    : " << cantidadPartidos << "\n";
    std::cout << "║  Eliminado   : " << (eliminado ? "SI" : "NO") << "\n";
    std::cout << "╚══════════════════════════════════════════════════════╝\n";
}

size_t Equipo::obtenerTamano() {
    return sizeof(Equipo);
}

bool Equipo::operator<(const Equipo& other) const {
    int diff1 = golesAFavor - golesEnContra;
    int diff2 = other.golesAFavor - other.golesEnContra;
    
    if (puntos != other.puntos) return puntos < other.puntos;
    if (diff1 != diff2) return diff1 < diff2;
    return golesAFavor < other.golesAFavor;
}