#include <iostream>
#include <locale>
#include <limits>

using namespace std;


// 1. ESTRUCTURAS DE DATOS (Definicion de estructuras y tipos de datos)


struct Torneo {
    char nombre[100];
    char deporte[50]; 
    char formato[20]; // "GRUPOS" o "ELIMINATORIA"
    char fechaInicio[11];
    char fechaFin[11];
};

struct Equipo {
    int id;
    char nombre[100];
    char ciudad[100];
    char entrenador[100];
    int puntos;
    int victorias;
    int empates;
    int derrotas;
    int puntosAFavor;
    int puntosEnContra;
    char fechaRegistro[11];
};

struct Jugador {
    int id;
    int idEquipo;
    char nombre[100];
    char cedula[20];
    char posicion[20]; // "PORTERO", "DEFENSA", "MEDIOCAMPISTA", "DELANTERO"
    int edad;
    int numeroDorsal;
    char fechaRegistro[11];
};

struct Partido {
    int id;
    int idEquipoLocal;
    int idEquipoVisitante;
    int puntosLocal;
    int puntosVisitante;
    char fecha[11];
    char estado[12]; // "PROGRAMADO", "JUGADO", "CANCELADO" //Se refiere al estado del partido, no al resultado.
    char descripcion[200];
};

struct SistemaDeportivo {
    Torneo torneo;
    
    Equipo* equipos;
    int numEquipos;
    int capacidadEquipos;
    
    Jugador* jugadores;
    int numJugadores;
    int capacidadJugadores;
    
    Partido* partidos;
    int numPartidos;
    int capacidadPartidos;
    
    int siguienteIdEquipo;
    int siguienteIdJugador;
    int siguienteIdPartido;
};


//2. Funciones auxiliares de cadenas


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

 
//3.Capa de logica de negocio 

void inicializarSistema(SistemaDeportivo* s, Torneo torneo) {
    s->torneo = torneo;
    
    s->capacidadEquipos = 4;
    s->numEquipos = 0;
    s->equipos = new Equipo[s->capacidadEquipos];
    
    s->capacidadJugadores = 4;
    s->numJugadores = 0;
    s->jugadores = new Jugador[s->capacidadJugadores];
    
    s->capacidadPartidos = 4;
    s->numPartidos = 0;
    s->partidos = new Partido[s->capacidadPartidos];
    
    s->siguienteIdEquipo = 1;
    s->siguienteIdJugador = 1;
    s->siguienteIdPartido = 1;
}

void liberarSistema(SistemaDeportivo* s) {
    delete[] s->equipos;
    s->equipos = nullptr;
    
    delete[] s->jugadores;
    s->jugadores = nullptr;
    
    delete[] s->partidos;
    s->partidos = nullptr;
}

void redimensionarEquipos(SistemaDeportivo* s) {
    int nuevaCapacidad = s->capacidadEquipos * 2;
    Equipo* nuevoArray = new Equipo[nuevaCapacidad];
    for (int i = 0; i < s->numEquipos; i++) {
        nuevoArray[i] = s->equipos[i];
    }
    delete[] s->equipos;
    s->equipos = nuevoArray;
    s->capacidadEquipos = nuevaCapacidad;
}

void redimensionarJugadores(SistemaDeportivo* s) {
    int nuevaCapacidad = s->capacidadJugadores * 2;
    Jugador* nuevoArray = new Jugador[nuevaCapacidad];
    for (int i = 0; i < s->numJugadores; i++) {
        nuevoArray[i] = s->jugadores[i];
    }
    delete[] s->jugadores;
    s->jugadores = nuevoArray;
    s->capacidadJugadores = nuevaCapacidad;
}

void redimensionarPartidos(SistemaDeportivo* s) {
    int nuevaCapacidad = s->capacidadPartidos * 2;
    Partido* nuevoArray = new Partido[nuevaCapacidad];
    for (int i = 0; i < s->numPartidos; i++) {
        nuevoArray[i] = s->partidos[i];
    }
    delete[] s->partidos;
    s->partidos = nuevoArray;
    s->capacidadPartidos = nuevaCapacidad;
}

Equipo* buscarEquipoPorID(SistemaDeportivo* s, int id) {
    for (int i = 0; i < s->numEquipos; i++) {
        if (s->equipos[i].id == id) {
            return &s->equipos[i];
        }
    }
    return nullptr;
}

Equipo* agregarEquipo(SistemaDeportivo* s, const char* nombre, const char* ciudad, const char* entrenador) {
    for (int i = 0; i < s->numEquipos; i++) {
        if (compararCadenas(s->equipos[i].nombre, nombre)) {
            return nullptr; 
        }
    }
    
    if (s->numEquipos == s->capacidadEquipos) {
        redimensionarEquipos(s);
    }
    
    Equipo& nuevo = s->equipos[s->numEquipos];
    nuevo.id = s->siguienteIdEquipo++;
    copiarCadena(nuevo.nombre, nombre);
    copiarCadena(nuevo.ciudad, ciudad);
    copiarCadena(nuevo.entrenador, entrenador);
    nuevo.puntos = 0;
    nuevo.victorias = 0;
    nuevo.empates = 0;
    nuevo.derrotas = 0;
    nuevo.puntosAFavor = 0;
    nuevo.puntosEnContra = 0;
    copiarCadena(nuevo.fechaRegistro, "2026-06-06");
    
    s->numEquipos++;
    return &nuevo;
}

