#include "interfaz/Interfaz.hpp"
#include "persistencia/GestorArchivos.hpp"
#include "equipos/Equipo.hpp"
#include "jugadores/Jugador.hpp"
#include "partidos/Partido.hpp"
#include <iostream>

int main() {
    if (!GestorArchivos::inicializarArchivo("datos/equipos.bin") ||
        !GestorArchivos::inicializarArchivo("datos/jugadores.bin") ||
        !GestorArchivos::inicializarArchivo("datos/partidos.bin")) {
        std::cout << "ERROR al inicializar archivos del sistema.\n";
        return 1;
    }
    
    Interfaz interfaz;
    interfaz.ejecutar();
    
    return 0;
}
