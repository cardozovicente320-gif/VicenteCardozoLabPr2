#include "Formatos.hpp"
#include <cstring>
#include <sstream>
#include <iomanip>
#include <ctime>

void Formatos::limpiarBuffer() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

void Formatos::pausarEsperandoEnter() {
    std::cout << "Presione Enter para continuar...";
    std::cin.get();
}

bool Formatos::compararCadenas(const char* cad1, const char* cad2) {
    if (cad1 == nullptr || cad2 == nullptr) return false;
    while (*cad1 != '\0' && *cad2 != '\0') {
        if (*cad1 != *cad2) return false;
        cad1++;
        cad2++;
    }
    return *cad1 == *cad2;
}

char Formatos::aMinuscula(char c) {
    if (c >= 'A' && c <= 'Z') return c + 32;
    return c;
}

bool Formatos::contieneSubcadena(const char* cadena, const char* subcadena) {
    if (cadena == nullptr || subcadena == nullptr) return false;
    if (*subcadena == '\0') return true;
    for (const char* h = cadena; *h != '\0'; h++) {
        const char* h_temp = h;
        const char* n = subcadena;
        while (*h_temp != '\0' && *n != '\0' && aMinuscula(*h_temp) == aMinuscula(*n)) {
            h_temp++;
            n++;
        }
        if (*n == '\0') return true;
    }
    return false;
}

void Formatos::copiarCadena(char* destino, const char* origen) {
    if (origen == nullptr) {
        destino[0] = '\0';
        return;
    }
    while (*origen != '\0') {
        *destino = *origen;
        destino++;
        origen++;
    }
    *destino = '\0';
}

void Formatos::obtenerFechaHoy(char* buffer) {
    time_t t = time(nullptr);
    tm* now = localtime(&t);
    strftime(buffer, 11, "%Y-%m-%d", now);
}

std::string Formatos::timeTtoString(time_t t) {
    char buffer[26];
#ifdef _WIN32
    ctime_s(buffer, sizeof(buffer), &t);
#else
    ctime_r(&t, buffer);
#endif
    std::string result(buffer);
    if (!result.empty() && result[result.length() - 1] == '\n') {
        result.pop_back();
    }
    return result;
}

bool Formatos::solicitarConfirmacion(const char* mensaje) {
    char op;
    std::cout << mensaje << " (S/N): ";
    std::cin >> op;
    limpiarBuffer();
    return (op == 'S' || op == 's');
}

void Formatos::leerCadenaObligatoria(char* destino, int tamMax, const char* campo) {
    while (true) {
        std::cout << "Ingrese " << campo << ": ";
        std::cin.getline(destino, tamMax);
        if (destino[0] != '\0') break;
        std::cout << "ERROR: El campo no puede estar vacio.\n";
    }
}