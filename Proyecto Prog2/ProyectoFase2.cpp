#include <iostream>
#include <fstream>
#include <locale>
#include <limits>
#include <ctime>
#include <iomanip>

using namespace std;

// 1. Estructuras de datos adaptada para persistencia.

struct Torneo {
    char nombre[100];
    char deporte[50];
    char formato[20];
    char fechaInicio[11];
    char fechaFin[11];
    // Metadata de control
    time_t fechaCreacion;
    time_t fechaUltimaModificacion;
};

struct Equipo {
    // Datos básicos
    int  id;
    char nombre[100];
    char ciudad[100];
    char entrenador[100];

    // Estadísticas del torneo
    int  puntos;
    int  victorias;
    int  empates;
    int  derrotas;
    int  golesAFavor;
    int  golesEnContra;

    // Relaciones: IDs de partidos en que participó este equipo
    int  partidosIDs[50];       // Máximo 50 partidos por equipo
    int  cantidadPartidos;

    // Metadata de control
    bool     eliminado;
    time_t   fechaCreacion;
    time_t   fechaUltimaModificacion;
};

struct Jugador {
    // Datos básicos
    int  id;
    int  idEquipo;
    char nombre[100];
    char cedula[20];
    char posicion[20];
    int  edad;
    int  numeroDorsal;

    // Estadísticas individuales
    int  golesAnotados;
    int  tarjetasAmarillas;
    int  tarjetasRojas;

    // Metadata de control
    bool     eliminado;
    time_t   fechaCreacion;
    time_t   fechaUltimaModificacion;
};

struct Gol {
    int  idJugador;       // ID del jugador que anotó (0 = desconocido / gol en contra)
    int  minuto;          // Minuto del partido en que se anotó (1 - 120)
    char equipo[12];      // "LOCAL" o "VISITANTE"
};

struct Partido {
    // Datos básicos
    int  id;
    int  idEquipoLocal;
    int  idEquipoVisitante;
    char fecha[11];
    char estado[12];       // "PROGRAMADO", "JUGADO", "CANCELADO"
    char descripcion[200];

    // Marcador global
    int  golesLocal;
    int  golesVisitante;

    // Detalle de goles
    Gol  goles[22];        // Máximo 22 goles por partido
    int  numGoles;

    // Metadata de control
    bool     eliminado;
    time_t   fechaCreacion;
    time_t   fechaUltimaModificacion;
};

// Header obligatorio para archivos con múltiples registros
struct ArchivoHeader {
    int cantidadRegistros;  // Total histórico (incluyendo eliminados lógicamente)
    int proximoID;          // Siguiente ID a asignar (autoincremental)
    int registrosActivos;   // Registros con eliminado == false
    int version;            // Control de versión del archivo (iniciar en 1)
};

// Constantes de archivos
const char* FILE_TORNEO = "torneo.bin";
const char* FILE_EQUIPOS = "equipos.bin";
const char* FILE_JUGADORES = "jugadores.bin";
const char* FILE_PARTIDOS = "partidos.bin";

const int MAX_RESULTADOS = 100;


// 2. Funciones auxiliares de cadena y tiempo.


void copiarCadena(char* destino, const char* origen) {
    while (*origen != '\0') {
        *destino = *origen;
        destino++;
        origen++;
    }
    *destino = '\0';
}

bool compararCadenas(const char* cad1, const char* cad2) {
    while (*cad1 != '\0' && *cad2 != '\0') {
        if (*cad1 != *cad2) return false;
        cad1++;
        cad2++;
    }
    return *cad1 == *cad2;
}

char aMinuscula(char c) {
    if (c >= 'A' && c <= 'Z') return c + 32;
    return c;
}

bool contieneSubcadena(const char* cadena, const char* subcadena) {
    if (*subcadena == '\0') return true;
    for (const char* h = cadena; *h != '\0'; h++) {
        const char* h_temp = h;
        const char* n = subcadena;
        while (*h_temp != '\0' && *n != '\0' && aMinuscula(*h_temp) == aMinuscula(*n)) {
            h_temp++;
            n++;
        }
        if (*n == '\0') return true;
    }
    return false;
}

void obtenerFechaHoy(char* buffer) {
    time_t t = time(nullptr);
    tm* now = localtime(&t);
    strftime(buffer, 11, "%Y-%m-%d", now);
}


// 3. Capa de persistencia y controladores header.


bool inicializarArchivo(const char* nombreArchivo) {
    ifstream check(nombreArchivo, ios::binary);
    if (check.good()) {
        check.close();
        return true; // Ya existe
    }
    check.close();

    ofstream nuevo(nombreArchivo, ios::binary);
    if (!nuevo) return false;

    if (compararCadenas(nombreArchivo, FILE_TORNEO)) {
        // Torneo no lleva Header autoincremental por regla del enunciado
        Torneo t;
        copiarCadena(t.nombre, "Torneo de Verano 2026");
        copiarCadena(t.deporte, "Futbol");
        copiarCadena(t.formato, "ELIMINATORIA");
        copiarCadena(t.fechaInicio, "2026-06-01");
        copiarCadena(t.fechaFin, "2026-07-15");
        t.fechaCreacion = time(nullptr);
        t.fechaUltimaModificacion = time(nullptr);
        nuevo.write(reinterpret_cast<char*>(&t), sizeof(Torneo));
    } else {
        ArchivoHeader h;
        h.cantidadRegistros = 0;
        h.proximoID = 1;
        h.registrosActivos = 0;
        h.version = 1;
        nuevo.write(reinterpret_cast<char*>(&h), sizeof(ArchivoHeader));
    }
    nuevo.close();
    return true;
}

ArchivoHeader leerHeader(const char* nombreArchivo) {
    ArchivoHeader h = {0, 1, 0, 1};
    ifstream archivo(nombreArchivo, ios::binary);
    if (archivo) {
        archivo.read(reinterpret_cast<char*>(&h), sizeof(ArchivoHeader));
        archivo.close();
    }
    return h;
}

bool actualizarHeader(const char* nombreArchivo, ArchivoHeader header) {
    fstream archivo(nombreArchivo, ios::in | ios::out | ios::binary);
    if (!archivo) return false;
    archivo.seekp(0, ios::beg);
    archivo.write(reinterpret_cast<char*>(&header), sizeof(ArchivoHeader));
    archivo.close();
    return true;
}

