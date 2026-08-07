#include "Torneo.hpp"
#include "../utilidades/Formatos.hpp"
#include "../utilidades/Validaciones.hpp"
#include <iostream>
#include <cstring>

Torneo::Torneo() {
    nombre[0] = '\0';
    deporte[0] = '\0';
    formato[0] = '\0';
    fechaInicio[0] = '\0';
    fechaFin[0] = '\0';
    fechaCreacion = time(nullptr);
    fechaUltimaModificacion = time(nullptr);
}

Torneo::Torneo(const char* nombre, const char* deporte, const char* formato,
               const char* fechaInicio, const char* fechaFin) {
    setNombre(nombre);
    setDeporte(deporte);
    setFormato(formato);
    setFechaInicio(fechaInicio);
    setFechaFin(fechaFin);
    fechaCreacion = time(nullptr);
    fechaUltimaModificacion = time(nullptr);
}

Torneo::Torneo(const Torneo& other) {
    Formatos::copiarCadena(nombre, other.nombre);
    Formatos::copiarCadena(deporte, other.deporte);
    Formatos::copiarCadena(formato, other.formato);
    Formatos::copiarCadena(fechaInicio, other.fechaInicio);
    Formatos::copiarCadena(fechaFin, other.fechaFin);
    fechaCreacion = other.fechaCreacion;
    fechaUltimaModificacion = other.fechaUltimaModificacion;
}

Torneo::~Torneo() {}

const char* Torneo::getNombre() const { return nombre; }
const char* Torneo::getDeporte() const { return deporte; }
const char* Torneo::getFormato() const { return formato; }
const char* Torneo::getFechaInicio() const { return fechaInicio; }
const char* Torneo::getFechaFin() const { return fechaFin; }
time_t Torneo::getFechaCreacion() const { return fechaCreacion; }
time_t Torneo::getFechaUltimaModificacion() const { return fechaUltimaModificacion; }

bool Torneo::setNombre(const char* nombre) {
    if (nombre == nullptr || strlen(nombre) == 0) return false;
    Formatos::copiarCadena(this->nombre, nombre);
    return true;
}

bool Torneo::setDeporte(const char* deporte) {
    if (deporte == nullptr || strlen(deporte) == 0) return false;
    Formatos::copiarCadena(this->deporte, deporte);
    return true;
}

bool Torneo::setFormato(const char* formato) {
    if (formato == nullptr || strlen(formato) == 0) return false;
    Formatos::copiarCadena(this->formato, formato);
    return true;
}

bool Torneo::setFechaInicio(const char* fecha) {
    if (!Validaciones::validarFecha(fecha)) return false;
    Formatos::copiarCadena(fechaInicio, fecha);
    return true;
}

bool Torneo::setFechaFin(const char* fecha) {
    if (!Validaciones::validarFecha(fecha)) return false;
    Formatos::copiarCadena(fechaFin, fecha);
    return true;
}

void Torneo::setFechaUltimaModificacion(time_t fecha) {
    fechaUltimaModificacion = fecha;
}

bool Torneo::esValido() const {
    return nombre[0] != '\0' && deporte[0] != '\0' && 
           formato[0] != '\0' && fechaInicio[0] != '\0' && fechaFin[0] != '\0';
}

void Torneo::mostrarBasico() const {
    std::cout << "Torneo: " << nombre << " (" << deporte << ")\n";
}

void Torneo::mostrarCompleto() const {
    std::cout << "╔══════════════════════════════════════════════════════╗\n";
    std::cout << "║              DATOS DEL TORNEO                        ║\n";
    std::cout << "╠══════════════════════════════════════════════════════╣\n";
    std::cout << "║  Nombre     : " << nombre << "\n";
    std::cout << "║  Deporte    : " << deporte << "\n";
    std::cout << "║  Formato    : " << formato << "\n";
    std::cout << "║  Inicio     : " << fechaInicio << "\n";
    std::cout << "║  Fin        : " << fechaFin << "\n";
    std::cout << "║  Creación   : " << Formatos::timeTtoString(fechaCreacion) << "\n";
    std::cout << "║  Últ. Mod.  : " << Formatos::timeTtoString(fechaUltimaModificacion) << "\n";
    std::cout << "╚══════════════════════════════════════════════════════╝\n";
}

size_t Torneo::obtenerTamano() {
    return sizeof(Torneo);
}