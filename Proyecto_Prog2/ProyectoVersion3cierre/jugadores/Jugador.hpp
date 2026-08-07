#ifndef JUGADOR_HPP
#define JUGADOR_HPP

#include <ctime>

class Jugador {
private:
    int id;
    int idEquipo;
    char nombre[100];
    char cedula[20];
    char posicion[20];
    int edad;
    int numeroDorsal;
    int golesAnotados;
    int tarjetasAmarillas;
    int tarjetasRojas;
    bool eliminado;
    time_t fechaCreacion;
    time_t fechaUltimaModificacion;

public:
    Jugador();
    Jugador(int idEquipo, const char* nombre, const char* cedula,
            const char* posicion, int edad, int numeroDorsal);
    Jugador(const Jugador& other);
    ~Jugador();
    
    int getId() const;
    int getIdEquipo() const;
    const char* getNombre() const;
    const char* getCedula() const;
    const char* getPosicion() const;
    int getEdad() const;
    int getNumeroDorsal() const;
    int getGolesAnotados() const;
    int getTarjetasAmarillas() const;
    int getTarjetasRojas() const;
    bool getEliminado() const;
    time_t getFechaCreacion() const;
    time_t getFechaUltimaModificacion() const;
    
    bool setId(int id);
    bool setIdEquipo(int idEquipo);
    bool setNombre(const char* nombre);
    bool setCedula(const char* cedula);
    bool setPosicion(const char* posicion);
    bool setEdad(int edad);
    bool setNumeroDorsal(int numeroDorsal);
    bool setGolesAnotados(int goles);
    bool setTarjetasAmarillas(int tarjetas);
    bool setTarjetasRojas(int tarjetas);
    bool setEliminado(bool eliminado);
    void setFechaCreacion(time_t fecha);
    void setFechaUltimaModificacion(time_t fecha);
    
    bool esValido() const;
    void incrementarGoles();
    void incrementarTarjetaAmarilla();
    void incrementarTarjetaRoja();
    void decrementarGoles();
    void mostrarBasico() const;
    void mostrarCompleto() const;
    static size_t obtenerTamano();
};

#endif