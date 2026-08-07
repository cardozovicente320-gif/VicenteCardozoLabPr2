#ifndef TORNEO_HPP
#define TORNEO_HPP

#include <ctime>

class Torneo {
private:
    char nombre[100];
    char deporte[50];
    char formato[20];
    char fechaInicio[11];
    char fechaFin[11];
    time_t fechaCreacion;
    time_t fechaUltimaModificacion;

public:
    Torneo();
    Torneo(const char* nombre, const char* deporte, const char* formato,
           const char* fechaInicio, const char* fechaFin);
    Torneo(const Torneo& other);
    ~Torneo();
    
    const char* getNombre() const;
    const char* getDeporte() const;
    const char* getFormato() const;
    const char* getFechaInicio() const;
    const char* getFechaFin() const;
    time_t getFechaCreacion() const;
    time_t getFechaUltimaModificacion() const;
    
    bool setNombre(const char* nombre);
    bool setDeporte(const char* deporte);
    bool setFormato(const char* formato);
    bool setFechaInicio(const char* fecha);
    bool setFechaFin(const char* fecha);
    void setFechaUltimaModificacion(time_t fecha);
    
    bool esValido() const;
    void mostrarBasico() const;
    void mostrarCompleto() const;
    static size_t obtenerTamano();
};

#endif