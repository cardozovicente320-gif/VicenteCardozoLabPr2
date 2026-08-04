#include <iostream>
#include <fstream>
#include <locale>
#include <limits>
#include <ctime>
#include <iomanip>

using namespace std;

// ============================================================================
// 1. CLASES DE ENTIDAD (ENCAPSULAMIENTO COMPLETO Y MANTENIMIENTO DE LAYOUT)
// ============================================================================

// --- AUXILIARES PREVIAS DE CADENA (Necesarias para los constructores) ---
void copiarCadena(char* destino, const char* origen) {
    if (!destino || !origen) return;
    while (*origen != '\0') {
        *destino = *origen;
        destino++;
        origen++;
    }
    *destino = '\0';
}

bool compararCadenas(const char* cad1, const char* cad2) {
    if (!cad1 || !cad2) return false;
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
    if (!cadena || !subcadena) return false;
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

// --- CLASE GOL ---
class Gol {
private:
    int  idJugador;       // ID del jugador que anotó (0 = desconocido / gol en contra)
    int  minuto;          // Minuto del partido en que se anotó (1 - 120)
    char equipo[12];      // "LOCAL" o "VISITANTE"

public:
    Gol() : idJugador(0), minuto(0) {
        equipo[0] = '\0';
    }

    Gol(int idJugador, int minuto, const char* eq) : idJugador(idJugador) {
        setMinuto(minuto);
        setEquipo(eq);
    }

    Gol(const Gol& otro) {
        idJugador = otro.idJugador;
        minuto = otro.minuto;
        copiarCadena(equipo, otro.equipo);
    }

    ~Gol() {}

    int getIdJugador() const { return idJugador; }
    void setIdJugador(int id) { idJugador = id; }

    int getMinuto() const { return minuto; }
    bool setMinuto(int min) {
        if (min >= 1 && min <= 120) {
            minuto = min;
            return true;
        }
        return false;
    }

    const char* getEquipo() const { return equipo; }
    bool setEquipo(const char* eq) {
        if (eq && (compararCadenas(eq, "LOCAL") || compararCadenas(eq, "VISITANTE"))) {
            copiarCadena(equipo, eq);
            return true;
        }
        return false;
    }

    bool esValido() const {
        return minuto >= 1 && minuto <= 120 && (compararCadenas(equipo, "LOCAL") || compararCadenas(equipo, "VISITANTE"));
    }

    static size_t obtenerTamano() { return sizeof(Gol); }
};

// --- CLASE TORNEO ---
class Torneo {
private:
    char   nombre[100];
    char   deporte[50];
    char   formato[20];
    char   fechaInicio[11];
    char   fechaFin[11];
    time_t fechaCreacion;
    time_t fechaUltimaModificacion;

public:
    Torneo() {
        nombre[0] = '\0';
        deporte[0] = '\0';
        formato[0] = '\0';
        fechaInicio[0] = '\0';
        fechaFin[0] = '\0';
        fechaCreacion = time(nullptr);
        fechaUltimaModificacion = time(nullptr);
    }

    Torneo(const char* nom, const char* dep, const char* form, const char* fIn, const char* fFin) {
        setNombre(nom);
        setDeporte(dep);
        setFormato(form);
        setFechaInicio(fIn);
        setFechaFin(fFin);
        fechaCreacion = time(nullptr);
        fechaUltimaModificacion = time(nullptr);
    }

    Torneo(const Torneo& otro) {
        copiarCadena(nombre, otro.nombre);
        copiarCadena(deporte, otro.deporte);
        copiarCadena(formato, otro.formato);
        copiarCadena(fechaInicio, otro.fechaInicio);
        copiarCadena(fechaFin, otro.fechaFin);
        fechaCreacion = otro.fechaCreacion;
        fechaUltimaModificacion = otro.fechaUltimaModificacion;
    }

    ~Torneo() {}

    const char* getNombre() const { return nombre; }
    bool setNombre(const char* nom) {
        if (nom && nom[0] != '\0') {
            copiarCadena(nombre, nom);
            return true;
        }
        return false;
    }

    const char* getDeporte() const { return deporte; }
    bool setDeporte(const char* dep) {
        if (dep && dep[0] != '\0') {
            copiarCadena(deporte, dep);
            return true;
        }
        return false;
    }

    const char* getFormato() const { return formato; }
    bool setFormato(const char* form) {
        if (form && form[0] != '\0') {
            copiarCadena(formato, form);
            return true;
        }
        return false;
    }

    const char* getFechaInicio() const { return fechaInicio; }
    bool setFechaInicio(const char* f) {
        if (f) { copiarCadena(fechaInicio, f); return true; }
        return false;
    }

    const char* getFechaFin() const { return fechaFin; }
    bool setFechaFin(const char* f) {
        if (f) { copiarCadena(fechaFin, f); return true; }
        return false;
    }

    time_t getFechaCreacion() const { return fechaCreacion; }
    void setFechaCreacion(time_t t) { fechaCreacion = t; }

    time_t getFechaUltimaModificacion() const { return fechaUltimaModificacion; }
    void setFechaUltimaModificacion(time_t t) { fechaUltimaModificacion = t; }

    bool esValido() const { return nombre[0] != '\0'; }
    static size_t obtenerTamano() { return sizeof(Torneo); }
};

// --- CLASE EQUIPO ---
class Equipo {
private:
    int    id;
    char   nombre[100];
    char   ciudad[100];
    char   entrenador[100];

    int    puntos;
    int    victorias;
    int    empates;
    int    derrotas;
    int    golesAFavor;
    int    golesEnContra;

    int    partidosIDs[50];
    int    cantidadPartidos;

    bool   eliminado;
    time_t fechaCreacion;
    time_t fechaUltimaModificacion;

public:
    Equipo() : id(0), puntos(0), victorias(0), empates(0), derrotas(0),
               golesAFavor(0), golesEnContra(0), cantidadPartidos(0), eliminado(false) {
        nombre[0] = '\0';
        ciudad[0] = '\0';
        entrenador[0] = '\0';
        fechaCreacion = time(nullptr);
        fechaUltimaModificacion = time(nullptr);
    }

    Equipo(const char* nom, const char* ciu, const char* ent) : Equipo() {
        setNombre(nom);
        setCiudad(ciu);
        setEntrenador(ent);
    }

    Equipo(const Equipo& otro) {
        id = otro.id;
        copiarCadena(nombre, otro.nombre);
        copiarCadena(ciudad, otro.ciudad);
        copiarCadena(entrenador, otro.entrenador);
        puntos = otro.puntos;
        victorias = otro.victorias;
        empates = otro.empates;
        derrotas = otro.derrotas;
        golesAFavor = otro.golesAFavor;
        golesEnContra = otro.golesEnContra;
        cantidadPartidos = otro.cantidadPartidos;
        for (int i = 0; i < cantidadPartidos; i++) partidosIDs[i] = otro.partidosIDs[i];
        eliminado = otro.eliminado;
        fechaCreacion = otro.fechaCreacion;
        fechaUltimaModificacion = otro.fechaUltimaModificacion;
    }

    ~Equipo() {}

    int getId() const { return id; }
    void setId(int i) { id = i; }

    const char* getNombre() const { return nombre; }
    bool setNombre(const char* nom) {
        if (nom && nom[0] != '\0') {
            copiarCadena(nombre, nom);
            return true;
        }
        return false;
    }

    const char* getCiudad() const { return ciudad; }
    bool setCiudad(const char* ciu) {
        if (ciu && ciu[0] != '\0') {
            copiarCadena(ciudad, ciu);
            return true;
        }
        return false;
    }

    const char* getEntrenador() const { return entrenador; }
    bool setEntrenador(const char* ent) {
        if (ent && ent[0] != '\0') {
            copiarCadena(entrenador, ent);
            return true;
        }
        return false;
    }

    int getPuntos() const { return puntos; }
    void setPuntos(int p) { puntos = p; }

    int getVictorias() const { return victorias; }
    void setVictorias(int v) { victorias = v; }

    int getEmpates() const { return empates; }
    void setEmpates(int e) { empates = e; }

    int getDerrotas() const { return derrotas; }
    void setDerrotas(int d) { derrotas = d; }

    int getGolesAFavor() const { return golesAFavor; }
    void setGolesAFavor(int gf) { golesAFavor = gf; }

    int getGolesEnContra() const { return golesEnContra; }
    void setGolesEnContra(int gc) { golesEnContra = gc; }

    int getCantidadPartidos() const { return cantidadPartidos; }
    const int* getPartidosIDs() const { return partidosIDs; }

    bool agregarPartidoID(int idPart) {
        if (cantidadPartidos < 50) {
            partidosIDs[cantidadPartidos++] = idPart;
            return true;
        }
        return false;
    }

    bool removerPartidoID(int idPart) {
        for (int i = 0; i < cantidadPartidos; i++) {
            if (partidosIDs[i] == idPart) {
                for (int j = i; j < cantidadPartidos - 1; j++) {
                    partidosIDs[j] = partidosIDs[j + 1];
                }
                cantidadPartidos--;
                return true;
            }
        }
        return false;
    }

    bool getEliminado() const { return eliminado; }
    void setEliminado(bool val) { eliminado = val; }

    time_t getFechaCreacion() const { return fechaCreacion; }
    void setFechaCreacion(time_t t) { fechaCreacion = t; }

    time_t getFechaUltimaModificacion() const { return fechaUltimaModificacion; }
    void setFechaUltimaModificacion(time_t t) { fechaUltimaModificacion = t; }

    bool esValido() const { return nombre[0] != '\0' && !eliminado; }

    void mostrarBasico() const {
        cout << "ID: " << id << " | " << nombre << " | " << ciudad << " | Puntos: " << puntos << "\n";
    }

    void mostrarCompleto() const {
        cout << "ID: " << id << " | " << nombre << " | Ciudad: " << ciudad << " | DT: " << entrenador << "\n";
        cout << "PTS: " << puntos << " | V: " << victorias << " | E: " << empates << " | D: " << derrotas 
             << " | GF: " << golesAFavor << " | GC: " << golesEnContra << "\n";
    }

    static size_t obtenerTamano() { return sizeof(Equipo); }
};

// --- CLASE JUGADOR ---
class Jugador {
private:
    int    id;
    int    idEquipo;
    char   nombre[100];
    char   cedula[20];
    char   posicion[20];
    int    edad;
    int    numeroDorsal;

    int    golesAnotados;
    int    tarjetasAmarillas;
    int    tarjetasRojas;

    bool   eliminado;
    time_t fechaCreacion;
    time_t fechaUltimaModificacion;

public:
    Jugador() : id(0), idEquipo(0), edad(0), numeroDorsal(0), golesAnotados(0),
                tarjetasAmarillas(0), tarjetasRojas(0), eliminado(false) {
        nombre[0] = '\0';
        cedula[0] = '\0';
        posicion[0] = '\0';
        fechaCreacion = time(nullptr);
        fechaUltimaModificacion = time(nullptr);
    }

    Jugador(int idEq, const char* nom, const char* ced, const char* pos, int ed, int dorsal) : Jugador() {
        setIdEquipo(idEq);
        setNombre(nom);
        setCedula(ced);
        setPosicion(pos);
        setEdad(ed);
        setNumeroDorsal(dorsal);
    }

    Jugador(const Jugador& otro) {
        id = otro.id;
        idEquipo = otro.idEquipo;
        copiarCadena(nombre, otro.nombre);
        copiarCadena(cedula, otro.cedula);
        copiarCadena(posicion, otro.posicion);
        edad = otro.edad;
        numeroDorsal = otro.numeroDorsal;
        golesAnotados = otro.golesAnotados;
        tarjetasAmarillas = otro.tarjetasAmarillas;
        tarjetasRojas = otro.tarjetasRojas;
        eliminado = otro.eliminado;
        fechaCreacion = otro.fechaCreacion;
        fechaUltimaModificacion = otro.fechaUltimaModificacion;
    }

    ~Jugador() {}

    int getId() const { return id; }
    void setId(int i) { id = i; }

    int getIdEquipo() const { return idEquipo; }
    bool setIdEquipo(int idEq) {
        if (idEq > 0) { idEquipo = idEq; return true; }
        return false;
    }

    const char* getNombre() const { return nombre; }
    bool setNombre(const char* nom) {
        if (nom && nom[0] != '\0') { copiarCadena(nombre, nom); return true; }
        return false;
    }

    const char* getCedula() const { return cedula; }
    bool setCedula(const char* ced) {
        if (ced && ced[0] != '\0') { copiarCadena(cedula, ced); return true; }
        return false;
    }

    const char* getPosicion() const { return posicion; }
    bool setPosicion(const char* pos) {
        if (pos && pos[0] != '\0') { copiarCadena(posicion, pos); return true; }
        return false;
    }

    int getEdad() const { return edad; }
    bool setEdad(int ed) {
        if (ed >= 15 && ed <= 50) { edad = ed; return true; }
        return false;
    }

    int getNumeroDorsal() const { return numeroDorsal; }
    bool setNumeroDorsal(int d) {
        if (d >= 1 && d <= 99) { numeroDorsal = d; return true; }
        return false;
    }

    int getGolesAnotados() const { return golesAnotados; }
    void setGolesAnotados(int g) { golesAnotados = g; }

    int getTarjetasAmarillas() const { return tarjetasAmarillas; }
    void setTarjetasAmarillas(int ta) { tarjetasAmarillas = ta; }

    int getTarjetasRojas() const { return tarjetasRojas; }
    void setTarjetasRojas(int tr) { tarjetasRojas = tr; }

    bool getEliminado() const { return eliminado; }
    void setEliminado(bool val) { eliminado = val; }

    time_t getFechaCreacion() const { return fechaCreacion; }
    void setFechaCreacion(time_t t) { fechaCreacion = t; }

    time_t getFechaUltimaModificacion() const { return fechaUltimaModificacion; }
    void setFechaUltimaModificacion(time_t t) { fechaUltimaModificacion = t; }

    bool esValido() const { return idEquipo > 0 && nombre[0] != '\0' && !eliminado; }

    void mostrarBasico() const {
        cout << "ID: " << id << " | " << nombre << " | Dorsal: " << numeroDorsal << "\n";
    }

    void mostrarCompleto() const {
        cout << "ID: " << id << " | " << nombre << " | C.I.: " << cedula << " | Edad: " << edad << "\n";
        cout << "Posición: " << posicion << " | Dorsal: " << numeroDorsal << " | Goles: " << golesAnotados << "\n";
    }

    static size_t obtenerTamano() { return sizeof(Jugador); }
};

// --- CLASE PARTIDO ---
class Partido {
private:
    int    id;
    int    idEquipoLocal;
    int    idEquipoVisitante;
    char   fecha[11];
    char   estado[12];       // "PROGRAMADO", "JUGADO", "CANCELADO"
    char   descripcion[200];

    int    golesLocal;
    int    golesVisitante;

    Gol    goles[22];        // Composición
    int    numGoles;

    bool   eliminado;
    time_t fechaCreacion;
    time_t fechaUltimaModificacion;

public:
    Partido() : id(0), idEquipoLocal(0), idEquipoVisitante(0), golesLocal(0),
                golesVisitante(0), numGoles(0), eliminado(false) {
        fecha[0] = '\0';
        copiarCadena(estado, "PROGRAMADO");
        descripcion[0] = '\0';
        fechaCreacion = time(nullptr);
        fechaUltimaModificacion = time(nullptr);
    }

    Partido(int idLoc, int idVis, const char* f, const char* desc) : Partido() {
        setIdEquipoLocal(idLoc);
        setIdEquipoVisitante(idVis);
        setFecha(f);
        setDescripcion(desc);
    }

    Partido(const Partido& otro) {
        id = otro.id;
        idEquipoLocal = otro.idEquipoLocal;
        idEquipoVisitante = otro.idEquipoVisitante;
        copiarCadena(fecha, otro.fecha);
        copiarCadena(estado, otro.estado);
        copiarCadena(descripcion, otro.descripcion);
        golesLocal = otro.golesLocal;
        golesVisitante = otro.golesVisitante;
        numGoles = otro.numGoles;
        for (int i = 0; i < numGoles; i++) goles[i] = otro.goles[i];
        eliminado = otro.eliminado;
        fechaCreacion = otro.fechaCreacion;
        fechaUltimaModificacion = otro.fechaUltimaModificacion;
    }

    ~Partido() {}

    int getId() const { return id; }
    void setId(int i) { id = i; }

    int getIdEquipoLocal() const { return idEquipoLocal; }
    bool setIdEquipoLocal(int idLoc) {
        if (idLoc > 0) { idEquipoLocal = idLoc; return true; }
        return false;
    }

    int getIdEquipoVisitante() const { return idEquipoVisitante; }
    bool setIdEquipoVisitante(int idVis) {
        if (idVis > 0) { idEquipoVisitante = idVis; return true; }
        return false;
    }

    const char* getFecha() const { return fecha; }
    bool setFecha(const char* f) {
        if (f) { copiarCadena(fecha, f); return true; }
        return false;
    }

    const char* getEstado() const { return estado; }
    bool setEstado(const char* est) {
        if (est && (compararCadenas(est, "PROGRAMADO") || compararCadenas(est, "JUGADO") || compararCadenas(est, "CANCELADO"))) {
            copiarCadena(estado, est);
            return true;
        }
        return false;
    }

    const char* getDescripcion() const { return descripcion; }
    void setDescripcion(const char* desc) {
        if (desc) copiarCadena(descripcion, desc);
    }

    int getGolesLocal() const { return golesLocal; }
    void setGolesLocal(int g) { golesLocal = g; }

    int getGolesVisitante() const { return golesVisitante; }
    void setGolesVisitante(int g) { golesVisitante = g; }

    int getNumGoles() const { return numGoles; }
    void setNumGoles(int n) { if (n >= 0 && n <= 22) numGoles = n; }

    const Gol* getGoles() const { return goles; }
    Gol* getGolesModificables() { return goles; }

    bool agregarGol(const Gol& g) {
        if (numGoles < 22) {
            goles[numGoles++] = g;
            return true;
        }
        return false;
    }

    bool getEliminado() const { return eliminado; }
    void setEliminado(bool val) { eliminado = val; }

    time_t getFechaCreacion() const { return fechaCreacion; }
    void setFechaCreacion(time_t t) { fechaCreacion = t; }

    time_t getFechaUltimaModificacion() const { return fechaUltimaModificacion; }
    void setFechaUltimaModificacion(time_t t) { fechaUltimaModificacion = t; }

    bool esValido() const { return idEquipoLocal > 0 && idEquipoVisitante > 0 && !eliminado; }

    void mostrarBasico() const {
        cout << "ID: " << id << " | Estado: " << estado << " | Fecha: " << fecha << "\n";
    }

    static size_t obtenerTamano() { return sizeof(Partido); }
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


