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


//FUNCIONES AUXILIARES DE CADENAS


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

 
//Capa de logica de negocio 

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
    
    // Algoritmo de ordenación burbuja por punteros bajo los criterios del negocio
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

int main(){


    return 0;
}