Equipo** buscarEquiposPorNombre(SistemaDeportivo* s, const char* subcadena, int* cantidad) {
    *cantidad = 0;
    for (int i = 0; i < s->numEquipos; i++) {
        if (contieneSubcadena(s->equipos[i].nombre, subcadena)) {
            (*cantidad)++;
        }
    }
    
    if (*cantidad == 0) return nullptr;
    
    Equipo** resultado = new Equipo*[*cantidad];
    int idx = 0;
    for (int i = 0; i < s->numEquipos; i++) {
        if (contieneSubcadena(s->equipos[i].nombre, subcadena)) {
            resultado[idx++] = &s->equipos[i];
        }
    }
    return resultado;
}

Equipo** listarEquipos(SistemaDeportivo* s, int* cantidad) {
    *cantidad = s->numEquipos;
    if (*cantidad == 0) return nullptr;
    
    Equipo** resultado = new Equipo*[*cantidad];
    for (int i = 0; i < s->numEquipos; i++) {
        resultado[i] = &s->equipos[i];
    }
    return resultado;
}

Equipo** generarTablaPosiciones(SistemaDeportivo* s, int* cantidad) {
    *cantidad = s->numEquipos;
    if (*cantidad == 0) return nullptr;
    
    Equipo** tabla = new Equipo*[*cantidad];
    for (int i = 0; i < s->numEquipos; i++) {
        tabla[i] = &s->equipos[i];
    }
    
    //4. Algoritmo de ordenación burbuja por punteros bajo los criterios del negocio
    for (int i = 0; i < *cantidad - 1; i++) {
        for (int j = 0; j < *cantidad - i - 1; j++) {
            bool intercambiar = false;
            int difJ = tabla[j]->puntosAFavor - tabla[j]->puntosEnContra;
            int difSig = tabla[j+1]->puntosAFavor - tabla[j+1]->puntosEnContra;
            
            if (tabla[j]->puntos < tabla[j+1]->puntos) {
                intercambiar = true;
            } else if (tabla[j]->puntos == tabla[j+1]->puntos) {
                if (difJ < difSig) {
                    intercambiar = true;
                } else if (difJ == difSig) {
                    if (tabla[j]->puntosAFavor < tabla[j+1]->puntosAFavor) {
                        intercambiar = true;
                    }
                }
            }
            
            if (intercambiar) {
                Equipo* temp = tabla[j];
                tabla[j] = tabla[j+1];
                tabla[j+1] = temp;
            }
        }
    }
    return tabla;
}

bool actualizarEquipo(SistemaDeportivo* s, int id, Equipo equipoActualizado) {
    Equipo* eq = buscarEquipoPorID(s, id);
    if (eq == nullptr) return false;
    
    copiarCadena(eq->nombre, equipoActualizado.nombre);
    copiarCadena(eq->ciudad, equipoActualizado.ciudad);
    copiarCadena(eq->entrenador, equipoActualizado.entrenador);
    return true;
}

bool eliminarEquipo(SistemaDeportivo* s, int id) {
    int idx = -1;
    for (int i = 0; i < s->numEquipos; i++) {
        if (s->equipos[i].id == id) {
            idx = i;
            break;
        }
    }
    if (idx == -1) return false;
    
    //Reordenamiento por desplazamiento para mantener la integridad física del array continuo
    for (int i = idx; i < s->numEquipos - 1; i++) {
        s->equipos[i] = s->equipos[i+1];
    }
    s->numEquipos--;
    return true;
}

int contarPartidosDeEquipo(SistemaDeportivo* s, int idEquipo) {
    int cont = 0;
    for (int i = 0; i < s->numPartidos; i++) {
        if (s->partidos[i].idEquipoLocal == idEquipo || s->partidos[i].idEquipoVisitante == idEquipo) {
            cont++;
        }
    }
    return cont;
}

Jugador* agregarJugador(SistemaDeportivo* s, int idEquipo, const char* nombre, const char* cedula, const char* posicion, int edad, int numeroDorsal) {
    if (buscarEquipoPorID(s, idEquipo) == nullptr) return nullptr;
    
    for (int i = 0; i < s->numJugadores; i++) {
        if (compararCadenas(s->jugadores[i].cedula, cedula)) return nullptr;
        if (s->jugadores[i].idEquipo == idEquipo && s->jugadores[i].numeroDorsal == numeroDorsal) return nullptr;
    }
    
    if (s->numJugadores == s->capacidadJugadores) {
        redimensionarJugadores(s);
    }
    
    Jugador& nuevo = s->jugadores[s->numJugadores];
    nuevo.id = s->siguienteIdJugador++;
    nuevo.idEquipo = idEquipo;
    copiarCadena(nuevo.nombre, nombre);
    copiarCadena(nuevo.cedula, cedula);
    copiarCadena(nuevo.posicion, posicion);
    nuevo.edad = edad;
    nuevo.numeroDorsal = numeroDorsal;
    copiarCadena(nuevo.fechaRegistro, "2026-06-06");
    
    s->numJugadores++;
    return &nuevo;
}

Jugador* buscarJugadorPorID(SistemaDeportivo* s, int id) {
    for (int i = 0; i < s->numJugadores; i++) {
        if (s->jugadores[i].id == id) return &s->jugadores[i];
    }
    return nullptr;
}

