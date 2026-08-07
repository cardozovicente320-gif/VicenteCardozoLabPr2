#include "Interfaz.hpp"
#include "../equipos/operacionesEquipos.hpp"
#include "../jugadores/operacionesJugadores.hpp"
#include "../partidos/operacionesPartidos.hpp"
#include "../utilidades/Formatos.hpp"
#include "../utilidades/Validaciones.hpp"
#include <iostream>
#include <iomanip>
#include <fstream>

const int MAX_RESULTADOS = 100;

Interfaz::Interfaz() : ejecutando(true) {}

Interfaz::~Interfaz() {}

void Interfaz::ejecutar() {
    int opcion;
    while (ejecutando) {
        mostrarMenuPrincipal();
        std::cin >> opcion;
        Formatos::limpiarBuffer();
        
        switch (opcion) {
            case 1: menuEquipos(); break;
            case 2: menuJugadores(); break;
            case 3: menuPartidos(); break;
            case 4: reportarTablaPosiciones(); break;
            case 5: menuReportes(); break;
            case 6: menuMantenimiento(); break;
            case 0: salir(); break;
            default: std::cout << "Opción no válida\n";
        }
    }
}

void Interfaz::salir() {
    std::cout << "Guardando y cerrando sistema...\n";
    ejecutando = false;
}

void Interfaz::mostrarMenuPrincipal() {
    std::cout << "\n==============================================\n";
    std::cout << "         SISTEMA DE GESTION DE TORNEOS        \n";
    std::cout << "==============================================\n";
    std::cout << "1. Gestion de Equipos\n";
    std::cout << "2. Gestion de Jugadores\n";
    std::cout << "3. Gestion de Partidos\n";
    std::cout << "4. Tabla de Posiciones\n";
    std::cout << "5. Reportes Analiticos\n";
    std::cout << "6. Mantenimiento del Sistema\n";
    std::cout << "0. Salir\n";
    std::cout << "Seleccione una opcion: ";
}

// ============================================
// MENÚ EQUIPOS
// ============================================

void Interfaz::menuEquipos() {
    int op = -1;
    do {
        std::cout << "\n-- SUBMENU EQUIPOS --\n";
        std::cout << "1. Registrar Equipo\n";
        std::cout << "2. Buscar Equipo\n";
        std::cout << "3. Actualizar Equipo\n";
        std::cout << "4. Eliminar Equipo\n";
        std::cout << "5. Listar Todos\n";
        std::cout << "0. Volver\n";
        std::cout << "Opcion: ";
        std::cin >> op;
        Formatos::limpiarBuffer();
        
        switch (op) {
            case 1: registrarEquipo(); break;
            case 2: buscarEquipo(); break;
            case 3: actualizarEquipo(); break;
            case 4: eliminarEquipo(); break;
            case 5: listarEquipos(); break;
            case 0: break;
            default: std::cout << "Opcion no valida.\n";
        }
    } while (op != 0);
}

void Interfaz::registrarEquipo() {
    std::cout << "\n--- REGISTRAR NUEVO EQUIPO ---\n";
    Equipo eq;
    
    char nombre[100], ciudad[100], entrenador[100];
    Formatos::leerCadenaObligatoria(nombre, 100, "Nombre del Equipo");
    Formatos::leerCadenaObligatoria(ciudad, 100, "Ciudad");
    Formatos::leerCadenaObligatoria(entrenador, 100, "Entrenador");
    
    eq.setNombre(nombre);
    eq.setCiudad(ciudad);
    eq.setEntrenador(entrenador);
    
    if (Formatos::solicitarConfirmacion("¿Desea guardar este equipo?")) {
        if (OperacionesEquipos::registrarEquipo(eq)) {
            std::cout << "Equipo registrado con éxito con el ID: " << eq.getId() << "\n";
        } else {
            std::cout << "ERROR: No se pudo registrar el equipo (posible nombre duplicado).\n";
        }
    }
}

