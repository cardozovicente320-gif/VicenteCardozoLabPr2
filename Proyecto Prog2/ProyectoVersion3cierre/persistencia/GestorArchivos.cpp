#include "GestorArchivos.hpp"
#include <cstring>

// ============================================
// IMPLEMENTACIÓN DE MÉTODOS NO-TEMPLATE
// ============================================

GestorArchivos::ArchivoHeader GestorArchivos::leerHeader(const char* rutaArchivo) {
    ArchivoHeader h = {0, 1, 0, 1};
    std::ifstream archivo(rutaArchivo, std::ios::binary);
    if (archivo) {
        archivo.read(reinterpret_cast<char*>(&h), sizeof(ArchivoHeader));
        archivo.close();
    }
    return h;
}

bool GestorArchivos::actualizarHeader(const char* rutaArchivo, const ArchivoHeader& header) {
    std::fstream archivo(rutaArchivo, std::ios::in | std::ios::out | std::ios::binary);
    if (!archivo) return false;
    archivo.seekp(0, std::ios::beg);
    archivo.write(reinterpret_cast<const char*>(&header), sizeof(ArchivoHeader));
    archivo.close();
    return true;
}

bool GestorArchivos::decrementarActivos(const char* rutaArchivo) {
    ArchivoHeader h = leerHeader(rutaArchivo);
    if (h.registrosActivos > 0) {
        h.registrosActivos--;
    }
    return actualizarHeader(rutaArchivo, h);
}

bool GestorArchivos::incrementarActivos(const char* rutaArchivo) {
    ArchivoHeader h = leerHeader(rutaArchivo);
    h.registrosActivos++;
    return actualizarHeader(rutaArchivo, h);
}

bool GestorArchivos::inicializarArchivo(const char* rutaArchivo) {
    std::ifstream check(rutaArchivo, std::ios::binary);
    if (check.good()) {
        check.close();
        return true;
    }
    check.close();

    std::ofstream nuevo(rutaArchivo, std::ios::binary);
    if (!nuevo) return false;

    ArchivoHeader h;
    h.cantidadRegistros = 0;
    h.proximoID = 1;
    h.registrosActivos = 0;
    h.version = 1;
    nuevo.write(reinterpret_cast<const char*>(&h), sizeof(ArchivoHeader));
    nuevo.close();
    return true;
}