Jugador** buscarJugadoresPorNombre(SistemaDeportivo* s, const char* subcadena, int* cantidad) {
    *cantidad = 0;
    for (int i = 0; i < s->numJugadores; i++) {
        if (contieneSubcadena(s->jugadores[i].nombre, subcadena)) (*cantidad)++;
    }
    if (*cantidad == 0) return nullptr;
    
    Jugador** resultado = new Jugador*[*cantidad];
    int idx = 0;
    for (int i = 0; i < s->numJugadores; i++) {
        if (contieneSubcadena(s->jugadores[i].nombre, subcadena)) {
            resultado[idx++] = &s->jugadores[i];
        }
    }
    return resultado;
}

Jugador** listarJugadoresPorEquipo(SistemaDeportivo* s, int idEquipo, int* cantidad) {
    *cantidad = 0;
    for (int i = 0; i < s->numJugadores; i++) {
        if (s->jugadores[i].idEquipo == idEquipo) (*cantidad)++;
    }
    if (*cantidad == 0) return nullptr;
    
    Jugador** resultado = new Jugador*[*cantidad];
    int idx = 0;
    for (int i = 0; i < s->numJugadores; i++) {
        if (s->jugadores[i].idEquipo == idEquipo) {
            resultado[idx++] = &s->jugadores[i];
        }
    }
    return resultado;
}

Jugador** listarJugadores(SistemaDeportivo* s, int* cantidad) {
    *cantidad = s->numJugadores;
    if (*cantidad == 0) return nullptr;
    
    Jugador** resultado = new Jugador*[*cantidad];
    for (int i = 0; i < s->numJugadores; i++) {
        resultado[i] = &s->jugadores[i];
    }
    return resultado;
}

bool actualizarJugador(SistemaDeportivo* s, int id, Jugador jugadorActualizado) {
    Jugador* jug = buscarJugadorPorID(s, id);
    if (jug == nullptr) return false;
    
    copiarCadena(jug->nombre, jugadorActualizado.nombre);
    copiarCadena(jug->posicion, jugadorActualizado.posicion);
    jug->edad = jugadorActualizado.edad;
    jug->numeroDorsal = jugadorActualizado.numeroDorsal;
    return true;
}

bool eliminarJugador(SistemaDeportivo* s, int id) {
    int idx = -1;
    for (int i = 0; i < s->numJugadores; i++) {
        if (s->jugadores[i].id == id) {
            idx = i;
            break;
        }
    }
    if (idx == -1) return false;
    
    for (int i = idx; i < s->numJugadores - 1; i++) {
        s->jugadores[i] = s->jugadores[i+1];
    }
    s->numJugadores--;
    return true;
}

Partido* buscarPartidoPorID(SistemaDeportivo* s, int id) {
    for (int i = 0; i < s->numPartidos; i++) {
        if (s->partidos[i].id == id) return &s->partidos[i];
    }
    return nullptr;
}

Partido* programarPartido(SistemaDeportivo* s, int idLocal, int idVisitante, const char* fecha, const char* descripcion) {
    if (idLocal == idVisitante) return nullptr;
    if (buscarEquipoPorID(s, idLocal) == nullptr || buscarEquipoPorID(s, idVisitante) == nullptr) return nullptr;
    
    for (int i = 0; i < s->numPartidos; i++) {
        if (compararCadenas(s->partidos[i].estado, "PROGRAMADO")) {
            if ((s->partidos[i].idEquipoLocal == idLocal && s->partidos[i].idEquipoVisitante == idVisitante) ||
                (s->partidos[i].idEquipoLocal == idVisitante && s->partidos[i].idEquipoVisitante == idLocal)) {
                return nullptr;
            }
        }
    }
    
    if (s->numPartidos == s->capacidadPartidos) {
        redimensionarPartidos(s);
    }
    
    Partido& nuevo = s->partidos[s->numPartidos];
    nuevo.id = s->siguienteIdPartido++;
    nuevo.idEquipoLocal = idLocal;
    nuevo.idEquipoVisitante = idVisitante;
    nuevo.puntosLocal = 0;
    nuevo.puntosVisitante = 0;
    copiarCadena(nuevo.fecha, fecha);
    copiarCadena(nuevo.estado, "PROGRAMADO");
    copiarCadena(nuevo.descripcion, descripcion);
    
    s->numPartidos++;
    return &nuevo;
}

void alterarEstadisticasEquipo(Equipo* eq, int puntosObtenidos, int victor, int empat, int derrot, int fav, int contra) {
    if (eq == nullptr) return;
    eq->puntos += puntosObtenidos;
    eq->victorias += victor;
    eq->empates += empat;
    eq->derrotas += derrot;
    eq->puntosAFavor += fav;
    eq->puntosEnContra += contra;
}

Partido* registrarResultado(SistemaDeportivo* s, int idPartido, int puntosLocal, int puntosVisitante) {
    Partido* part = buscarPartidoPorID(s, idPartido);
    if (part == nullptr || !compararCadenas(part->estado, "PROGRAMADO")) return nullptr;
    
    part->puntosLocal = puntosLocal;
    part->puntosVisitante = puntosVisitante;
    copiarCadena(part->estado, "JUGADO");
    
    Equipo* local = buscarEquipoPorID(s, part->idEquipoLocal);
    Equipo* visitante = buscarEquipoPorID(s, part->idEquipoVisitante);
    
    if (puntosLocal > puntosVisitante) {
        alterarEstadisticasEquipo(local, 3, 1, 0, 0, puntosLocal, puntosVisitante);
        alterarEstadisticasEquipo(visitante, 0, 0, 0, 1, puntosVisitante, puntosLocal);
    } else if (puntosLocal == puntosVisitante) {
        alterarEstadisticasEquipo(local, 1, 0, 1, 0, puntosLocal, puntosVisitante);
        alterarEstadisticasEquipo(visitante, 1, 0, 1, 0, puntosVisitante, puntosLocal);
    } else {
        alterarEstadisticasEquipo(local, 0, 0, 0, 1, puntosLocal, puntosVisitante);
        alterarEstadisticasEquipo(visitante, 3, 1, 0, 0, puntosVisitante, puntosLocal);
    }
    
    return part;
}

