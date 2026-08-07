#ifndef PARTIDO_HPP
#define PARTIDO_HPP

#include "Gol.hpp"
#include <ctime>
#include <cstddef>

class Partido {
private:
    int id;
    int idEquipoLocal;
    int idEquipoVisitante;
    char fecha[11];
    char estado[12];
    char descripcion[200];
    int golesLocal;
    int golesVisitante;
    Gol goles[22];
    int numGoles;
    bool eliminado;
    time_t fechaCreacion;
    time_t fechaUltimaModificacion;

public:
    Partido();
    Partido(int idLocal, int idVisitante, const char* fecha, const char* descripcion);
    Partido(const Partido& other);
    ~Partido();
    
    int getId() const;
    int getIdEquipoLocal() const;
    int getIdEquipoVisitante() const;
    const char* getFecha() const;
    const char* getEstado() const;
    const char* getDescripcion() const;
    int getGolesLocal() const;
    int getGolesVisitante() const;
    const Gol* getGoles() const;
    int getNumGoles() const;
    bool getEliminado() const;
    time_t getFechaCreacion() const;
    time_t getFechaUltimaModificacion() const;
    
    bool setId(int id);
    bool setIdEquipoLocal(int id);
    bool setIdEquipoVisitante(int id);
    bool setFecha(const char* fecha);
    bool setEstado(const char* estado);
    bool setDescripcion(const char* descripcion);
    bool setGolesLocal(int goles);
    bool setGolesVisitante(int goles);
    bool agregarGol(const Gol& gol);
    bool setEliminado(bool eliminado);
    void setFechaCreacion(time_t fecha);
    void setFechaUltimaModificacion(time_t fecha);
    
    bool esValido() const;
    void limpiarGoles();
    void mostrarBasico() const;
    void mostrarCompleto() const;
    static size_t obtenerTamano();
};

#endif