#ifndef INTERFAZ_HPP
#define INTERFAZ_HPP

#include "../equipos/Equipo.hpp"
#include "../jugadores/Jugador.hpp"
#include "../partidos/Partido.hpp"
#include "../partidos/Gol.hpp"

class Interfaz {
private:
    bool ejecutando;
    
    // Menús principales
    void mostrarMenuPrincipal();
    void menuEquipos();
    void menuJugadores();
    void menuPartidos();
    void menuReportes();
    void menuMantenimiento();
    
    // Operaciones de Equipos
    void registrarEquipo();
    void buscarEquipo();
    void actualizarEquipo();
    void eliminarEquipo();
    void listarEquipos();
    
    // Operaciones de Jugadores
    void registrarJugador();
    void listarJugadores();
    
    // Operaciones de Partidos
    void programarPartido();
    void registrarResultado();
    void cancelarPartido();
    void listarPartidos();
    
    // Reportes
    void reportarTablaPosiciones();
    void reportarTablaGoleadores();
    void reportarFichaTecnica();
    
    // Mantenimiento
    void verificarIntegridad();
    void crearBackup();

public:
    Interfaz();
    ~Interfaz();
    void ejecutar();
    void salir();
};

#endif