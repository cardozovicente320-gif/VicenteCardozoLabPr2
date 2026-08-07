#ifndef OPERACIONES_JUGADORES_HPP
#define OPERACIONES_JUGADORES_HPP

#include "Jugador.hpp"

class OperacionesJugadores {
private:
    static const char* RUTA_ARCHIVO;

public:
    static bool registrarJugador(Jugador& jugador);
    static bool leerJugadorPorID(int id, Jugador& resultado);
    static bool actualizarJugador(Jugador& jugador);
    static bool eliminarJugadorLogico(int id);
    static int listarJugadoresPorEquipo(int idEquipo, Jugador resultados[], int maxResultados);
    static int contarJugadoresActivos();
    static void listarJugadores();
    static bool existeCedula(const char* cedula);
    static int obtenerTodosLosJugadores(Jugador resultados[], int maxResultados);
};

#endif