#ifndef FORMATOS_HPP
#define FORMATOS_HPP

#include <ctime>
#include <iostream>
#include <limits>
#include <string>

class Formatos {
private:
    Formatos() {}

public:
    static void limpiarBuffer();
    static void pausarEsperandoEnter();
    static bool compararCadenas(const char* cad1, const char* cad2);
    static bool contieneSubcadena(const char* cadena, const char* subcadena);
    static char aMinuscula(char c);
    static void copiarCadena(char* destino, const char* origen);
    static void obtenerFechaHoy(char* buffer);
    static std::string timeTtoString(time_t t);
    static bool solicitarConfirmacion(const char* mensaje);
    static void leerCadenaObligatoria(char* destino, int tamMax, const char* campo);
};

#endif