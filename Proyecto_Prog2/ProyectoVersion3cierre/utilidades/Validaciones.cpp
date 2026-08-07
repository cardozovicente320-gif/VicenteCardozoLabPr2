#include "Validaciones.hpp"

bool Validaciones::esNumero(const char* str) {
    if (str == nullptr || *str == '\0') return false;
    while (*str != '\0') {
        if (!std::isdigit(*str)) return false;
        str++;
    }
    return true;
}

bool Validaciones::validarFecha(const char* fecha) {
    if (fecha == nullptr || strlen(fecha) != 10) return false;
    if (fecha[4] != '-' || fecha[7] != '-') return false;
    
    for (int i = 0; i < 10; i++) {
        if (i == 4 || i == 7) continue;
        if (!std::isdigit(fecha[i])) return false;
    }
    return true;
}

bool Validaciones::validarRango(int valor, int min, int max) {
    return valor >= min && valor <= max;
}

bool Validaciones::validarCedula(const char* cedula) {
    if (cedula == nullptr || strlen(cedula) == 0) return false;
    return esNumero(cedula);
}

bool Validaciones::validarEstadoPartido(const char* estado) {
    if (estado == nullptr) return false;
    return (strcmp(estado, "PROGRAMADO") == 0 ||
            strcmp(estado, "JUGADO") == 0 ||
            strcmp(estado, "CANCELADO") == 0);
}

bool Validaciones::validarPosicion(const char* posicion) {
    if (posicion == nullptr || strlen(posicion) == 0) return false;
    return (strcmp(posicion, "PORTERO") == 0 ||
            strcmp(posicion, "DEFENSA") == 0 ||
            strcmp(posicion, "MEDIOCAMPISTA") == 0 ||
            strcmp(posicion, "DELANTERO") == 0);
}

bool Validaciones::validarMinuto(int minuto) {
    return minuto >= 1 && minuto <= 120;
}

bool Validaciones::validarEquipoGol(const char* equipo) {
    if (equipo == nullptr) return false;
    return (strcmp(equipo, "LOCAL") == 0 || strcmp(equipo, "VISITANTE") == 0);
}

bool Validaciones::validarEdad(int edad) {
    return edad >= 15 && edad <= 60;
}