Partido** buscarPartidosPorEquipo(SistemaDeportivo* s, int idEquipo, int* cantidad) {
    *cantidad = 0;
    for (int i = 0; i < s->numPartidos; i++) {
        if (s->partidos[i].idEquipoLocal == idEquipo || s->partidos[i].idEquipoVisitante == idEquipo) {
            (*cantidad)++;
        }
    }
    if (*cantidad == 0) return nullptr;
    
    Partido** resultado = new Partido*[*cantidad];
    int idx = 0;
    for (int i = 0; i < s->numPartidos; i++) {
        if (s->partidos[i].idEquipoLocal == idEquipo || s->partidos[i].idEquipoVisitante == idEquipo) {
            resultado[idx++] = &s->partidos[i];
        }
    }
    return resultado;
}

Partido** listarPartidosPorEstado(SistemaDeportivo* s, const char* estado, int* cantidad) {
    *cantidad = 0;
    for (int i = 0; i < s->numPartidos; i++) {
        if (compararCadenas(s->partidos[i].estado, estado)) (*cantidad)++;
    }
    if (*cantidad == 0) return nullptr;
    
    Partido** resultado = new Partido*[*cantidad];
    int idx = 0;
    for (int i = 0; i < s->numPartidos; i++) {
        if (compararCadenas(s->partidos[i].estado, estado)) {
            resultado[idx++] = &s->partidos[i];
        }
    }
    return resultado;
}

Partido** listarPartidos(SistemaDeportivo* s, int* cantidad) {
    *cantidad = s->numPartidos;
    if (*cantidad == 0) return nullptr;
    
    Partido** resultado = new Partido*[*cantidad];
    for (int i = 0; i < s->numPartidos; i++) {
        resultado[i] = &s->partidos[i];
    }
    return resultado;
}

bool cancelarPartido(SistemaDeportivo* s, int idPartido) {
    Partido* part = buscarPartidoPorID(s, idPartido);
    if (part == nullptr || compararCadenas(part->estado, "CANCELADO")) return false;
    
    if (compararCadenas(part->estado, "JUGADO")) {
        Equipo* local = buscarEquipoPorID(s, part->idEquipoLocal);
        Equipo* visitante = buscarEquipoPorID(s, part->idEquipoVisitante);
        
        if (part->puntosLocal > part->puntosVisitante) {
            alterarEstadisticasEquipo(local, -3, -1, 0, 0, -part->puntosLocal, -part->puntosVisitante);
            alterarEstadisticasEquipo(visitante, 0, 0, 0, -1, -part->puntosVisitante, -part->puntosLocal);
        } else if (part->puntosLocal == part->puntosVisitante) {
            alterarEstadisticasEquipo(local, -1, 0, -1, 0, -part->puntosLocal, -part->puntosVisitante);
            // AQUÍ ESTABA EL ERROR: Cambiado -part->pLocal por -part->puntosLocal
            alterarEstadisticasEquipo(visitante, -1, 0, -1, 0, -part->puntosVisitante, -part->puntosLocal); 
        } else {
            alterarEstadisticasEquipo(local, 0, 0, 0, -1, -part->puntosLocal, -part->puntosVisitante);
            alterarEstadisticasEquipo(visitante, -3, -1, 0, 0, -part->puntosVisitante, -part->puntosLocal);
        }
    }
    
    copiarCadena(part->estado, "CANCELADO");
    return true;
}

// Capa de presentacion 


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

void leerCadenaOBligatoria(char* destino, int tamMax, const char* campo) {
    while (true) {
        cout << "Ingrese " << campo << ": ";
        cin.getline(destino, tamMax);
        if (destino[0] != '\0') break;
        cout << "ERROR: El campo no puede estar vacio.\n";
    }
}

void mostrarEquipo(Equipo* e) {
    if (e == nullptr) return;
    cout << "ID: " << e->id << " | Nombre: " << e->nombre << " | Ciudad: " << e->ciudad 
         << " | Entrenador: " << e->entrenador << " | PTS: " << e->puntos << "\n";
}

void mostrarListaEquipos(Equipo** equipos, int cantidad) {
    cout << "----------------------------------------------------------------------\n";
    for (int i = 0; i < cantidad; i++) {
        mostrarEquipo(equipos[i]);
    }
    cout << "----------------------------------------------------------------------\n";
}