int buscarIndicePorID(const char* nombreArchivo, int id, int tamanoRegistro) {
    ifstream archivo(nombreArchivo, ios::binary);
    if (!archivo) return -1;

    ArchivoHeader h;
    archivo.read(reinterpret_cast<char*>(&h), sizeof(ArchivoHeader));

    char* buffer = new char[tamanoRegistro];
    int resultado = -1;

    for (int i = 0; i < h.cantidadRegistros; i++) {
        archivo.seekg(sizeof(ArchivoHeader) + (i * tamanoRegistro), ios::beg);
        archivo.read(buffer, tamanoRegistro);
        
        // El ID está siempre en los primeros 4 bytes del struct de datos
        int registroId = *reinterpret_cast<int*>(buffer);
        // El booleano 'eliminado' varía según la estructura, pero se asume estructurado
        bool eliminado = false;
        
        if (compararCadenas(nombreArchivo, FILE_EQUIPOS)) {
            eliminado = reinterpret_cast<Equipo*>(buffer)->eliminado;
        } else if (compararCadenas(nombreArchivo, FILE_JUGADORES)) {
            eliminado = reinterpret_cast<Jugador*>(buffer)->eliminado;
        } else if (compararCadenas(nombreArchivo, FILE_PARTIDOS)) {
            eliminado = reinterpret_cast<Partido*>(buffer)->eliminado;
        }

        if (registroId == id && !eliminado) {
            resultado = i;
            break;
        }
    }

    delete[] buffer;
    archivo.close();
    return resultado;
}

bool inicializarSistemaArchivos() {
    return inicializarArchivo(FILE_TORNEO)    &&
           inicializarArchivo(FILE_EQUIPOS)   &&
           inicializarArchivo(FILE_JUGADORES) &&
           inicializarArchivo(FILE_PARTIDOS);
}


// 4. Funciones de persistencia  (CRUD)
//  Equipos

bool guardarEquipo(Equipo& equipo) {
    ArchivoHeader h = leerHeader(FILE_EQUIPOS);
    equipo.id = h.proximoID;
    equipo.eliminado = false;
    equipo.fechaCreacion = time(nullptr);
    equipo.fechaUltimaModificacion = time(nullptr);

    fstream archivo(FILE_EQUIPOS, ios::in | ios::out | ios::binary);
    if (!archivo) return false;

    // Posición al final: Header + i * sizeof(Equipo)
    archivo.seekp(sizeof(ArchivoHeader) + (h.cantidadRegistros * sizeof(Equipo)), ios::beg);
    archivo.write(reinterpret_cast<char*>(&equipo), sizeof(Equipo));
    archivo.close();

    h.cantidadRegistros++;
    h.registrosActivos++;
    h.proximoID++;
    return actualizarHeader(FILE_EQUIPOS, h);
}

bool leerEquipoPorID(int id, Equipo& resultado) {
    int index = buscarIndicePorID(FILE_EQUIPOS, id, sizeof(Equipo));
    if (index == -1) return false;

    ifstream archivo(FILE_EQUIPOS, ios::binary);
    if (!archivo) return false;

    archivo.seekg(sizeof(ArchivoHeader) + (index * sizeof(Equipo)), ios::beg);
    archivo.read(reinterpret_cast<char*>(&resultado), sizeof(Equipo));
    archivo.close();
    return true;
}

bool actualizarEquipo(Equipo& equipo) {
    int index = buscarIndicePorID(FILE_EQUIPOS, equipo.id, sizeof(Equipo));
    if (index == -1) return false;

    equipo.fechaUltimaModificacion = time(nullptr);

    fstream archivo(FILE_EQUIPOS, ios::in | ios::out | ios::binary);
    if (!archivo) return false;

    archivo.seekp(sizeof(ArchivoHeader) + (index * sizeof(Equipo)), ios::beg);
    archivo.write(reinterpret_cast<char*>(&equipo), sizeof(Equipo));
    archivo.close();
    return true;
}

bool eliminarEquipoLogico(int id) {
    Equipo eq;
    if (!leerEquipoPorID(id, eq)) return false;

    eq.eliminado = true;
    if (!actualizarEquipo(eq)) return false;

    ArchivoHeader h = leerHeader(FILE_EQUIPOS);
    h.registrosActivos--;
    return actualizarHeader(FILE_EQUIPOS, h);
}

int contarEquiposActivos() {
    return leerHeader(FILE_EQUIPOS).registrosActivos;
}

// Jugadores

bool guardarJugador(Jugador& jugador) {
    ArchivoHeader h = leerHeader(FILE_JUGADORES);
    jugador.id = h.proximoID;
    jugador.eliminado = false;
    jugador.fechaCreacion = time(nullptr);
    jugador.fechaUltimaModificacion = time(nullptr);

    fstream archivo(FILE_JUGADORES, ios::in | ios::out | ios::binary);
    if (!archivo) return false;

    archivo.seekp(sizeof(ArchivoHeader) + (h.cantidadRegistros * sizeof(Jugador)), ios::beg);
    archivo.write(reinterpret_cast<char*>(&jugador), sizeof(Jugador));
    archivo.close();

    h.cantidadRegistros++;
    h.registrosActivos++;
    h.proximoID++;
    return actualizarHeader(FILE_JUGADORES, h);
}

bool leerJugadorPorID(int id, Jugador& resultado) {
    int index = buscarIndicePorID(FILE_JUGADORES, id, sizeof(Jugador));
    if (index == -1) return false;

    ifstream archivo(FILE_JUGADORES, ios::binary);
    if (!archivo) return false;

    archivo.seekg(sizeof(ArchivoHeader) + (index * sizeof(Jugador)), ios::beg);
    archivo.read(reinterpret_cast<char*>(&resultado), sizeof(Jugador));
    archivo.close();
    return true;
}

bool actualizarJugador(Jugador& jugador) {
    int index = buscarIndicePorID(FILE_JUGADORES, jugador.id, sizeof(Jugador));
    if (index == -1) return false;

    jugador.fechaUltimaModificacion = time(nullptr);

    fstream archivo(FILE_JUGADORES, ios::in | ios::out | ios::binary);
    if (!archivo) return false;

    archivo.seekp(sizeof(ArchivoHeader) + (index * sizeof(Jugador)), ios::beg);
    archivo.write(reinterpret_cast<char*>(&jugador), sizeof(Jugador));
    archivo.close();
    return true;
}