void Interfaz::buscarEquipo() {
    std::cout << "\n--- BUSCAR EQUIPO ---\n";
    std::cout << "1. Buscar por ID\n";
    std::cout << "2. Buscar por Nombre Parcial\n";
    std::cout << "Seleccione: ";
    int op;
    std::cin >> op;
    Formatos::limpiarBuffer();
    
    if (op == 1) {
        int id;
        std::cout << "Ingrese ID: ";
        std::cin >> id;
        Formatos::limpiarBuffer();
        Equipo eq;
        if (OperacionesEquipos::leerEquipoPorID(id, eq)) {
            eq.mostrarCompleto();
        } else {
            std::cout << "No encontrado.\n";
        }
    } else if (op == 2) {
        char bus[100];
        Formatos::leerCadenaObligatoria(bus, 100, "nombre parcial");
        Equipo resultados[MAX_RESULTADOS];
        int n = OperacionesEquipos::buscarEquiposPorNombre(bus, resultados, MAX_RESULTADOS);
        for (int i = 0; i < n; i++) {
            resultados[i].mostrarBasico();
        }
        if (n == 0) std::cout << "No se encontraron equipos.\n";
    }
}

void Interfaz::actualizarEquipo() {
    std::cout << "\n--- ACTUALIZAR EQUIPO ---\n";
    int id;
    std::cout << "ID del equipo: ";
    std::cin >> id;
    Formatos::limpiarBuffer();
    
    Equipo eq;
    if (!OperacionesEquipos::leerEquipoPorID(id, eq)) {
        std::cout << "No existe el equipo.\n";
        return;
    }
    
    std::cout << "Actual: " << eq.getNombre() << "\n";
    char nombre[100], ciudad[100], entrenador[100];
    Formatos::leerCadenaObligatoria(nombre, 100, "Nuevo Nombre");
    Formatos::leerCadenaObligatoria(ciudad, 100, "Nueva Ciudad");
    Formatos::leerCadenaObligatoria(entrenador, 100, "Nuevo Entrenador");
    
    eq.setNombre(nombre);
    eq.setCiudad(ciudad);
    eq.setEntrenador(entrenador);
    
    if (OperacionesEquipos::actualizarEquipo(eq)) {
        std::cout << "Actualizado.\n";
    }
}

void Interfaz::eliminarEquipo() {
    std::cout << "\n--- ELIMINAR EQUIPO ---\n";
    int id;
    std::cout << "ID del equipo a eliminar: ";
    std::cin >> id;
    Formatos::limpiarBuffer();
    
    if (OperacionesEquipos::eliminarEquipoLogico(id)) {
        std::cout << "Equipo eliminado (borrado logico realizado exitosamente).\n";
    } else {
        std::cout << "ERROR al eliminar.\n";
    }
}

void Interfaz::listarEquipos() {
    OperacionesEquipos::listarEquipos();
}

// ============================================
// MENÚ JUGADORES
// ============================================

void Interfaz::menuJugadores() {
    int op = -1;
    do {
        std::cout << "\n-- SUBMENU JUGADORES --\n";
        std::cout << "1. Registrar Jugador\n";
        std::cout << "2. Listar Todos\n";
        std::cout << "0. Volver\n";
        std::cout << "Opcion: ";
        std::cin >> op;
        Formatos::limpiarBuffer();
        
        switch (op) {
            case 1: registrarJugador(); break;
            case 2: listarJugadores(); break;
            case 0: break;
            default: std::cout << "Opcion no valida.\n";
        }
    } while (op != 0);
}

void Interfaz::registrarJugador() {
    std::cout << "\n--- REGISTRAR NUEVO JUGADOR ---\n";
    Jugador j;
    
    int idEquipo;
    std::cout << "ID de Equipo: ";
    std::cin >> idEquipo;
    Formatos::limpiarBuffer();
    
    Equipo eq;
    if (!OperacionesEquipos::leerEquipoPorID(idEquipo, eq)) {
        std::cout << "ERROR: El equipo no existe o esta eliminado.\n";
        return;
    }
    j.setIdEquipo(idEquipo);
    
    char nombre[100], cedula[20], posicion[20];
    int edad, dorsal;
    
    Formatos::leerCadenaObligatoria(nombre, 100, "Nombre Completo");
    Formatos::leerCadenaObligatoria(cedula, 20, "Cedula");
    
    if (OperacionesJugadores::existeCedula(cedula)) {
        std::cout << "ERROR: La cédula ya está registrada.\n";
        return;
    }
    
    Formatos::leerCadenaObligatoria(posicion, 20, "Posicion (PORTERO, DEFENSA, MEDIOCAMPISTA, DELANTERO)");
    std::cout << "Edad: ";
    std::cin >> edad;
    std::cout << "Numero Dorsal: ";
    std::cin >> dorsal;
    Formatos::limpiarBuffer();
    
    j.setNombre(nombre);
    j.setCedula(cedula);
    j.setPosicion(posicion);
    j.setEdad(edad);
    j.setNumeroDorsal(dorsal);
    
    if (OperacionesJugadores::registrarJugador(j)) {
        std::cout << "Jugador guardado exitosamente con ID: " << j.getId() << "\n";
    } else {
        std::cout << "ERROR: No se pudo registrar el jugador.\n";
    }
}

