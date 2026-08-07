#ifndef OPERACIONES_EQUIPOS_HPP
#define OPERACIONES_EQUIPOS_HPP

#include "Equipo.hpp"

class OperacionesEquipos {
private:
    static const char* RUTA_ARCHIVO;

public:
    static bool registrarEquipo(Equipo& equipo);
    static bool leerEquipoPorID(int id, Equipo& resultado);
    static bool actualizarEquipo(Equipo& equipo);
    static bool eliminarEquipoLogico(int id);
    static int buscarEquiposPorNombre(const char* subcadena, Equipo resultados[], int maxResultados);
    static int contarEquiposActivos();
    static void listarEquipos();
    static void ordenarEquipos(Equipo arr[], int tam);
    static int obtenerTodosLosEquipos(Equipo resultados[], int maxResultados);
};

#endif