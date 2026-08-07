#ifndef OPERACIONES_PARTIDOS_HPP
#define OPERACIONES_PARTIDOS_HPP

#include "Partido.hpp"
#include "Gol.hpp"

class OperacionesPartidos {
private:
    static const char* RUTA_ARCHIVO;

public:
    static bool programarPartido(Partido& partido);
    static bool leerPartidoPorID(int id, Partido& resultado);
    static bool actualizarPartido(Partido& partido);
    static bool eliminarPartidoLogico(int id);
    static int listarPartidosPorEquipo(int idEquipo, Partido resultados[], int maxResultados);
    static int listarPartidosPorEstado(const char* estado, Partido resultados[], int maxResultados);
    static int contarPartidosActivos();
    static void listarPartidos();
    static bool registrarResultado(int idPartido, int golesLocal, int golesVisitante,
                                   Gol detalleGoles[], int numGoles);
    static bool cancelarPartidoJugado(int idPartido);
    static int obtenerTodosLosPartidos(Partido resultados[], int maxResultados);
};

#endif