void Interfaz::listarJugadores() {
    OperacionesJugadores::listarJugadores();
}

// ============================================
// MENÚ PARTIDOS
// ============================================

void Interfaz::menuPartidos() {
    int op = -1;
    do {
        std::cout << "\n-- SUBMENU PARTIDOS --\n";
        std::cout << "1. Programar Partido\n";
        std::cout << "2. Registrar Resultado Completo\n";
        std::cout << "3. Cancelar/Revertir Partido Jugado\n";
        std::cout << "4. Listar Todos\n";
        std::cout << "0. Volver\n";
        std::cout << "Opcion: ";
        std::cin >> op;
        Formatos::limpiarBuffer();
        
        switch (op) {
            case 1: programarPartido(); break;
            case 2: registrarResultado(); break;
            case 3: cancelarPartido(); break;
            case 4: listarPartidos(); break;
            case 0: break;
            default: std::cout << "Opcion no valida.\n";
        }
    } while (op != 0);
}

void Interfaz::programarPartido() {
    std::cout << "\n--- PROGRAMAR NUEVO PARTIDO ---\n";
    Partido p;
    
    int idLocal, idVisitante;
    std::cout << "ID Equipo Local: ";
    std::cin >> idLocal;
    std::cout << "ID Equipo Visitante: ";
    std::cin >> idVisitante;
    Formatos::limpiarBuffer();
    
    if (idLocal == idVisitante) {
        std::cout << "ERROR: No puede ser el mismo equipo.\n";
        return;
    }
    
    Equipo loc, vis;
    if (!OperacionesEquipos::leerEquipoPorID(idLocal, loc) || 
        !OperacionesEquipos::leerEquipoPorID(idVisitante, vis)) {
        std::cout << "ERROR: Uno o ambos equipos no existen.\n";
        return;
    }
    
    p.setIdEquipoLocal(idLocal);
    p.setIdEquipoVisitante(idVisitante);
    
    char fecha[11], descripcion[200];
    Formatos::leerCadenaObligatoria(fecha, 11, "Fecha (AAAA-MM-DD)");
    Formatos::leerCadenaObligatoria(descripcion, 200, "Descripción");
    
    p.setFecha(fecha);
    p.setDescripcion(descripcion);
    p.setEstado("PROGRAMADO");
    
    if (OperacionesPartidos::programarPartido(p)) {
        std::cout << "Partido programado con éxito con ID: " << p.getId() << "\n";
    } else {
        std::cout << "ERROR: No se pudo programar el partido.\n";
    }
}

void Interfaz::registrarResultado() {
    std::cout << "\n--- REGISTRAR RESULTADO DE PARTIDO ---\n";
    int idPartido;
    std::cout << "ID del Partido: ";
    std::cin >> idPartido;
    Formatos::limpiarBuffer();
    
    Partido p;
    if (!OperacionesPartidos::leerPartidoPorID(idPartido, p)) {
        std::cout << "No existe o ya no esta disponible.\n";
        return;
    }
    
    int gl, gv;
    std::cout << "Goles Local: ";
    std::cin >> gl;
    std::cout << "Goles Visitante: ";
    std::cin >> gv;
    Formatos::limpiarBuffer();
    
    Gol detalle[22];
    int totalGoles = gl + gv;
    if (totalGoles > 22) totalGoles = 22;
    
    for (int i = 0; i < totalGoles; i++) {
        std::cout << "\nDetalle del Gol #" << i+1 << ":\n";
        int idJugador, minuto;
        char equipo[12];
        
        std::cout << "ID del Jugador anotador (0 si es desconocido/propia puerta): ";
        std::cin >> idJugador;
        std::cout << "Minuto (1-120): ";
        std::cin >> minuto;
        Formatos::limpiarBuffer();
        Formatos::leerCadenaObligatoria(equipo, 12, "Equipo (LOCAL o VISITANTE)");
        
        detalle[i].setIdJugador(idJugador);
        detalle[i].setMinuto(minuto);
        detalle[i].setEquipo(equipo);
    }
    
    if (OperacionesPartidos::registrarResultado(idPartido, gl, gv, detalle, totalGoles)) {
        std::cout << "Resultado guardado y estadísticas actualizadas en disco.\n";
    } else {
        std::cout << "ERROR al registrar el resultado.\n";
    }
}