bool eliminarJugadorLogico(int id) {
    Jugador jug;
    if (!leerJugadorPorID(id, jug)) return false;

    jug.eliminado = true;
    if (!actualizarJugador(jug)) return false;

    ArchivoHeader h = leerHeader(FILE_JUGADORES);
    h.registrosActivos--;
    return actualizarHeader(FILE_JUGADORES, h);
}

// Partidos

bool guardarPartido(Partido& partido) {
    ArchivoHeader h = leerHeader(FILE_PARTIDOS);
    partido.id = h.proximoID;
    partido.eliminado = false;
    partido.fechaCreacion = time(nullptr);
    partido.fechaUltimaModificacion = time(nullptr);

    fstream archivo(FILE_PARTIDOS, ios::in | ios::out | ios::binary);
    if (!archivo) return false;

    archivo.seekp(sizeof(ArchivoHeader) + (h.cantidadRegistros * sizeof(Partido)), ios::beg);
    archivo.write(reinterpret_cast<char*>(&partido), sizeof(Partido));
    archivo.close();

    h.cantidadRegistros++;
    h.registrosActivos++;
    h.proximoID++;
    return actualizarHeader(FILE_PARTIDOS, h);
}

bool leerPartidoPorID(int id, Partido& resultado) {
    int index = buscarIndicePorID(FILE_PARTIDOS, id, sizeof(Partido));
    if (index == -1) return false;

    ifstream archivo(FILE_PARTIDOS, ios::binary);
    if (!archivo) return false;

    archivo.seekg(sizeof(ArchivoHeader) + (index * sizeof(Partido)), ios::beg);
    archivo.read(reinterpret_cast<char*>(&resultado), sizeof(Partido));
    archivo.close();
    return true;
}

bool actualizarPartido(Partido& partido) {
    int index = buscarIndicePorID(FILE_PARTIDOS, partido.id, sizeof(Partido));
    if (index == -1) return false;

    partido.fechaUltimaModificacion = time(nullptr);

    fstream archivo(FILE_PARTIDOS, ios::in | ios::out | ios::binary);
    if (!archivo) return false;

    archivo.seekp(sizeof(ArchivoHeader) + (index * sizeof(Partido)), ios::beg);
    archivo.write(reinterpret_cast<char*>(&partido), sizeof(Partido));
    archivo.close();
    return true;
}


// 5. Logica de busqueda adaptada (Arrays temporales en vez de dinámicos).


int buscarEquiposPorNombre(const char* subcadena, Equipo resultados[], int maxResultados) {
    ifstream archivo(FILE_EQUIPOS, ios::binary);
    if (!archivo) return 0;

    ArchivoHeader h;
    archivo.read(reinterpret_cast<char*>(&h), sizeof(ArchivoHeader));

    int count = 0;
    for (int i = 0; i < h.cantidadRegistros && count < maxResultados; i++) {
        Equipo eq;
        archivo.read(reinterpret_cast<char*>(&eq), sizeof(Equipo));
        if (!eq.eliminado && contieneSubcadena(eq.nombre, subcadena)) {
            resultados[count++] = eq;
        }
    }
    archivo.close();
    return count;
}

int listarJugadoresPorEquipo(int idEquipo, Jugador resultados[], int maxResultados) {
    ifstream archivo(FILE_JUGADORES, ios::binary);
    if (!archivo) return 0;

    ArchivoHeader h;
    archivo.read(reinterpret_cast<char*>(&h), sizeof(ArchivoHeader));

    int count = 0;
    for (int i = 0; i < h.cantidadRegistros && count < maxResultados; i++) {
        Jugador jug;
        archivo.read(reinterpret_cast<char*>(&jug), sizeof(Jugador));
        if (!jug.eliminado && jug.idEquipo == idEquipo) {
            resultados[count++] = jug;
        }
    }
    archivo.close();
    return count;
}

int listarPartidosPorEquipo(int idEquipo, Partido resultados[], int maxResultados) {
    ifstream archivo(FILE_PARTIDOS, ios::binary);
    if (!archivo) return 0;

    ArchivoHeader h;
    archivo.read(reinterpret_cast<char*>(&h), sizeof(ArchivoHeader));

    int count = 0;
    for (int i = 0; i < h.cantidadRegistros && count < maxResultados; i++) {
        Partido p;
        archivo.read(reinterpret_cast<char*>(&p), sizeof(Partido));
        if (!p.eliminado && (p.idEquipoLocal == idEquipo || p.idEquipoVisitante == idEquipo)) {
            resultados[count++] = p;
        }
    }
    archivo.close();
    return count;
}

int listarPartidosPorEstado(const char* estado, Partido resultados[], int maxResultados) {
    ifstream archivo(FILE_PARTIDOS, ios::binary);
    if (!archivo) return 0;

    ArchivoHeader h;
    archivo.read(reinterpret_cast<char*>(&h), sizeof(ArchivoHeader));

    int count = 0;
    for (int i = 0; i < h.cantidadRegistros && count < maxResultados; i++) {
        Partido p;
        archivo.read(reinterpret_cast<char*>(&p), sizeof(Partido));
        if (!p.eliminado && compararCadenas(p.estado, estado)) {
            resultados[count++] = p;
        }
    }
    archivo.close();
    return count;
}


// 6. OPERACIONES COMPUESTAS (Registro y Reversión de Resultados).


