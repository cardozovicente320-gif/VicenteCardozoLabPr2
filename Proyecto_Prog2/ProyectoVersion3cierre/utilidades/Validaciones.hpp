#ifndef VALIDACIONES_HPP
#define VALIDACIONES_HPP

#include <cctype>
#include <cstring>

class Validaciones {
private:
    Validaciones() {}

public:
    static bool esNumero(const char* str);
    static bool validarFecha(const char* fecha);
    static bool validarRango(int valor, int min, int max);
    static bool validarCedula(const char* cedula);
    static bool validarEstadoPartido(const char* estado);
    static bool validarPosicion(const char* posicion);
    static bool validarMinuto(int minuto);
    static bool validarEquipoGol(const char* equipo);
    static bool validarEdad(int edad);
};

#endif