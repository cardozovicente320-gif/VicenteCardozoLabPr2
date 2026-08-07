#include "Jugador.hpp"
#include "../utilidades/Formatos.hpp"
#include "../utilidades/Validaciones.hpp"
#include <iostream>
#include <cstring>

Jugador::Jugador() {
    id = 0;
    idEquipo = 0;
    nombre[0] = '\0';
    cedula[0] = '\0';
    posicion[0] = '\0';
    edad = 0;
    numeroDorsal = 0;
    golesAnotados = 0;
    tarjetasAmarillas = 0;
    tarjetasRojas = 0;
    eliminado = false;
    fechaCreacion = time(nullptr);
    fechaUltimaModificacion = time(nullptr);
}

Jugador::Jugador(int idEquipo, const char* nombre, const char* cedula,
                 const char* posicion, int edad, int numeroDorsal) {
    this->id = 0;
    setIdEquipo(idEquipo);
    setNombre(nombre);
    setCedula(cedula);
    setPosicion(posicion);
    setEdad(edad);
    setNumeroDorsal(numeroDorsal);
    golesAnotados = 0;
    tarjetasAmarillas = 0;
    tarjetasRojas = 0;
    eliminado = false;
    fechaCreacion = time(nullptr);
    fechaUltimaModificacion = time(nullptr);
}

Jugador::Jugador(const Jugador& other) {
    id = other.id;
    idEquipo = other.idEquipo;
    Formatos::copiarCadena(nombre, other.nombre);
    Formatos::copiarCadena(cedula, other.cedula);
    Formatos::copiarCadena(posicion, other.posicion);
    edad = other.edad;
    numeroDorsal = other.numeroDorsal;
    golesAnotados = other.golesAnotados;
    tarjetasAmarillas = other.tarjetasAmarillas;
    tarjetasRojas = other.tarjetasRojas;
    eliminado = other.eliminado;
    fechaCreacion = other.fechaCreacion;
    fechaUltimaModificacion = other.fechaUltimaModificacion;
}

Jugador::~Jugador() {}

// Getters
int Jugador::getId() const { return id; }
int Jugador::getIdEquipo() const { return idEquipo; }
const char* Jugador::getNombre() const { return nombre; }
const char* Jugador::getCedula() const { return cedula; }
const char* Jugador::getPosicion() const { return posicion; }
int Jugador::getEdad() const { return edad; }
int Jugador::getNumeroDorsal() const { return numeroDorsal; }
int Jugador::getGolesAnotados() const { return golesAnotados; }
int Jugador::getTarjetasAmarillas() const { return tarjetasAmarillas; }
int Jugador::getTarjetasRojas() const { return tarjetasRojas; }
bool Jugador::getEliminado() const { return eliminado; }
time_t Jugador::getFechaCreacion() const { return fechaCreacion; }
time_t Jugador::getFechaUltimaModificacion() const { return fechaUltimaModificacion; }

// Setters
bool Jugador::setId(int id) {
    if (id < 0) return false;
    this->id = id;
    return true;
}

bool Jugador::setIdEquipo(int idEquipo) {
    if (idEquipo < 0) return false;
    this->idEquipo = idEquipo;
    return true;
}

bool Jugador::setNombre(const char* nombre) {
    if (nombre == nullptr || strlen(nombre) == 0) return false;
    Formatos::copiarCadena(this->nombre, nombre);
    return true;
}

bool Jugador::setCedula(const char* cedula) {
    if (!Validaciones::validarCedula(cedula)) return false;
    Formatos::copiarCadena(this->cedula, cedula);
    return true;
}

bool Jugador::setPosicion(const char* posicion) {
    if (!Validaciones::validarPosicion(posicion)) return false;
    Formatos::copiarCadena(this->posicion, posicion);
    return true;
}

bool Jugador::setEdad(int edad) {
    if (!Validaciones::validarEdad(edad)) return false;
    this->edad = edad;
    return true;
}

bool Jugador::setNumeroDorsal(int numeroDorsal) {
    if (!Validaciones::validarRango(numeroDorsal, 1, 99)) return false;
    this->numeroDorsal = numeroDorsal;
    return true;
}

bool Jugador::setGolesAnotados(int goles) {
    if (goles < 0) return false;
    golesAnotados = goles;
    return true;
}

bool Jugador::setTarjetasAmarillas(int tarjetas) {
    if (tarjetas < 0) return false;
    tarjetasAmarillas = tarjetas;
    return true;
}

bool Jugador::setTarjetasRojas(int tarjetas) {
    if (tarjetas < 0) return false;
    tarjetasRojas = tarjetas;
    return true;
}

bool Jugador::setEliminado(bool eliminado) {
    this->eliminado = eliminado;
    return true;
}

void Jugador::setFechaCreacion(time_t fecha) {
    fechaCreacion = fecha;
}

void Jugador::setFechaUltimaModificacion(time_t fecha) {
    fechaUltimaModificacion = fecha;
}

bool Jugador::esValido() const {
    return nombre[0] != '\0' && cedula[0] != '\0' &&
           posicion[0] != '\0' && edad > 0 && numeroDorsal > 0;
}

void Jugador::incrementarGoles() {
    golesAnotados++;
}

void Jugador::incrementarTarjetaAmarilla() {
    tarjetasAmarillas++;
}

void Jugador::incrementarTarjetaRoja() {
    tarjetasRojas++;
}

void Jugador::decrementarGoles() {
    if (golesAnotados > 0) golesAnotados--;
}

void Jugador::mostrarBasico() const {
    std::cout << "ID: " << id << " | " << nombre << " | Dorsal: " << numeroDorsal
              << " | Goles: " << golesAnotados << "\n";
}

void Jugador::mostrarCompleto() const {
    std::cout << "╔══════════════════════════════════════════════════════╗\n";
    std::cout << "║              DATOS DEL JUGADOR                       ║\n";
    std::cout << "╠══════════════════════════════════════════════════════╣\n";
    std::cout << "║  ID            : " << id << "\n";
    std::cout << "║  ID Equipo     : " << idEquipo << "\n";
    std::cout << "║  Nombre        : " << nombre << "\n";
    std::cout << "║  Cedula        : " << cedula << "\n";
    std::cout << "║  Posicion      : " << posicion << "\n";
    std::cout << "║  Edad          : " << edad << "\n";
    std::cout << "║  Dorsal        : " << numeroDorsal << "\n";
    std::cout << "║  Goles         : " << golesAnotados << "\n";
    std::cout << "║  Tar. Amarillas: " << tarjetasAmarillas << "\n";
    std::cout << "║  Tar. Rojas    : " << tarjetasRojas << "\n";
    std::cout << "║  Eliminado     : " << (eliminado ? "SI" : "NO") << "\n";
    std::cout << "╚══════════════════════════════════════════════════════╝\n";
}

size_t Jugador::obtenerTamano() {
    return sizeof(Jugador);
}