bool registrarResultadoPartido(int idPartido, int golesLocal, int golesVisitante, Gol detalleGoles[], int numGoles) {
    Partido part;
    if (!leerPartidoPorID(idPartido, part)) return false;
    if (!compararCadenas(part.estado, "PROGRAMADO")) return false;

    Equipo local, visitante;
    if (!leerEquipoPorID(part.idEquipoLocal, local) || !leerEquipoPorID(part.idEquipoVisitante, visitante)) return false;

    // Actualizar goles del partido
    part.golesLocal = golesLocal;
    part.golesVisitante = golesVisitante;
    part.numGoles = numGoles;
    for (int i = 0; i < numGoles; i++) {
        part.goles[i] = detalleGoles[i];
    }
    copiarCadena(part.estado, "JUGADO");

    // Lógica de puntos
    if (golesLocal > golesVisitante) {
        local.puntos += 3; local.victorias++;
        visitante.derrotas++;
    } else if (golesLocal == golesVisitante) {
        local.puntos += 1; local.empates++;
        visitante.puntos += 1; visitante.empates++;
    } else {
        visitante.puntos += 3; visitante.victorias++;
        local.derrotas++;
    }

    local.golesAFavor += golesLocal;
    local.golesEnContra += golesVisitante;
    visitante.golesAFavor += golesVisitante;
    visitante.golesEnContra += golesLocal;

    // Asociar partido a los equipos
    if (local.cantidadPartidos < 50) local.partidosIDs[local.cantidadPartidos++] = idPartido;
    if (visitante.cantidadPartidos < 50) visitante.partidosIDs[visitante.cantidadPartidos++] = idPartido;

    // Actualizar goles anotados por jugadores
    for (int i = 0; i < numGoles; i++) {
        if (detalleGoles[i].idJugador > 0) {
            Jugador jug;
            if (leerJugadorPorID(detalleGoles[i].idJugador, jug)) {
                jug.golesAnotados++;
                actualizarJugador(jug);
            }
        }
    }

    // Persistir todos los cambios sincronizadamente
    return actualizarPartido(part) && actualizarEquipo(local) && actualizarEquipo(visitante);
}

bool cancelarPartidoJugado(int idPartido) {
    Partido part;
    if (!leerPartidoPorID(idPartido, part)) return false;
    if (!compararCadenas(part.estado, "JUGADO")) return false;

    Equipo local, visitante;
    if (!leerEquipoPorID(part.idEquipoLocal, local) || !leerEquipoPorID(part.idEquipoVisitante, visitante)) return false;

    // Deshacer estadísticas generales de equipos
    if (part.golesLocal > part.golesVisitante) {
        local.puntos -= 3; local.victorias--;
        visitante.derrotas--;
    } else if (part.golesLocal == part.golesVisitante) {
        local.puntos -= 1; local.empates--;
        visitante.puntos -= 1; visitante.empates--;
    } else {
        visitante.puntos -= 3; visitante.victorias--;
        local.derrotas--;
    }

    local.golesAFavor -= part.golesLocal;
    local.golesEnContra -= part.golesVisitante;
    visitante.golesAFavor -= part.golesVisitante;
    visitante.golesEnContra -= part.golesLocal;

    // Quitar ID de la lista de partidos de los equipos
    for (int i = 0; i < local.cantidadPartidos; i++) {
        if (local.partidosIDs[i] == idPartido) {
            for (int j = i; j < local.cantidadPartidos - 1; j++) local.partidosIDs[j] = local.partidosIDs[j+1];
            local.cantidadPartidos--;
            break;
        }
    }
    for (int i = 0; i < visitante.cantidadPartidos; i++) {
        if (visitante.partidosIDs[i] == idPartido) {
            for (int j = i; j < visitante.cantidadPartidos - 1; j++) visitante.partidosIDs[j] = visitante.partidosIDs[j+1];
            visitante.cantidadPartidos--;
            break;
        }
    }

    // Revertir goles individuales de jugadores
    for (int i = 0; i < part.numGoles; i++) {
        if (part.goles[i].idJugador > 0) {
            Jugador jug;
            if (leerJugadorPorID(part.goles[i].idJugador, jug)) {
                if (jug.golesAnotados > 0) jug.golesAnotados--;
                actualizarJugador(jug);
            }
        }
    }

    // Cambiar estado a CANCELADO y limpiar goles
    copiarCadena(part.estado, "CANCELADO");
    part.golesLocal = 0;
    part.golesVisitante = 0;
    part.numGoles = 0;

    return actualizarPartido(part) && actualizarEquipo(local) && actualizarEquipo(visitante);
}


// 7. Mantenimiento de integridad referencial y backups 


void verificarIntegridadReferencial() {
    int equiposVerificados = 0, jugadoresVerificados = 0, partidosVerificados = 0;
    int rotas = 0;

    cout << "\n--- CORRIENDO VERIFICACIÓN DE INTEGRIDAD REFERENCIAL ---\n";

    // a) Jugadores -> Equipos
    ifstream fJug(FILE_JUGADORES, ios::binary);
    if (fJug) {
        ArchivoHeader h; fJug.read(reinterpret_cast<char*>(&h), sizeof(ArchivoHeader));
        for (int i = 0; i < h.cantidadRegistros; i++) {
            Jugador j; fJug.read(reinterpret_cast<char*>(&j), sizeof(Jugador));
            if (!j.eliminado) {
                jugadoresVerificados++;
                Equipo eq;
                if (!leerEquipoPorID(j.idEquipo, eq)) {
                    cout << "ROTO: Jugador ID " << j.id << " (" << j.nombre << ") apunta a Equipo ID " << j.idEquipo << " (no existe)\n";
                    rotas++;
                }
            }
        }
        fJug.close();
    }

    // b) Partidos -> Equipos, Goles -> Jugadores
    ifstream fPart(FILE_PARTIDOS, ios::binary);
    if (fPart) {
        ArchivoHeader h; fPart.read(reinterpret_cast<char*>(&h), sizeof(ArchivoHeader));
        for (int i = 0; i < h.cantidadRegistros; i++) {
            Partido p; fPart.read(reinterpret_cast<char*>(&p), sizeof(Partido));
            if (!p.eliminado) {
                partidosVerificados++;
                Equipo loc, vis;
                if (!leerEquipoPorID(p.idEquipoLocal, loc)) {
                    cout << "ROTO: Partido ID " << p.id << " apunta a Equipo Local ID " << p.idEquipoLocal << " (no existe)\n";
                    rotas++;
                }
                if (!leerEquipoPorID(p.idEquipoVisitante, vis)) {
                    cout << "ROTO: Partido ID " << p.id << " apunta a Equipo Visitante ID " << p.idEquipoVisitante << " (no existe)\n";
                    rotas++;
                }

                // Goles individuales
                for (int g = 0; g < p.numGoles; g++) {
                    if (p.goles[g].idJugador > 0) {
                        Jugador jug;
                        if (!leerJugadorPorID(p.goles[g].idJugador, jug)) {
                            cout << "ROTO: Partido ID " << p.id << " registra Gol de Jugador ID " << p.goles[g].idJugador << " (no existe)\n";
                            rotas++;
                        }
                    }
                }
            }
        }
        fPart.close();
    }

    equiposVerificados = contarEquiposActivos();

    cout << "\n╔═══════════════════════════════════════════════════════╗\n";
    cout << "║         REPORTE DE INTEGRIDAD REFERENCIAL             ║\n";
    cout << "╠═══════════════════════════════════════════════════════╣\n";
    cout << "║  Equipos verificados   : " << setw(29) << left << equiposVerificados << "║\n";
    cout << "║  Jugadores verificados : " << setw(29) << left << jugadoresVerificados << "║\n";
    cout << "║  Partidos verificados  : " << setw(29) << left << partidosVerificados << "║\n";
    cout << "║                                                       ║\n";
    cout << "║  Referencias rotas encontradas: " << setw(22) << left << rotas << "║\n";
    if (rotas == 0) {
        cout << "║  Estado: ✓ SISTEMA ÍNTEGRO                            ║\n";
    } else {
        cout << "║  Estado: ⚠ ATENCIÓN REQUERIDA                         ║\n";
    }
    cout << "╚═══════════════════════════════════════════════════════╝\n";
}