void Interfaz::cancelarPartido() {
    std::cout << "\n--- REVERTIR/CANCELAR PARTIDO JUGADO ---\n";
    int id;
    std::cout << "ID del Partido a cancelar: ";
    std::cin >> id;
    Formatos::limpiarBuffer();
    
    if (OperacionesPartidos::cancelarPartidoJugado(id)) {
        std::cout << "Partido revertido a CANCELADO y estadisticas restauradas.\n";
    } else {
        std::cout << "ERROR: El partido no esta en estado JUGADO o no existe.\n";
    }
}

void Interfaz::listarPartidos() {
    OperacionesPartidos::listarPartidos();
}

// ============================================
// REPORTES
// ============================================

void Interfaz::menuReportes() {
    int op = -1;
    do {
        std::cout << "\n-- SUBMENU REPORTES --\n";
        std::cout << "1. Tabla de posiciones\n";
        std::cout << "2. Tabla de goleadores (Top 10)\n";
        std::cout << "3. Ficha técnica de partido\n";
        std::cout << "0. Volver\n";
        std::cout << "Opcion: ";
        std::cin >> op;
        Formatos::limpiarBuffer();
        
        switch (op) {
            case 1: reportarTablaPosiciones(); break;
            case 2: reportarTablaGoleadores(); break;
            case 3: reportarFichaTecnica(); break;
            case 0: break;
            default: std::cout << "Opcion no valida.\n";
        }
    } while (op != 0);
}

void Interfaz::reportarTablaPosiciones() {
    Equipo equipos[MAX_RESULTADOS];
    int total = OperacionesEquipos::obtenerTodosLosEquipos(equipos, MAX_RESULTADOS);
    
    if (total == 0) {
        std::cout << "No hay equipos registrados.\n";
        return;
    }
    
    OperacionesEquipos::ordenarEquipos(equipos, total);
    
    std::cout << "\n╔═════════════════════════════════════════════════════════════╗\n";
    std::cout << "║                     TABLA DE POSICIONES                     ║\n";
    std::cout << "╠════╦══════════════════════════╦═════╦═══╦═══╦═══╦═══╦════╦════╣\n";
    std::cout << "║ #  ║ Equipo                   ║ PTS ║ J ║ G ║ E ║ D ║ GF ║ GC ║\n";
    std::cout << "╠════╬══════════════════════════╬═════╬═══╬═══╬═══╬═══╬════╬════╣\n";
    
    for (int i = 0; i < total; i++) {
        int jugados = equipos[i].getVictorias() + equipos[i].getEmpates() + equipos[i].getDerrotas();
        printf("║ %2d ║ %-24s ║ %3d ║ %1d ║ %1d ║ %1d ║ %1d ║ %2d ║ %2d ║\n",
               i + 1, equipos[i].getNombre(), equipos[i].getPuntos(), jugados,
               equipos[i].getVictorias(), equipos[i].getEmpates(), equipos[i].getDerrotas(),
               equipos[i].getGolesAFavor(), equipos[i].getGolesEnContra());
    }
    std::cout << "╚════╩══════════════════════════╩═════╩═══╩═══╩═══╩═══╩════╩════╝\n";
}