void menuRegistrarEquipo(SistemaDeportivo* s) {
    cout << "\n--- REGISTRAR NUEVO EQUIPO ---\n";
    char nombre[100], ciudad[100], entrenador[100];
    
    leerCadenaOBligatoria(nombre, 100, "Nombre del Equipo");
    
    // Verificación temprana en presentación para evitar recolección innecesaria
    int cantTemp = 0;
    Equipo** test = buscarEquiposPorNombre(s, nombre, &cantTemp);
    if (test != nullptr) {
        for (int i = 0; i < cantTemp; i++) {
            if (compararCadenas(test[i]->nombre, nombre)) {
                cout << "ERROR: Ya existe un equipo con el nombre '" << nombre << "'.\n";
                delete[] test;
                return;
            }
        }
        delete[] test;
    }
    
    leerCadenaOBligatoria(ciudad, 100, "Ciudad");
    if (compararCadenas(ciudad, "CANCELAR")) { cout << "Operacion cancelada.\n"; return; }
    
    leerCadenaOBligatoria(entrenador, 100, "Entrenador");
    
    if (solicitarConfirmacion("¿Desea guardar este equipo?")) {
        Equipo* eq = agregarEquipo(s, nombre, ciudad, entrenador);
        if (eq != nullptr) {
            cout << "Equipo registrado con exito.\n";
            mostrarEquipo(eq);
        } else {
            cout << "ERROR: No se pudo registrar el equipo.\n";
        }
    } else {
        cout << "Registro cancelado por el usuario.\n";
    }
}

void menuBuscarEquipo(SistemaDeportivo* s) {
    cout << "\n--- BUSCAR EQUIPO ---\n";
    cout << "1. Buscar por ID\n2. Buscar por Nombre Parcial\nSeleccione: ";
    int op;
    if (!(cin >> op)) { limpiarBuffer(); return; }
    limpiarBuffer();
    
    if (op == 1) {
        int id;
        cout << "Ingrese ID del equipo: ";
        if (!(cin >> id)) { limpiarBuffer(); return; }
        limpiarBuffer();
        Equipo* e = buscarEquipoPorID(s, id);
        if (e != nullptr) mostrarEquipo(e);
        else cout << "ERROR: No existe ningun equipo con ID " << id << ".\n";
    } else if (op == 2) {
        char sub[100];
        leerCadenaOBligatoria(sub, 100, "nombre parcial");
        int cant = 0;
        Equipo** res = buscarEquiposPorNombre(s, sub, &cant);
        if (res != nullptr) {
            mostrarListaEquipos(res, cant);
            delete[] res;
        } else {
            cout << "No se encontraron equipos que coincidan.\n";
        }
    }
}

void menuActualizarEquipo(SistemaDeportivo* s) {
    cout << "\n--- ACTUALIZAR EQUIPO ---\n";
    int id;
    cout << "Ingrese ID del equipo a actualizar: ";
    if (!(cin >> id)) { limpiarBuffer(); return; }
    limpiarBuffer();
    
    Equipo* eq = buscarEquipoPorID(s, id);
    if (eq == nullptr) {
        cout << "ERROR: No existe ningun equipo con ID " << id << ".\n";
        return;
    }
    
    cout << "Datos actuales: "; mostrarEquipo(eq);
    
    Equipo temp;
    leerCadenaOBligatoria(temp.nombre, 100, "Nuevo Nombre");
    leerCadenaOBligatoria(temp.ciudad, 100, "Nueva Ciudad");
    leerCadenaOBligatoria(temp.entrenador, 100, "Nuevo Entrenador");
    
    if (solicitarConfirmacion("¿Desea confirmar las modificaciones?")) {
        if (actualizarEquipo(s, id, temp)) {
            cout << "Equipo modificado correctamente.\n";
        }
    }
}

void menuEliminarEquipo(SistemaDeportivo* s) {
    cout << "\n--- ELIMINAR EQUIPO ---\n";
    int id;
    cout << "Ingrese ID del equipo a eliminar: ";
    if (!(cin >> id)) { limpiarBuffer(); return; }
    limpiarBuffer();
    
    Equipo* eq = buscarEquipoPorID(s, id);
    if (eq == nullptr) {
        cout << "ERROR: No existe ningun equipo con ID " << id << ".\n";
        return;
    }
    
    int partidosAsociados = contarPartidosDeEquipo(s, id);
    if (partidosAsociados > 0) {
        cout << "ADVERTENCIA: El equipo tiene " << partidosAsociados << " partidos asociados.\n";
        if (!solicitarConfirmacion("¿Desea eliminarlo igualmente?")) {
            cout << "Eliminacion abortada.\n";
            return;
        }
    } else {
        if (!solicitarConfirmacion("¿Esta seguro de eliminar este equipo?")) return;
    }
    
    if (eliminarEquipo(s, id)) {
        cout << "Equipo eliminado de manera exitosa.\n";
    }
}

void menuListarEquipos(SistemaDeportivo* s) {
    int cant = 0;
    Equipo** lista = listarEquipos(s, &cant);
    if (lista != nullptr) {
        cout << "\n--- LISTADO GENERAL DE EQUIPOS ---\n";
        mostrarListaEquipos(lista, cant);
        delete[] lista;
    } else {
        cout << "No hay equipos registrados en el sistema.\n";
    }
}