bool crearBackup() {
    // Generar nombres estáticos de backup simulados con fecha de control
    const char* backupFiles[] = {"backup_equipos.bin", "backup_jugadores.bin", "backup_partidos.bin", "backup_torneo.bin"};
    const char* originalFiles[] = {FILE_EQUIPOS, FILE_JUGADORES, FILE_PARTIDOS, FILE_TORNEO};

    for (int i = 0; i < 4; i++) {
        ifstream src(originalFiles[i], ios::binary);
        ofstream dst(backupFiles[i], ios::binary);
        if (!src || !dst) return false;
        dst << src.rdbuf();
        src.close();
        dst.close();
    }
    return true;
}


// 8. Reporte de tabla de posiciones y goleadores. 


void ordenarEquiposPorBurbuja(Equipo arr[], int tam) {
    for (int i = 0; i < tam - 1; i++) {
        for (int j = 0; j < tam - i - 1; j++) {
            bool intercambiar = false;
            int difJ = arr[j].golesAFavor - arr[j].golesEnContra;
            int difSig = arr[j+1].golesAFavor - arr[j+1].golesEnContra;

            if (arr[j].puntos < arr[j+1].puntos) {
                intercambiar = true;
            } else if (arr[j].puntos == arr[j+1].puntos) {
                if (difJ < difSig) {
                    intercambiar = true;
                } else if (difJ == difSig) {
                    if (arr[j].golesAFavor < arr[j+1].golesAFavor) {
                        intercambiar = true;
                    }
                }
            }

            if (intercambiar) {
                Equipo temp = arr[j];
                arr[j] = arr[j+1];
                arr[j+1] = temp;
            }
        }
    }
}

void reportarTablaPosiciones() {
    ifstream archivo(FILE_EQUIPOS, ios::binary);
    if (!archivo) return;

    ArchivoHeader h;
    archivo.read(reinterpret_cast<char*>(&h), sizeof(ArchivoHeader));

    Equipo* tempArray = new Equipo[h.registrosActivos];
    int idx = 0;

    for (int i = 0; i < h.cantidadRegistros; i++) {
        Equipo eq;
        archivo.read(reinterpret_cast<char*>(&eq), sizeof(Equipo));
        if (!eq.eliminado) {
            tempArray[idx++] = eq;
        }
    }
    archivo.close();

    ordenarEquiposPorBurbuja(tempArray, idx);

    cout << "\n╔═════════════════════════════════════════════════════════════╗\n";
    cout << "║                     TABLA DE POSICIONES                     ║\n";
    cout << "╠════╦══════════════════════════╦═════╦═══╦═══╦═══╦═══╦════╦════╣\n";
    cout << "║ #  ║ Equipo                   ║ PTS ║ J ║ G ║ E ║ D ║ GF ║ GC ║\n";
    cout << "╠════╬══════════════════════════╬═════╬═══╬═══╬═══╬═══╬════╬════╣\n";

    for (int i = 0; i < idx; i++) {
        int jugados = tempArray[i].victorias + tempArray[i].empates + tempArray[i].derrotas;
        printf("║ %2d ║ %-24s ║ %3d ║ %1d ║ %1d ║ %1d ║ %1d ║ %2d ║ %2d ║\n",
               i + 1, tempArray[i].nombre, tempArray[i].puntos, jugados,
               tempArray[i].victorias, tempArray[i].empates, tempArray[i].derrotas,
               tempArray[i].golesAFavor, tempArray[i].golesEnContra);
    }
    cout << "╚════╩══════════════════════════╩═════╩═══╩═══╩═══╩═══╩════╩════╝\n";

    delete[] tempArray;
}

void reportarTablaGoleadores() {
    ifstream archivo(FILE_JUGADORES, ios::binary);
    if (!archivo) return;

    ArchivoHeader h;
    archivo.read(reinterpret_cast<char*>(&h), sizeof(ArchivoHeader));

    Jugador* tempArray = new Jugador[h.registrosActivos];
    int idx = 0;

    for (int i = 0; i < h.cantidadRegistros; i++) {
        Jugador j;
        archivo.read(reinterpret_cast<char*>(&j), sizeof(Jugador));
        if (!j.eliminado) {
            tempArray[idx++] = j;
        }
    }
    archivo.close();

    // Ordenar jugadores descendentemente por goles anotados
    for (int i = 0; i < idx - 1; i++) {
        for (int j = 0; j < idx - i - 1; j++) {
            if (tempArray[j].golesAnotados < tempArray[j+1].golesAnotados) {
                Jugador temp = tempArray[j];
                tempArray[j] = tempArray[j+1];
                tempArray[j+1] = temp;
            }
        }
    }

    cout << "\n╔══════════════════════════════════════════════════════╗\n";
    cout << "║               TABLA DE GOLEADORES                    ║\n";
    cout << "╠════╦══════════════════╦══════════════╦═══════╣\n";
    cout << "║ #  ║ Jugador          ║ Equipo       ║ Goles ║\n";
    cout << "╠════╬══════════════════╬══════════════╬═══════╣\n";

    int tops = idx < 10 ? idx : 10;
    for (int i = 0; i < tops; i++) {
        Equipo eq;
        const char* nombreEquipo = "N/A";
        if (leerEquipoPorID(tempArray[i].idEquipo, eq)) {
            nombreEquipo = eq.nombre;
        }
        printf("║ %2d ║ %-16s ║ %-12s ║   %2d  ║\n", i + 1, tempArray[i].nombre, nombreEquipo, tempArray[i].golesAnotados);
    }
    cout << "╚════╩══════════════════╩══════════════╩═══════╝\n";

    delete[] tempArray;
}