void Interfaz::reportarTablaGoleadores() {
    Jugador jugadores[MAX_RESULTADOS];
    int total = OperacionesJugadores::obtenerTodosLosJugadores(jugadores, MAX_RESULTADOS);
    
    if (total == 0) {
        std::cout << "No hay jugadores registrados.\n";
        return;
    }
    
    // Ordenar por goles (burbuja descendente)
    for (int i = 0; i < total - 1; i++) {
        for (int j = 0; j < total - i - 1; j++) {
            if (jugadores[j].getGolesAnotados() < jugadores[j+1].getGolesAnotados()) {
                Jugador temp = jugadores[j];
                jugadores[j] = jugadores[j+1];
                jugadores[j+1] = temp;
            }
        }
    }
    
    int tops = total < 10 ? total : 10;
    
    std::cout << "\n╔══════════════════════════════════════════════════════╗\n";
    std::cout << "║               TABLA DE GOLEADORES                    ║\n";
    std::cout << "╠════╦══════════════════╦══════════════╦═══════╣\n";
    std::cout << "║ #  ║ Jugador          ║ Equipo       ║ Goles ║\n";
    std::cout << "╠════╬══════════════════╬══════════════╬═══════╣\n";
    
    for (int i = 0; i < tops; i++) {
        Equipo eq;
        const char* nombreEquipo = "N/A";
        if (OperacionesEquipos::leerEquipoPorID(jugadores[i].getIdEquipo(), eq)) {
            nombreEquipo = eq.getNombre();
        }
        printf("║ %2d ║ %-16s ║ %-12s ║   %2d  ║\n", 
               i + 1, jugadores[i].getNombre(), nombreEquipo, jugadores[i].getGolesAnotados());
    }
    std::cout << "╚════╩══════════════════╩══════════════╩═══════╝\n";
}

void Interfaz::reportarFichaTecnica() {
    int idPartido;
    std::cout << "Ingrese ID del Partido: ";
    std::cin >> idPartido;
    Formatos::limpiarBuffer();
    
    Partido p;
    if (!OperacionesPartidos::leerPartidoPorID(idPartido, p)) {
        std::cout << "ERROR: No existe un partido con ID " << idPartido << "\n";
        return;
    }
    
    Equipo loc, vis;
    const char* nombreLocal = OperacionesEquipos::leerEquipoPorID(p.getIdEquipoLocal(), loc) ? loc.getNombre() : "Desconocido";
    const char* nombreVisitante = OperacionesEquipos::leerEquipoPorID(p.getIdEquipoVisitante(), vis) ? vis.getNombre() : "Desconocido";
    
    std::cout << "\n╔══════════════════════════════════════════════════════════╗\n";
    std::cout << "║              FICHA TÉCNICA DEL PARTIDO                   ║\n";
    std::cout << "╠══════════════════════════════════════════════════════════╣\n";
    printf("║  Partido ID  : %-10d Fecha: %-21s ║\n", p.getId(), p.getFecha());
    printf("║  Estado      : %-40s ║\n", p.getEstado());
    std::cout << "╠══════════════════════════════════════════════════════════╣\n";
    printf("║  %-21s %2d  -  %-2d %-21s ║\n", nombreLocal, p.getGolesLocal(), p.getGolesVisitante(), nombreVisitante);
    std::cout << "╠══════════════════════════════════════════════════════════╣\n";
    std::cout << "║  GOLES:                                                  ║\n";
    
    const Gol* goles = p.getGoles();
    for (int i = 0; i < p.getNumGoles(); i++) {
        const char* nombreJugador = "Jugador desconocido";
        Jugador jug;
        if (goles[i].getIdJugador() > 0 && OperacionesJugadores::leerJugadorPorID(goles[i].getIdJugador(), jug)) {
            nombreJugador = jug.getNombre();
        }
        printf("║  [%-10s] Min. %3d - %-32s ║\n", goles[i].getEquipo(), goles[i].getMinuto(), nombreJugador);
    }
    if (p.getNumGoles() == 0) {
        std::cout << "║  (Sin goles registrados)                                   ║\n";
    }
    std::cout << "╚══════════════════════════════════════════════════════════╝\n";
}

// ============================================
// MANTENIMIENTO
// ============================================

void Interfaz::menuMantenimiento() {
    int op = -1;
    do {
        std::cout << "\n-- SUBMENU MANTENIMIENTO --\n";
        std::cout << "1. Verificar integridad referencial\n";
        std::cout << "2. Crear backup de datos\n";
        std::cout << "0. Volver\n";
        std::cout << "Opcion: ";
        std::cin >> op;
        Formatos::limpiarBuffer();
        
        switch (op) {
            case 1: verificarIntegridad(); break;
            case 2: crearBackup(); break;
            case 0: break;
            default: std::cout << "Opcion no valida.\n";
        }
    } while (op != 0);
}