void mostrarTablaPosiciones(SistemaDeportivo* s) {
    int cant = 0;
    Equipo** tabla = generarTablaPosiciones(s, &cant);
    
    cout << "\n╔═════════════════════════════════════════════════════╗\n";
    cout << "║                      TABLA DE POSICIONES               ║\n";
    cout << "║                      " << s->torneo.nombre << "        ║\n";
    cout << "╠════╦══════════════════╦═════╦═══╦═══╦═══╦════╦════╦════╣\n";
    cout << "║ #  ║ Equipo           ║ PTS ║ J ║ G ║ E ║ D  ║ GF ║ GC ║\n";
    cout << "╠════╬══════════════════╬═════╬═══╬═══╬═══╬════╬════╬════╣\n";
    
    if (tabla == nullptr || cant == 0) {
        cout << "║    No hay equipos registrados aun en la competicion.                 ║\n";
    } else {
        for (int i = 0; i < cant; i++) {
            int jugados = tabla[i]->victorias + tabla[i]->empates + tabla[i]->derrotas;
            printf("║ %2d ║ %-16s ║ %3d ║ %1d ║ %1d ║ %1d ║ %1d  ║ %2d ║ %2d ║\n",
                   i + 1, tabla[i]->nombre, tabla[i]->puntos, jugados,
                   tabla[i]->victorias, tabla[i]->empates, tabla[i]->derrotas,
                   tabla[i]->puntosAFavor, tabla[i]->puntosEnContra);
        }
    }
    cout << "╚════╩══════════════════╩═════╩═══╩═══╩═══╩════╩════╩════╝\n";
    if (tabla != nullptr) delete[] tabla;
}

void mostrarJugador(Jugador* j, SistemaDeportivo* s) {
    if (j == nullptr) return;
    Equipo* eq = buscarEquipoPorID(s, j->idEquipo);
    const char* nombreEquipo = (eq != nullptr) ? eq->nombre : "Desconocido";
    
    cout << "ID: " << j->id << " | Nombre: " << j->nombre << " | Equipo: " << nombreEquipo
         << " | Posición: " << j->posicion << " | Dorsal: " << j->numeroDorsal << " | Edad: " << j->edad << "\n";
}

void mostrarListaJugadores(Jugador** jugadores, int cantidad, SistemaDeportivo* s) {
    cout << "╔════╦══════════════════╦══════════════╦═══════════════╦═════╦═════════╗\n";
    cout << "║ ID ║ Nombre           ║ Equipo       ║ Posición      ║ Edad║ Dorsal  ║\n";
    cout << "╠════╬══════════════════╬══════════════╬═══════════════╬═════╬═════════╣\n";
    for (int i = 0; i < cantidad; i++) {
        Equipo* eq = buscarEquipoPorID(s, jugadores[i]->idEquipo);
        printf("║ %2d ║ %-16s ║ %-12s ║ %-13s ║ %3d ║   %2d    ║\n",
               jugadores[i]->id, jugadores[i]->nombre, eq ? eq->nombre : "N/A",
               jugadores[i]->posicion, jugadores[i]->edad, jugadores[i]->numeroDorsal);
    }
    cout << "╚════╩══════════════════╩══════════════╩═══════════════╩═════╩═════════╝\n";
    cout << "Total de jugadores: " << cantidad << "\n";
}

void menuRegistrarJugador(SistemaDeportivo* s) {
    cout << "\n--- REGISTRAR NUEVO JUGADOR ---\n";
    char nombre[100], cedula[20], posicion[20];
    int idEquipo, edad, dorsal;
    
    cout << "Ingrese el ID del equipo al que se unira: ";
    if (!(cin >> idEquipo)) { limpiarBuffer(); return; }
    limpiarBuffer();
    
    Equipo* eq = buscarEquipoPorID(s, idEquipo);
    if (eq == nullptr) {
        cout << "ERROR: No existe ningún equipo con ID " << idEquipo << ".\n";
        return;
    }
    
    leerCadenaOBligatoria(nombre, 100, "Nombre Completo (escriba 'CANCELAR' si desea abortar)");
    if (compararCadenas(nombre, "CANCELAR")) { cout << "Registro cancelado.\n"; return; }
    
    leerCadenaOBligatoria(cedula, 20, "Cedula de Identidad");
    for (int i = 0; i < s->numJugadores; i++) {
        if (compararCadenas(s->jugadores[i].cedula, cedula)) {
            cout << "ERROR: La cédula '" << cedula << "' ya está registrada.\n";
            return;
        }
    }
    
    while (true) {
        leerCadenaOBligatoria(posicion, 20, "Posicion (PORTERO, DEFENSA, MEDIOCAMPISTA, DELANTERO)");
        if (compararCadenas(posicion, "PORTERO") || compararCadenas(posicion, "DEFENSA") ||
            compararCadenas(posicion, "MEDIOCAMPISTA") || compararCadenas(posicion, "DELANTERO")) {
            break;
        }
        cout << "ERROR: Posicion invalida.\n";
    }
    
    while (true) {
        cout << "Ingrese Edad (14-50): ";
        if (cin >> edad && edad >= 14 && edad <= 50) { limpiarBuffer(); break; }
        cout << "ERROR: Rango de edad invalido.\n";
        limpiarBuffer();
    }
    
    while (true) {
        cout << "Ingrese Numero Dorsal (1-99): ";
        if (cin >> dorsal && dorsal >= 1 && dorsal <= 99) {
            limpiarBuffer();
            bool duplicado = false;
            for (int i = 0; i < s->numJugadores; i++) {
                if (s->jugadores[i].idEquipo == idEquipo && s->jugadores[i].numeroDorsal == dorsal) {
                    duplicado = true;
                    break;
                }
            }
            if (!duplicado) break;
            cout << "ERROR: El dorsal " << dorsal << " ya está en uso en el equipo '" << eq->nombre << "'.\n";
        } else {
            cout << "ERROR: Dorsal fuera de rango.\n";
            limpiarBuffer();
        }
    }
    
    if (solicitarConfirmacion("¿Desea confirmar la inscripcion del jugador?")) {
        Jugador* j = agregarJugador(s, idEquipo, nombre, cedula, posicion, edad, dorsal);
        if (j != nullptr) cout << "Jugador registrado correctamente.\n";
    }
}