void reportarFichaTecnicaPartido(int idPartido) {
    Partido p;
    if (!leerPartidoPorID(idPartido, p)) {
        cout << "ERROR: No existe un partido con ID " << idPartido << "\n";
        return;
    }

    Equipo loc, vis;
    const char* nombreLocal = leerEquipoPorID(p.idEquipoLocal, loc) ? loc.nombre : "Desconocido";
    const char* nombreVisitante = leerEquipoPorID(p.idEquipoVisitante, vis) ? vis.nombre : "Desconocido";

    cout << "\n╔══════════════════════════════════════════════════════════╗\n";
    cout << "║              FICHA TÉCNICA DEL PARTIDO                   ║\n";
    cout << "╠══════════════════════════════════════════════════════════╣\n";
    printf("║  Partido ID  : %-10d Fecha: %-21s ║\n", p.id, p.fecha);
    printf("║  Estado      : %-40s ║\n", p.estado);
    cout << "╠══════════════════════════════════════════════════════════╣\n";
    printf("║  %-21s %2d  -  %-2d %-21s ║\n", nombreLocal, p.golesLocal, p.golesVisitante, nombreVisitante);
    cout << "╠══════════════════════════════════════════════════════════╣\n";
    cout << "║  GOLES:                                                  ║\n";
    for (int i = 0; i < p.numGoles; i++) {
        const char* nombreJugador = "Jugador desconocido";
        Jugador jug;
        if (p.goles[i].idJugador > 0 && leerJugadorPorID(p.goles[i].idJugador, jug)) {
            nombreJugador = jug.nombre;
        }
        printf("║  [%-10s] Min. %3d - %-32s ║\n", p.goles[i].equipo, p.goles[i].minuto, nombreJugador);
    }
    cout << "╚══════════════════════════════════════════════════════════╝\n";
}


// 9. CAPA DE PRESENTACIÓN / MENÚS DE CONSOLA


void limpiarBuffer() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

bool solicitarConfirmacion(const char* mensaje) {
    char op;
    cout << mensaje << " (S/N): ";
    cin >> op;
    limpiarBuffer();
    return (op == 'S' || op == 's');
}

void leerCadenaObligatoria(char* destino, int tamMax, const char* campo) {
    while (true) {
        cout << "Ingrese " << campo << ": ";
        cin.getline(destino, tamMax);
        if (destino[0] != '\0') break;
        cout << "ERROR: El campo no puede estar vacio.\n";
    }
}

// --- MENÚS CRUD DE EQUIPOS ---

void menuRegistrarEquipo() {
    cout << "\n--- REGISTRAR NUEVO EQUIPO ---\n";
    Equipo eq;
    leerCadenaObligatoria(eq.nombre, 100, "Nombre del Equipo");
    
    // Validar unicidad de nombre en disco
    Equipo control[100];
    int hallados = buscarEquiposPorNombre(eq.nombre, control, 100);
    for (int i = 0; i < hallados; i++) {
        if (compararCadenas(control[i].nombre, eq.nombre)) {
            cout << "ERROR: Ya existe un equipo con ese nombre.\n";
            return;
        }
    }

    leerCadenaObligatoria(eq.ciudad, 100, "Ciudad");
    leerCadenaObligatoria(eq.entrenador, 100, "Entrenador");
    
    eq.puntos = 0; eq.victorias = 0; eq.empates = 0; eq.derrotas = 0;
    eq.golesAFavor = 0; eq.golesEnContra = 0; eq.cantidadPartidos = 0;

    if (solicitarConfirmacion("¿Desea guardar este equipo?")) {
        if (guardarEquipo(eq)) {
            cout << "Equipo registrado con éxito con el ID: " << eq.id << "\n";
        }
    }
}

void menuBuscarEquipo() {
    cout << "\n--- BUSCAR EQUIPO ---\n";
    cout << "1. Buscar por ID\n2. Buscar por Nombre Parcial\nSeleccione: ";
    int op; cin >> op; limpiarBuffer();
    if (op == 1) {
        int id; cout << "Ingrese ID: "; cin >> id; limpiarBuffer();
        Equipo eq;
        if (leerEquipoPorID(id, eq)) {
            cout << "ID: " << eq.id << " | " << eq.nombre << " | " << eq.ciudad << " | DT: " << eq.entrenador << "\n";
            // Caso de uso 2: Debug offset
            cout << "[DEBUG OFFSET] Leido en byte: " << sizeof(ArchivoHeader) + ((id - 1) * sizeof(Equipo)) << "\n";
        } else {
            cout << "No encontrado.\n";
        }
    } else {
        char bus[100]; leerCadenaObligatoria(bus, 100, "nombre parcial");
        Equipo resultados[MAX_RESULTADOS];
        int n = buscarEquiposPorNombre(bus, resultados, MAX_RESULTADOS);
        for (int i = 0; i < n; i++) {
            cout << "ID: " << resultados[i].id << " | " << resultados[i].nombre << " | " << resultados[i].ciudad << "\n";
        }
    }
}

void menuActualizarEquipo() {
    cout << "\n--- ACTUALIZAR EQUIPO ---\n";
    int id; cout << "ID del equipo: "; cin >> id; limpiarBuffer();
    Equipo eq;
    if (!leerEquipoPorID(id, eq)) {
        cout << "No existe el equipo.\n"; return;
    }
    cout << "Actual: " << eq.nombre << "\n";
    leerCadenaObligatoria(eq.nombre, 100, "Nuevo Nombre");
    leerCadenaObligatoria(eq.ciudad, 100, "Nueva Ciudad");
    leerCadenaObligatoria(eq.entrenador, 100, "Nuevo Entrenador");
    if (actualizarEquipo(eq)) cout << "Actualizado.\n";
}

