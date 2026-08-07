#ifndef EQUIPO_HPP
#define EQUIPO_HPP

#include <ctime>

class Equipo {
private:
    int id;
    char nombre[100];
    char ciudad[100];
    char entrenador[100];
    int puntos;
    int victorias;
    int empates;
    int derrotas;
    int golesAFavor;
    int golesEnContra;
    int partidosIDs[50];
    int cantidadPartidos;
    bool eliminado;
    time_t fechaCreacion;
    time_t fechaUltimaModificacion;

public:
    Equipo();
    Equipo(const char* nombre, const char* ciudad, const char* entrenador);
    Equipo(const Equipo& other);
    ~Equipo();
    
    int getId() const;
    const char* getNombre() const;
    const char* getCiudad() const;
    const char* getEntrenador() const;
    int getPuntos() const;
    int getVictorias() const;
    int getEmpates() const;
    int getDerrotas() const;
    int getGolesAFavor() const;
    int getGolesEnContra() const;
    const int* getPartidosIDs() const;
    int getCantidadPartidos() const;
    bool getEliminado() const;
    time_t getFechaCreacion() const;
    time_t getFechaUltimaModificacion() const;
    
    bool setId(int id);
    bool setNombre(const char* nombre);
    bool setCiudad(const char* ciudad);
    bool setEntrenador(const char* entrenador);
    bool setPuntos(int puntos);
    bool setVictorias(int victorias);
    bool setEmpates(int empates);
    bool setDerrotas(int derrotas);
    bool setGolesAFavor(int goles);
    bool setGolesEnContra(int goles);
    bool agregarPartidoID(int idPartido);
    bool eliminarPartidoID(int idPartido);
    bool setEliminado(bool eliminado);
    void setFechaCreacion(time_t fecha);
    void setFechaUltimaModificacion(time_t fecha);
    
    bool esValido() const;
    void actualizarEstadisticas(int golesFavor, int golesContra, bool victoria, bool empate);
    void revertirEstadisticas(int golesFavor, int golesContra, bool victoria, bool empate);
    void mostrarBasico() const;
    void mostrarCompleto() const;
    static size_t obtenerTamano();
    
    bool operator<(const Equipo& other) const;
};

#endif