void menuBuscarJugador(SistemaDeportivo* s) {
    cout << "\n--- BUSCAR JUGADOR ---\n";
    char sub[100];
    leerCadenaOBligatoria(sub, 100, "Nombre completo o parcial a buscar");
    int cant = 0;
    Jugador** res = buscarJugadoresPorNombre(s, sub, &cant);
    if (res != nullptr) {
        mostrarListaJugadores(res, cant, s);
        delete[] res;
    } else {
        cout << "No se encontraron coincidencias.\n";
    }
}

void menuActualizarJugador(SistemaDeportivo* s) {
    cout << "\n--- ACTUALIZAR JUGADOR ---\n";
    int id;
    cout << "ID del jugador: ";
    if (!(cin >> id)) { limpiarBuffer(); return; }
    limpiarBuffer();
    
    Jugador* j = buscarJugadorPorID(s, id);
    if (j == nullptr) { cout << "Jugador no encontrado.\n"; return; }
    
    Jugador temp = *j;
    leerCadenaOBligatoria(temp.nombre, 100, "Nuevo Nombre");
    cout << "Nueva edad (" << j->edad << "): "; cin >> temp.edad; limpiarBuffer();
    
    if (solicitarConfirmacion("¿Confirmar edicion?")) {
        actualizarJugador(s, id, temp);
        cout << "Informacion salvada.\n";
    }
}

void menuEliminarJugador(SistemaDeportivo* s) {
    cout << "\n--- DEBAJA DE JUGADOR ---\n";
    int id;
    cout << "ID del jugador: ";
    if (!(cin >> id)) { limpiarBuffer(); return; }
    limpiarBuffer();
    
    if (solicitarConfirmacion("¿Esta seguro de desvincular al jugador?")) {
        if (eliminarJugador(s, id)) cout << "El jugador ha sido borrado del sistema.\n";
        else cout << "ERROR: ID inexistente.\n";
    }
}

void menuListarJugadores(SistemaDeportivo* s) {
    cout << "\n1. Listar todos\n2. Listar por Equipo\nSeleccione: ";
    int op; cin >> op; limpiarBuffer();
    int cant = 0;
    if (op == 1) {
        Jugador** lista = listarJugadores(s, &cant);
        if (lista) { mostrarListaJugadores(lista, cant, s); delete[] lista; }
        else cout << "No hay registros.\n";
    } else {
        int idEq; cout << "ID de Equipo: "; cin >> idEq; limpiarBuffer();
        Jugador** lista = listarJugadoresPorEquipo(s, idEq, &cant);
        if (lista) { mostrarListaJugadores(lista, cant, s); delete[] lista; }
        else cout << "No hay jugadores en este equipo.\n";
    }
}

void mostrarPartido(Partido* p, SistemaDeportivo* s) {
    if (p == nullptr) return;
    Equipo* loc = buscarEquipoPorID(s, p->idEquipoLocal);
    Equipo* vis = buscarEquipoPorID(s, p->idEquipoVisitante);
    
    cout << "╔══════════════════════════════════════════════════╗\n";
    cout << "║               DETALLE DE PARTIDO                 ║\n";
    cout << "╠══════════════════════════════════════════════════╣\n";
    printf("║ ID Partido  : %-34d ║\n", p->id);
    printf("║ Estado      : %-34s ║\n", p->estado);
    printf("║ Fecha       : %-34s ║\n", p->fecha);
    cout << "║                                                  ║\n";
    printf("║ %19s  %d - %d  %-19s ║\n", loc ? loc->nombre : "Local", p->puntosLocal, p->puntosVisitante, vis ? vis->nombre : "Vis.");
    cout << "║       (Local)                  (Visitante)       ║\n";
    cout << "║                                                  ║\n";
    printf("║ Notas: %-41s ║\n", p->descripcion);
    cout << "╚══════════════════════════════════════════════════╝\n";
}

void menuProgramarPartido(SistemaDeportivo* s) {
    cout << "\n--- PROGRAMAR NUEVO PARTIDO ---\n";
    int idLocal, idVisitante;
    char fecha[11], descripcion[200];
    
    cout << "Ingrese ID del Equipo Local: ";
    if (!(cin >> idLocal)) { limpiarBuffer(); return; }
    cout << "Ingrese ID del Equipo Visitante: ";
    if (!(cin >> idVisitante)) { limpiarBuffer(); return; }
    limpiarBuffer();
    
    leerCadenaOBligatoria(fecha, 11, "Fecha (AAAA-MM-DD)");
    leerCadenaOBligatoria(descripcion, 200, "Descripción/Notas del Partido");
    
    Partido* p = programarPartido(s, idLocal, idVisitante, fecha, descripcion);
    if (p != nullptr) {
        cout << "Partido programado con éxito.\n";
    } else {
        cout << "ERROR: No se pudo programar el partido. Verifique IDs o duplicados.\n";
    }
}