void menuEliminarEquipo() {
    cout << "\n--- ELIMINAR EQUIPO ---\n";
    int id; cout << "ID del equipo a eliminar: "; cin >> id; limpiarBuffer();
    if (eliminarEquipoLogico(id)) {
        cout << "Equipo eliminado (borrado logico realizado exitosamente).\n";
    } else {
        cout << "ERROR al eliminar.\n";
    }
}

void menuListarEquipos() {
    ifstream archivo(FILE_EQUIPOS, ios::binary);
    if (!archivo) return;
    ArchivoHeader h; archivo.read(reinterpret_cast<char*>(&h), sizeof(ArchivoHeader));
    cout << "\n--- LISTADO GENERAL DE EQUIPOS ---\n";
    for (int i = 0; i < h.cantidadRegistros; i++) {
        Equipo eq; archivo.read(reinterpret_cast<char*>(&eq), sizeof(Equipo));
        if (!eq.eliminado) {
            cout << "ID: " << eq.id << " | " << eq.nombre << " | " << eq.ciudad << " | Puntos: " << eq.puntos << "\n";
        }
    }
    archivo.close();
}

// --- MENÚS CRUD DE JUGADORES ---

void menuRegistrarJugador() {
    cout << "\n--- REGISTRAR NUEVO JUGADOR ---\n";
    Jugador j;
    cout << "ID de Equipo: "; cin >> j.idEquipo; limpiarBuffer();
    Equipo eq;
    if (!leerEquipoPorID(j.idEquipo, eq)) {
        cout << "ERROR: El equipo no existe o esta eliminado.\n"; return;
    }

    leerCadenaObligatoria(j.nombre, 100, "Nombre Completo");
    leerCadenaObligatoria(j.cedula, 20, "Cedula");
    
    // Validar cédula única en disco
    ifstream f(FILE_JUGADORES, ios::binary);
    if (f) {
        ArchivoHeader h; f.read(reinterpret_cast<char*>(&h), sizeof(ArchivoHeader));
        for (int i = 0; i < h.cantidadRegistros; i++) {
            Jugador temp; f.read(reinterpret_cast<char*>(&temp), sizeof(Jugador));
            if (!temp.eliminado && compararCadenas(temp.cedula, j.cedula)) {
                cout << "ERROR: La cédula ya está registrada.\n";
                f.close(); return;
            }
        }
        f.close();
    }

    leerCadenaObligatoria(j.posicion, 20, "Posicion (PORTERO, DEFENSA, etc.)");
    cout << "Edad: "; cin >> j.edad;
    cout << "Numero Dorsal: "; cin >> j.numeroDorsal; limpiarBuffer();

    j.golesAnotados = 0; j.tarjetasAmarillas = 0; j.tarjetasRojas = 0;

    if (guardarJugador(j)) {
        cout << "Jugador guardado exitosamente con ID: " << j.id << "\n";
    }
}

void menuListarJugadores() {
    ifstream f(FILE_JUGADORES, ios::binary);
    if (!f) return;
    ArchivoHeader h; f.read(reinterpret_cast<char*>(&h), sizeof(ArchivoHeader));
    cout << "\n--- LISTADO GENERAL DE JUGADORES ---\n";
    for (int i = 0; i < h.cantidadRegistros; i++) {
        Jugador j; f.read(reinterpret_cast<char*>(&j), sizeof(Jugador));
        if (!j.eliminado) {
            Equipo eq;
            const char* nEq = leerEquipoPorID(j.idEquipo, eq) ? eq.nombre : "N/A";
            cout << "ID: " << j.id << " | " << j.nombre << " | Equipo: " << nEq << " | Dorsal: " << j.numeroDorsal << "\n";
        }
    }
    f.close();
}

// --- MENÚS CRUD DE PARTIDOS ---

void menuProgramarPartido() {
    cout << "\n--- PROGRAMAR NUEVO PARTIDO ---\n";
    Partido p;
    cout << "ID Equipo Local: "; cin >> p.idEquipoLocal;
    cout << "ID Equipo Visitante: "; cin >> p.idEquipoVisitante; limpiarBuffer();

    if (p.idEquipoLocal == p.idEquipoVisitante) {
        cout << "ERROR: No puede ser el mismo equipo.\n"; return;
    }

    Equipo loc, vis;
    if (!leerEquipoPorID(p.idEquipoLocal, loc) || !leerEquipoPorID(p.idEquipoVisitante, vis)) {
        cout << "ERROR: Uno o ambos equipos no existen.\n"; return;
    }

    leerCadenaObligatoria(p.fecha, 11, "Fecha (AAAA-MM-DD)");
    leerCadenaObligatoria(p.descripcion, 200, "Descripción");

    copiarCadena(p.estado, "PROGRAMADO");
    p.golesLocal = 0; p.golesVisitante = 0; p.numGoles = 0;

    if (guardarPartido(p)) {
        cout << "Partido programado con éxito con ID: " << p.id << "\n";
    }
}

void menuRegistrarResultadoCompleto() {
    cout << "\n--- REGISTRAR RESULTADO DE PARTIDO ---\n";
    int idPartido; 
    cout << "ID del Partido: "; 
    cin >> idPartido; // <-- Corregido aquí
    limpiarBuffer();
    
    Partido p;
    if (!leerPartidoPorID(idPartido, p)) {
        cout << "No existe o ya no esta disponible.\n"; return;
    }

    int gl, gv;
    cout << "Goles Local: "; cin >> gl;
    cout << "Goles Visitante: "; cin >> gv;

    Gol detalle[22];
    int totalGoles = gl + gv;
    if (totalGoles > 22) totalGoles = 22; // Hard cap de goles según regla de la estructura

    for (int i = 0; i < totalGoles; i++) {
        cout << "\nDetalle del Gol #" << i+1 << ":\n";
        cout << "ID del Jugador anotador (0 si es desconocido/propia puerta): ";
        cin >> detalle[i].idJugador;
        cout << "Minuto (1-120): "; cin >> detalle[i].minuto;
        leerCadenaObligatoria(detalle[i].equipo, 12, "Equipo (LOCAL o VISITANTE)");
    }
    limpiarBuffer();

    if (registrarResultadoPartido(idPartido, gl, gv, detalle, totalGoles)) {
        cout << "Resultado guardado y estadísticas actualizadas en disco.\n";
    } else {
        cout << "ERROR al registrar el resultado.\n";
    }
}

