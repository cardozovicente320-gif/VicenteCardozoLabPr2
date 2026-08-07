#ifndef GESTOR_ARCHIVOS_HPP
#define GESTOR_ARCHIVOS_HPP

#include <fstream>
#include <iostream>
#include <cstring>
#include <ctime>

class GestorArchivos {
public:
    struct ArchivoHeader {
        int cantidadRegistros;
        int proximoID;
        int registrosActivos;
        int version;
    };

    // Métodos no-template
    static ArchivoHeader leerHeader(const char* rutaArchivo);
    static bool actualizarHeader(const char* rutaArchivo, const ArchivoHeader& header);
    static bool decrementarActivos(const char* rutaArchivo);
    static bool incrementarActivos(const char* rutaArchivo);
    static bool inicializarArchivo(const char* rutaArchivo);

    // ============================================
    // MÉTODOS TEMPLATE
    // ============================================

    /**
     * Guarda un nuevo registro en el archivo.
     * El ID se asigna automáticamente desde el header.
     */
    template<typename T>
    static bool guardarRegistro(const char* rutaArchivo, T& entidad) {
        ArchivoHeader h = leerHeader(rutaArchivo);
        
        entidad.setId(h.proximoID);
        entidad.setEliminado(false);
        entidad.setFechaCreacion(time(nullptr));
        entidad.setFechaUltimaModificacion(time(nullptr));

        std::fstream archivo(rutaArchivo, std::ios::in | std::ios::out | std::ios::binary);
        if (!archivo) return false;

        archivo.seekp(sizeof(ArchivoHeader) + (h.cantidadRegistros * sizeof(T)), std::ios::beg);
        archivo.write(reinterpret_cast<const char*>(&entidad), sizeof(T));
        archivo.close();

        h.cantidadRegistros++;
        h.registrosActivos++;
        h.proximoID++;
        return actualizarHeader(rutaArchivo, h);
    }

    /**
     * Lee un registro por su ID.
     */
    template<typename T>
    static bool leerRegistroPorID(const char* rutaArchivo, int id, T& resultado) {
        int index = buscarIndicePorID<T>(rutaArchivo, id);
        if (index == -1) return false;

        std::ifstream archivo(rutaArchivo, std::ios::binary);
        if (!archivo) return false;

        archivo.seekg(sizeof(ArchivoHeader) + (index * sizeof(T)), std::ios::beg);
        archivo.read(reinterpret_cast<char*>(&resultado), sizeof(T));
        archivo.close();
        return true;
    }

    /**
     * Actualiza un registro existente.
     */
    template<typename T>
    static bool actualizarRegistro(const char* rutaArchivo, T& entidad) {
        int index = buscarIndicePorID<T>(rutaArchivo, entidad.getId());
        if (index == -1) return false;

        entidad.setFechaUltimaModificacion(time(nullptr));

        std::fstream archivo(rutaArchivo, std::ios::in | std::ios::out | std::ios::binary);
        if (!archivo) return false;

        archivo.seekp(sizeof(ArchivoHeader) + (index * sizeof(T)), std::ios::beg);
        archivo.write(reinterpret_cast<const char*>(&entidad), sizeof(T));
        archivo.close();
        return true;
    }

    /**
     * Elimina lógicamente un registro.
     */
    template<typename T>
    static bool eliminarRegistroLogico(const char* rutaArchivo, int id) {
        T entidad;
        if (!leerRegistroPorID<T>(rutaArchivo, id, entidad)) return false;
        
        entidad.setEliminado(true);
        if (!actualizarRegistro<T>(rutaArchivo, entidad)) return false;
        
        return decrementarActivos(rutaArchivo);
    }

private:
    /**
     * Busca el índice de un registro por su ID.
     */
    template<typename T>
    static int buscarIndicePorID(const char* rutaArchivo, int id) {
        std::ifstream archivo(rutaArchivo, std::ios::binary);
        if (!archivo) return -1;

        ArchivoHeader h;
        archivo.read(reinterpret_cast<char*>(&h), sizeof(ArchivoHeader));

        char* buffer = new char[sizeof(T)];
        int resultado = -1;

        for (int i = 0; i < h.cantidadRegistros; i++) {
            archivo.seekg(sizeof(ArchivoHeader) + (i * sizeof(T)), std::ios::beg);
            archivo.read(buffer, sizeof(T));
            
            T* ptrT = reinterpret_cast<T*>(buffer);
            
            if (ptrT->getId() == id && !ptrT->getEliminado()) {
                resultado = i;
                break;
            }
        }

        delete[] buffer;
        archivo.close();
        return resultado;
    }
};

#endif