void menuRegistrarResultado(SistemaDeportivo* s) {
    cout << "\n--- REGISTRAR RESULTADO DE PARTIDO ---\n";
    int idPartido, ptsLocal, ptsVisitante;
    cout << "Ingrese ID del Partido: ";
    if (!(cin >> idPartido)) { limpiarBuffer(); return; }
    cout << "Puntos/Goles Equipo Local: ";
    if (!(cin >> ptsLocal)) { limpiarBuffer(); return; }
    cout << "Puntos/Goles Equipo Visitante: ";
    if (!(cin >> ptsVisitante)) { limpiarBuffer(); return; }
    limpiarBuffer();
    
    if (registrarResultado(s, idPartido, ptsLocal, ptsVisitante)) {
        cout << "Resultado guardado y estadísticas actualizadas en la tabla.\n";
    } else {
        cout << "ERROR: El partido no existe o ya ha sido jugado/cancelado.\n";
    }
}

void menuListarPartidos(SistemaDeportivo* s) {
    int cant = 0;
    Partido** lista = listarPartidos(s, &cant);
    if (lista != nullptr) {
        cout << "\n--- CRONOGRAMA GENERAL DE PARTIDOS ---\n";
        for (int i = 0; i < cant; i++) {
            mostrarPartido(lista[i], s);
        }
        delete[] lista;
    } else {
        cout << "No hay partidos registrados.\n";
    }
}


// 5. FUNCIÓN PRINCIPAL (Implementación del Menú Consola)


int main(int argc, char* argv[]) {
    // Configuración regional para caracteres especiales
    setlocale(LC_ALL, "spanish");
    
    // Inicialización del torneo base
    Torneo miTorneo;
    copiarCadena(miTorneo.nombre, "Torneo de Verano 2026");
    copiarCadena(miTorneo.deporte, "Fútbol");
    copiarCadena(miTorneo.formato, "ELIMINATORIA");
    copiarCadena(miTorneo.fechaInicio, "2026-06-01");
    copiarCadena(miTorneo.fechaFin, "2026-07-15");
    
    SistemaDeportivo sistema;
    inicializarSistema(&sistema, miTorneo);
    
    int opcionPrimaria = 0;
    do {
        cout << "\n==================================================\n";
        cout << "         SISTEMA DE GESTIÓN DEPORTIVA             \n";
        cout << "         Torneo: " << sistema.torneo.nombre << "\n";
        cout << "==================================================\n";
        cout << "1. Gestión de Equipos\n";
        cout << "2. Gestión de Jugadores\n";
        cout << "3. Gestión de Partidos y Calendario\n";
        cout << "4. Ver Tabla de Posiciones / Estadísticas\n";
        cout << "5. Salir del Sistema\n";
        cout << "Seleccione una opción: ";
        
        if (!(cin >> opcionPrimaria)) {
            limpiarBuffer();
            continue;
        }
        limpiarBuffer();
        
        switch (opcionPrimaria) {
            case 1: {
                int opSub = 0;
                do {
                    cout << "\n-- SUBMENÚ EQUIPOS --\n";
                    cout << "1. Registrar Equipo\n2. Buscar Equipo\n3. Actualizar Equipo\n4. Eliminar Equipo\n5. Listar Todos\n6. Volver\nOpción: ";
                    if (cin >> opSub) {
                        limpiarBuffer();
                        if (opSub == 1) menuRegistrarEquipo(&sistema);
                        else if (opSub == 2) menuBuscarEquipo(&sistema);
                        else if (opSub == 3) menuActualizarEquipo(&sistema);
                        else if (opSub == 4) menuEliminarEquipo(&sistema);
                        else if (opSub == 5) menuListarEquipos(&sistema);
                    } else { limpiarBuffer(); }
                } while (opSub != 6);
                break;
            }
            case 2: {
                int opSub = 0;
                do {
                    cout << "\n-- SUBMENÚ JUGADORES --\n";
                    cout << "1. Registrar Jugador\n2. Buscar Jugador\n3. Actualizar Jugador\n4. Dar de Baja\n5. Listar Jugadores\n6. Volver\nOpción: ";
                    if (cin >> opSub) {
                        limpiarBuffer();
                        if (opSub == 1) menuRegistrarJugador(&sistema);
                        else if (opSub == 2) menuBuscarJugador(&sistema);
                        else if (opSub == 3) menuActualizarJugador(&sistema);
                        else if (opSub == 4) menuEliminarJugador(&sistema);
                        else if (opSub == 5) menuListarJugadores(&sistema);
                    } else { limpiarBuffer(); }
                } while (opSub != 6);
                break;
            }
            case 3: {
                int opSub = 0;
                 do {
                    cout << "\n-- SUBMENÚ PARTIDOS --\n";
                    cout << "1. Programar Partido\n2. Registrar Marcador/Resultado\n3. Listar Todos los Partidos\n4. Volver\nOpción: ";
                        if (cin >> opSub) {
                     limpiarBuffer();
                     if (opSub == 1) menuProgramarPartido(&sistema);
                            
                    else if (opSub == 2) menuRegistrarResultado(&sistema); 
                    else if (opSub == 3) menuListarPartidos(&sistema);
                } else { limpiarBuffer(); }
                 } while (opSub != 4);
                 break;
}
            case 4:
                mostrarTablaPosiciones(&sistema);
                break;
            case 5:
                cout << "Guardando cambios y liberando memoria del sistema...\n";
                break;
            default:
                cout << "Opción inválida. Intente de nuevo.\n";
        }
    } while (opcionPrimaria != 5);
    
    liberarSistema(&sistema);
    cout << "Sistema cerrado correctamente.\n";
    return 0;
}