void menuCancelarPartido() {
    cout << "\n--- REVERTIR/CANCELAR PARTIDO JUGADO ---\n";
    int id; cout << "ID del Partido a cancelar: "; cin >> id; limpiarBuffer();
    if (cancelarPartidoJugado(id)) {
        cout << "Partido revertido a CANCELADO y estadisticas de equipos/jugadores restauradas.\n";
    } else {
        cout << "ERROR: El partido no esta en estado JUGADO o no existe.\n";
    }
}

void menuListarPartidos() {
    ifstream f(FILE_PARTIDOS, ios::binary);
    if (!f) return;
    ArchivoHeader h; f.read(reinterpret_cast<char*>(&h), sizeof(ArchivoHeader));
    cout << "\n--- CRONOGRAMA GENERAL DE PARTIDOS ---\n";
    for (int i = 0; i < h.cantidadRegistros; i++) {
        Partido p; f.read(reinterpret_cast<char*>(&p), sizeof(Partido));
        if (!p.eliminado) {
            Equipo loc, vis;
            const char* nLoc = leerEquipoPorID(p.idEquipoLocal, loc) ? loc.nombre : "N/A";
            const char* nVis = leerEquipoPorID(p.idEquipoVisitante, vis) ? vis.nombre : "N/A";
            cout << "ID: " << p.id << " | " << nLoc << " " << p.golesLocal << " - " << p.golesVisitante << " " << nVis << " | Estado: " << p.estado << "\n";
        }
    }
    f.close();
}

// ============================================================================
// 10. MENÚ PRINCIPAL Y SUBMENÚS
// ============================================================================

int main(int argc, char* argv[]) {
    setlocale(LC_ALL, "spanish");

    if (!inicializarSistemaArchivos()) {
        cout << "ERROR critico al inicializar los archivos del sistema.\n";
        return 1;
    }

    int opPrincipal = -1;
    do {
        cout << "\n==============================================\n";
        cout << "         SISTEMA DE GESTION DE TORNEOS        \n";
        cout << "             Liga Apertura 2026               \n";
        cout << "==============================================\n";
        cout << "1. Gestion de Equipos\n";
        cout << "2. Gestion de Jugadores\n";
        cout << "3. Gestion de Partidos\n";
        cout << "4. Tabla de Posiciones\n";
        cout << "5. Reportes Analiticos\n";
        cout << "6. Mantenimiento del Sistema\n";
        cout << "0. Salir\n";
        cout << "Seleccione una opcion: ";
        if (!(cin >> opPrincipal)) { limpiarBuffer(); continue; }
        limpiarBuffer();

        switch (opPrincipal) {
            case 1: {
                int op = -1;
                do {
                    cout << "\n-- SUBMENU EQUIPOS --\n";
                    cout << "1. Registrar Equipo\n2. Buscar Equipo\n3. Actualizar Equipo\n4. Eliminar Equipo\n5. Listar Todos\n0. Volver\nOpcion: ";
                    if (cin >> op) {
                        limpiarBuffer();
                        if (op == 1) menuRegistrarEquipo();
                        else if (op == 2) menuBuscarEquipo();
                        else if (op == 3) menuActualizarEquipo();
                        else if (op == 4) menuEliminarEquipo();
                        else if (op == 5) menuListarEquipos();
                    } else { limpiarBuffer(); }
                } while (op != 0);
                break;
            }
            case 2: {
                int op = -1;
                do {
                    cout << "\n-- SUBMENU JUGADORES --\n";
                    cout << "1. Registrar Jugador\n2. Listar Todos\n0. Volver\nOpcion: ";
                    if (cin >> op) {
                        limpiarBuffer();
                        if (op == 1) menuRegistrarJugador();
                        else if (op == 2) menuListarJugadores();
                    } else { limpiarBuffer(); }
                } while (op != 0);
                break;
            }
            case 3: {
                int op = -1;
                do {
                    cout << "\n-- SUBMENU PARTIDOS --\n";
                    cout << "1. Programar Partido\n2. Registrar Resultado Completo\n3. Cancelar/Revertir Partido Jugado\n4. Listar Todos\n0. Volver\nOpcion: ";
                    if (cin >> op) {
                        limpiarBuffer();
                        if (op == 1) menuProgramarPartido();
                        else if (op == 2) menuRegistrarResultadoCompleto();
                        else if (op == 3) menuCancelarPartido();
                        else if (op == 4) menuListarPartidos();
                    } else { limpiarBuffer(); }
                } while (op != 0);
                break;
            }
            case 4:
                reportarTablaPosiciones();
                break;
            case 5: {
                int op = -1;
                do {
                    cout << "\n-- SUBMENU REPORTES --\n";
                    cout << "1. Tabla de posiciones\n2. Tabla de goleadores (Top 10)\n3. Ficha técnica de partido\n0. Volver\nOpcion: ";
                    if (cin >> op) {
                        limpiarBuffer();
                        if (op == 1) reportarTablaPosiciones();
                        else if (op == 2) reportarTablaGoleadores();
                        else if (op == 3) {
                            int idPart; cout << "Ingrese ID del Partido: "; cin >> idPart; limpiarBuffer();
                            reportarFichaTecnicaPartido(idPart);
                        }
                    } else { limpiarBuffer(); }
                } while (op != 0);
                break;
            }
            case 6: {
                int op = -1;
                do {
                    cout << "\n-- SUBMENU MANTENIMIENTO --\n";
                    cout << "1. Verificar integridad referencial\n2. Crear backup de datos\n0. Volver\nOpcion: ";
                    if (cin >> op) {
                        limpiarBuffer();
                        if (op == 1) verificarIntegridadReferencial();
                        else if (op == 2) {
                            if (crearBackup()) cout << "¡Backup creado con éxito! Los archivos se respaldaron en local.\n";
                            else cout << "Error al crear el Backup.\n";
                        }
                    } else { limpiarBuffer(); }
                } while (op != 0);
                break;
            }
            case 0:
                cout << "Guardando y cerrando sistema...\n";
                break;
            default:
                cout << "Opcion no valida.\n";
        }
    } while (opPrincipal != 0);

    return 0;
}