void Interfaz::verificarIntegridad() {
    int equiposVerificados = 0, jugadoresVerificados = 0, partidosVerificados = 0;
    int rotas = 0;
    
    std::cout << "\n--- CORRIENDO VERIFICACIÓN DE INTEGRIDAD REFERENCIAL ---\n";
    
    // Verificar Jugadores -> Equipos
    Jugador jugadores[MAX_RESULTADOS];
    int totalJugadores = OperacionesJugadores::obtenerTodosLosJugadores(jugadores, MAX_RESULTADOS);
    jugadoresVerificados = totalJugadores;
    
    for (int i = 0; i < totalJugadores; i++) {
        Equipo eq;
        if (!OperacionesEquipos::leerEquipoPorID(jugadores[i].getIdEquipo(), eq)) {
            std::cout << "ROTO: Jugador ID " << jugadores[i].getId() 
                      << " (" << jugadores[i].getNombre() 
                      << ") apunta a Equipo ID " << jugadores[i].getIdEquipo() 
                      << " (no existe)\n";
            rotas++;
        }
    }
    
    // Verificar Partidos -> Equipos y Goles -> Jugadores
    Partido partidos[MAX_RESULTADOS];
    int totalPartidos = OperacionesPartidos::obtenerTodosLosPartidos(partidos, MAX_RESULTADOS);
    partidosVerificados = totalPartidos;
    
    for (int i = 0; i < totalPartidos; i++) {
        Equipo loc, vis;
        if (!OperacionesEquipos::leerEquipoPorID(partidos[i].getIdEquipoLocal(), loc)) {
            std::cout << "ROTO: Partido ID " << partidos[i].getId() 
                      << " apunta a Equipo Local ID " << partidos[i].getIdEquipoLocal() 
                      << " (no existe)\n";
            rotas++;
        }
        if (!OperacionesEquipos::leerEquipoPorID(partidos[i].getIdEquipoVisitante(), vis)) {
            std::cout << "ROTO: Partido ID " << partidos[i].getId() 
                      << " apunta a Equipo Visitante ID " << partidos[i].getIdEquipoVisitante() 
                      << " (no existe)\n";
            rotas++;
        }
        
        const Gol* goles = partidos[i].getGoles();
        for (int g = 0; g < partidos[i].getNumGoles(); g++) {
            if (goles[g].getIdJugador() > 0) {
                Jugador jug;
                if (!OperacionesJugadores::leerJugadorPorID(goles[g].getIdJugador(), jug)) {
                    std::cout << "ROTO: Partido ID " << partidos[i].getId() 
                              << " registra Gol de Jugador ID " << goles[g].getIdJugador() 
                              << " (no existe)\n";
                    rotas++;
                }
            }
        }
    }
    
    equiposVerificados = OperacionesEquipos::contarEquiposActivos();
    
    std::cout << "\n╔═══════════════════════════════════════════════════════╗\n";
    std::cout << "║         REPORTE DE INTEGRIDAD REFERENCIAL             ║\n";
    std::cout << "╠═══════════════════════════════════════════════════════╣\n";
    std::cout << "║  Equipos verificados   : " << std::setw(29) << std::left << equiposVerificados << "║\n";
    std::cout << "║  Jugadores verificados : " << std::setw(29) << std::left << jugadoresVerificados << "║\n";
    std::cout << "║  Partidos verificados  : " << std::setw(29) << std::left << partidosVerificados << "║\n";
    std::cout << "║                                                       ║\n";
    std::cout << "║  Referencias rotas encontradas: " << std::setw(22) << std::left << rotas << "║\n";
    if (rotas == 0) {
        std::cout << "║  Estado: ✓ SISTEMA ÍNTEGRO                            ║\n";
    } else {
        std::cout << "║  Estado: ⚠ ATENCIÓN REQUERIDA                         ║\n";
    }
    std::cout << "╚═══════════════════════════════════════════════════════╝\n";
}

void Interfaz::crearBackup() {
    const char* backupFiles[] = {"backup_equipos.bin", "backup_jugadores.bin", "backup_partidos.bin"};
    const char* originalFiles[] = {"datos/equipos.bin", "datos/jugadores.bin", "datos/partidos.bin"};
    
    for (int i = 0; i < 3; i++) {
        std::ifstream src(originalFiles[i], std::ios::binary);
        std::ofstream dst(backupFiles[i], std::ios::binary);
        if (!src || !dst) {
            std::cout << "Error al crear el Backup.\n";
            return;
        }
        dst << src.rdbuf();
        src.close();
        dst.close();
    }
    std::cout << "¡Backup creado con éxito! Los archivos se respaldaron en local.\n";
}