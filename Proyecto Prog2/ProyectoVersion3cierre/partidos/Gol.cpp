#include "Gol.hpp"
#include "../utilidades/Formatos.hpp"
#include "../utilidades/Validaciones.hpp"
#include <iostream>
#include <cstring>

Gol::Gol() {
    idJugador = 0;
    minuto = 0;
    equipo[0] = '\0';
}

Gol::Gol(int idJugador, int minuto, const char* equipo) {
    setIdJugador(idJugador);
    setMinuto(minuto);
    setEquipo(equipo);
}

Gol::Gol(const Gol& other) {
    idJugador = other.idJugador;
    minuto = other.minuto;
    Formatos::copiarCadena(equipo, other.equipo);
}

Gol::~Gol() {}

int Gol::getIdJugador() const { return idJugador; }
int Gol::getMinuto() const { return minuto; }
const char* Gol::getEquipo() const { return equipo; }

bool Gol::setIdJugador(int id) {
    idJugador = id;
    return true;
}

bool Gol::setMinuto(int minuto) {
    if (!Validaciones::validarMinuto(minuto)) return false;
    this->minuto = minuto;
    return true;
}

bool Gol::setEquipo(const char* equipo) {
    if (!Validaciones::validarEquipoGol(equipo)) return false;
    Formatos::copiarCadena(this->equipo, equipo);
    return true;
}

bool Gol::esValido() const {
    return minuto >= 1 && minuto <= 120 &&
           equipo[0] != '\0' &&
           idJugador >= 0;
}

void Gol::mostrar() const {
    std::cout << "  Gol: Jugador ID " << idJugador
              << " | Minuto " << minuto
              << " | Equipo " << equipo << "\n";
}

size_t Gol::obtenerTamano() {
    return sizeof(Gol);
}