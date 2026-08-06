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


// ============================================================================
// 2. FUNCIONES AUXILIARES DE TIEMPO Y ENTRADA
// ============================================================================

void obtenerFechaHoy(char* buffer) {
    time_t t = time(nullptr);
    tm* now = localtime(&t);
    strftime(buffer, 11, "%Y-%m-%d", now);
}


// ============================================================================
// 3. CAPA DE PERSISTENCIA Y CONTROLADORES HEADER
// ============================================================================

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
        Torneo t("Torneo de Verano 2026", "Futbol", "ELIMINATORIA", "2026-06-01", "2026-07-15");
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
        
        int registroId = *reinterpret_cast<int*>(buffer);
        bool eliminado = false;
        
        if (compararCadenas(nombreArchivo, FILE_EQUIPOS)) {
            eliminado = reinterpret_cast<Equipo*>(buffer)->getEliminado();
        } else if (compararCadenas(nombreArchivo, FILE_JUGADORES)) {
            eliminado = reinterpret_cast<Jugador*>(buffer)->getEliminado();
        } else if (compararCadenas(nombreArchivo, FILE_PARTIDOS)) {
            eliminado = reinterpret_cast<Partido*>(buffer)->getEliminado();
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


// ============================================================================
// 4. FUNCIONES DE PERSISTENCIA (CRUD DE CLASES)
// ============================================================================

// --- EQUIPOS ---

bool guardarEquipo(Equipo& equipo) {
    ArchivoHeader h = leerHeader(FILE_EQUIPOS);
    equipo.setId(h.proximoID);
    equipo.setEliminado(false);
    equipo.setFechaCreacion(time(nullptr));
    equipo.setFechaUltimaModificacion(time(nullptr));

    fstream archivo(FILE_EQUIPOS, ios::in | ios::out | ios::binary);
    if (!archivo) return false;

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
    int index = buscarIndicePorID(FILE_EQUIPOS, equipo.getId(), sizeof(Equipo));
    if (index == -1) return false;

    equipo.setFechaUltimaModificacion(time(nullptr));

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

    eq.setEliminado(true);
    if (!actualizarEquipo(eq)) return false;

    ArchivoHeader h = leerHeader(FILE_EQUIPOS);
    h.registrosActivos--;
    return actualizarHeader(FILE_EQUIPOS, h);
}

int contarEquiposActivos() {
    return leerHeader(FILE_EQUIPOS).registrosActivos;
}

// --- JUGADORES ---

bool guardarJugador(Jugador& jugador) {
    ArchivoHeader h = leerHeader(FILE_JUGADORES);
    jugador.setId(h.proximoID);
    jugador.setEliminado(false);
    jugador.setFechaCreacion(time(nullptr));
    jugador.setFechaUltimaModificacion(time(nullptr));

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
    int index = buscarIndicePorID(FILE_JUGADORES, jugador.getId(), sizeof(Jugador));
    if (index == -1) return false;

    jugador.setFechaUltimaModificacion(time(nullptr));

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

    jug.setEliminado(true);
    if (!actualizarJugador(jug)) return false;

    ArchivoHeader h = leerHeader(FILE_JUGADORES);
    h.registrosActivos--;
    return actualizarHeader(FILE_JUGADORES, h);
}

// --- PARTIDOS ---

bool guardarPartido(Partido& partido) {
    ArchivoHeader h = leerHeader(FILE_PARTIDOS);
    partido.setId(h.proximoID);
    partido.setEliminado(false);
    partido.setFechaCreacion(time(nullptr));
    partido.setFechaUltimaModificacion(time(nullptr));

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
    int index = buscarIndicePorID(FILE_PARTIDOS, partido.getId(), sizeof(Partido));
    if (index == -1) return false;

    partido.setFechaUltimaModificacion(time(nullptr));

    fstream archivo(FILE_PARTIDOS, ios::in | ios::out | ios::binary);
    if (!archivo) return false;

    archivo.seekp(sizeof(ArchivoHeader) + (index * sizeof(Partido)), ios::beg);
    archivo.write(reinterpret_cast<char*>(&partido), sizeof(Partido));
    archivo.close();
    return true;
}


// ============================================================================
// 5. LÓGICA DE BÚSQUEDA Y FILTRADO
// ============================================================================

int buscarEquiposPorNombre(const char* subcadena, Equipo resultados[], int maxResultados) {
    ifstream archivo(FILE_EQUIPOS, ios::binary);
    if (!archivo) return 0;

    ArchivoHeader h;
    archivo.read(reinterpret_cast<char*>(&h), sizeof(ArchivoHeader));

    int count = 0;
    for (int i = 0; i < h.cantidadRegistros && count < maxResultados; i++) {
        Equipo eq;
        archivo.read(reinterpret_cast<char*>(&eq), sizeof(Equipo));
        if (!eq.getEliminado() && contieneSubcadena(eq.getNombre(), subcadena)) {
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
        if (!jug.getEliminado() && jug.getIdEquipo() == idEquipo) {
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
        if (!p.getEliminado() && (p.getIdEquipoLocal() == idEquipo || p.getIdEquipoVisitante() == idEquipo)) {
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
        if (!p.getEliminado() && compararCadenas(p.getEstado(), estado)) {
            resultados[count++] = p;
        }
    }
    archivo.close();
    return count;
}


// ============================================================================
// 6. OPERACIONES COMPUESTAS (TRANSACCIONALIDAD)
// ============================================================================

bool registrarResultadoPartido(int idPartido, int golesLocal, int golesVisitante, Gol detalleGoles[], int numGoles) {
    Partido part;
    if (!leerPartidoPorID(idPartido, part)) return false;
    if (!compararCadenas(part.getEstado(), "PROGRAMADO")) return false;

    Equipo local, visitante;
    if (!leerEquipoPorID(part.getIdEquipoLocal(), local) || !leerEquipoPorID(part.getIdEquipoVisitante(), visitante)) return false;

    part.setGolesLocal(golesLocal);
    part.setGolesVisitante(golesVisitante);
    part.setNumGoles(0); // Limpiar para agregar vía método de la clase
    
    for (int i = 0; i < numGoles; i++) {
        part.agregarGol(detalleGoles[i]);
    }
    part.setEstado("JUGADO");

    // Lógica de puntos
    if (golesLocal > golesVisitante) {
        local.setPuntos(local.getPuntos() + 3);
        local.setVictorias(local.getVictorias() + 1);
        visitante.setDerrotas(visitante.getDerrotas() + 1);
    } else if (golesLocal == golesVisitante) {
        local.setPuntos(local.getPuntos() + 1);
        local.setEmpates(local.getEmpates() + 1);
        visitante.setPuntos(visitante.getPuntos() + 1);
        visitante.setEmpates(visitante.getEmpates() + 1);
    } else {
        visitante.setPuntos(visitante.getPuntos() + 3);
        visitante.setVictorias(visitante.getVictorias() + 1);
        local.setDerrotas(local.getDerrotas() + 1);
    }

    local.setGolesAFavor(local.getGolesAFavor() + golesLocal);
    local.setGolesEnContra(local.getGolesEnContra() + golesVisitante);
    visitante.setGolesAFavor(visitante.getGolesAFavor() + golesVisitante);
    visitante.setGolesEnContra(visitante.getGolesEnContra() + golesLocal);

    local.agregarPartidoID(idPartido);
    visitante.agregarPartidoID(idPartido);

    // Actualizar goles individuales
    for (int i = 0; i < numGoles; i++) {
        if (detalleGoles[i].getIdJugador() > 0) {
            Jugador jug;
            if (leerJugadorPorID(detalleGoles[i].getIdJugador(), jug)) {
                jug.setGolesAnotados(jug.getGolesAnotados() + 1);
                actualizarJugador(jug);
            }
        }
    }

    return actualizarPartido(part) && actualizarEquipo(local) && actualizarEquipo(visitante);
}

bool cancelarPartidoJugado(int idPartido) {
    Partido part;
    if (!leerPartidoPorID(idPartido, part)) return false;
    if (!compararCadenas(part.getEstado(), "JUGADO")) return false;

    Equipo local, visitante;
    if (!leerEquipoPorID(part.getIdEquipoLocal(), local) || !leerEquipoPorID(part.getIdEquipoVisitante(), visitante)) return false;

    // Deshacer puntos
    if (part.getGolesLocal() > part.getGolesVisitante()) {
        local.setPuntos(local.getPuntos() - 3); local.setVictorias(local.getVictorias() - 1);
        visitante.setDerrotas(visitante.getDerrotas() - 1);
    } else if (part.getGolesLocal() == part.getGolesVisitante()) {
        local.setPuntos(local.getPuntos() - 1); local.setEmpates(local.getEmpates() - 1);
        visitante.setPuntos(visitante.getPuntos() - 1); visitante.setEmpates(visitante.getEmpates() - 1);
    } else {
        visitante.setPuntos(visitante.getPuntos() - 3); visitante.setVictorias(visitante.getVictorias() - 1);
        local.setDerrotas(local.getDerrotas() - 1);
    }

    local.setGolesAFavor(local.getGolesAFavor() - part.getGolesLocal());
    local.setGolesEnContra(local.getGolesEnContra() - part.getGolesVisitante());
    visitante.setGolesAFavor(visitante.getGolesAFavor() - part.getGolesVisitante());
    visitante.setGolesEnContra(visitante.getGolesEnContra() - part.getGolesLocal());

    local.removerPartidoID(idPartido);
    visitante.removerPartidoID(idPartido);

    // Revertir goles
    const Gol* golesPart = part.getGoles();
    for (int i = 0; i < part.getNumGoles(); i++) {
        if (golesPart[i].getIdJugador() > 0) {
            Jugador jug;
            if (leerJugadorPorID(golesPart[i].getIdJugador(), jug)) {
                if (jug.getGolesAnotados() > 0) jug.setGolesAnotados(jug.getGolesAnotados() - 1);
                actualizarJugador(jug);
            }
        }
    }

    part.setEstado("CANCELADO");
    part.setGolesLocal(0);
    part.setGolesVisitante(0);
    part.setNumGoles(0);

    return actualizarPartido(part) && actualizarEquipo(local) && actualizarEquipo(visitante);
}


// ============================================================================
// 7. MANTENIMIENTO DE INTEGRIDAD REFERENCIAL Y BACKUPS 
// ============================================================================

void verificarIntegridadReferencial() {
    int equiposVerificados = 0, jugadoresVerificados = 0, partidosVerificados = 0;
    int rotas = 0;

    cout << "\n--- CORRIENDO VERIFICACIÓN DE INTEGRIDAD REFERENCIAL ---\n";

    // Jugadores -> Equipos
    ifstream fJug(FILE_JUGADORES, ios::binary);
    if (fJug) {
        ArchivoHeader h; fJug.read(reinterpret_cast<char*>(&h), sizeof(ArchivoHeader));
        for (int i = 0; i < h.cantidadRegistros; i++) {
            Jugador j; fJug.read(reinterpret_cast<char*>(&j), sizeof(Jugador));
            if (!j.getEliminado()) {
                jugadoresVerificados++;
                Equipo eq;
                if (!leerEquipoPorID(j.getIdEquipo(), eq)) {
                    cout << "ROTO: Jugador ID " << j.getId() << " (" << j.getNombre() << ") apunta a Equipo ID " << j.getIdEquipo() << " (no existe)\n";
                    rotas++;
                }
            }
        }
        fJug.close();
    }

    // Partidos -> Equipos / Goles -> Jugadores
    ifstream fPart(FILE_PARTIDOS, ios::binary);
    if (fPart) {
        ArchivoHeader h; fPart.read(reinterpret_cast<char*>(&h), sizeof(ArchivoHeader));
        for (int i = 0; i < h.cantidadRegistros; i++) {
            Partido p; fPart.read(reinterpret_cast<char*>(&p), sizeof(Partido));
            if (!p.getEliminado()) {
                partidosVerificados++;
                Equipo loc, vis;
                if (!leerEquipoPorID(p.getIdEquipoLocal(), loc)) {
                    cout << "ROTO: Partido ID " << p.getId() << " apunta a Equipo Local ID " << p.getIdEquipoLocal() << " (no existe)\n";
                    rotas++;
                }
                if (!leerEquipoPorID(p.getIdEquipoVisitante(), vis)) {
                    cout << "ROTO: Partido ID " << p.getId() << " apunta a Equipo Visitante ID " << p.getIdEquipoVisitante() << " (no existe)\n";
                    rotas++;
                }

                const Gol* listaGoles = p.getGoles();
                for (int g = 0; g < p.getNumGoles(); g++) {
                    if (listaGoles[g].getIdJugador() > 0) {
                        Jugador jug;
                        if (!leerJugadorPorID(listaGoles[g].getIdJugador(), jug)) {
                            cout << "ROTO: Partido ID " << p.getId() << " registra Gol de Jugador ID " << listaGoles[g].getIdJugador() << " (no existe)\n";
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


// ============================================================================
// 8. REPORTES Y TABLAS DE POSICIONES
// ============================================================================

void ordenarEquiposPorBurbuja(Equipo arr[], int tam) {
    for (int i = 0; i < tam - 1; i++) {
        for (int j = 0; j < tam - i - 1; j++) {
            bool intercambiar = false;
            int difJ = arr[j].getGolesAFavor() - arr[j].getGolesEnContra();
            int difSig = arr[j+1].getGolesAFavor() - arr[j+1].getGolesEnContra();

            if (arr[j].getPuntos() < arr[j+1].getPuntos()) {
                intercambiar = true;
            } else if (arr[j].getPuntos() == arr[j+1].getPuntos()) {
                if (difJ < difSig) {
                    intercambiar = true;
                } else if (difJ == difSig) {
                    if (arr[j].getGolesAFavor() < arr[j+1].getGolesAFavor()) {
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
        if (!eq.getEliminado()) {
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
        int jugados = tempArray[i].getVictorias() + tempArray[i].getEmpates() + tempArray[i].getDerrotas();
        printf("║ %2d ║ %-24s ║ %3d ║ %1d ║ %1d ║ %1d ║ %1d ║ %2d ║ %2d ║\n",
               i + 1, tempArray[i].getNombre(), tempArray[i].getPuntos(), jugados,
               tempArray[i].getVictorias(), tempArray[i].getEmpates(), tempArray[i].getDerrotas(),
               tempArray[i].getGolesAFavor(), tempArray[i].getGolesEnContra());
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
        if (!j.getEliminado()) {
            tempArray[idx++] = j;
        }
    }
    archivo.close();

    // Ordenar jugadores por goles
    for (int i = 0; i < idx - 1; i++) {
        for (int j = 0; j < idx - i - 1; j++) {
            if (tempArray[j].getGolesAnotados() < tempArray[j+1].getGolesAnotados()) {
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
        if (leerEquipoPorID(tempArray[i].getIdEquipo(), eq)) {
            nombreEquipo = eq.getNombre();
        }
        printf("║ %2d ║ %-16s ║ %-12s ║   %2d  ║\n", i + 1, tempArray[i].getNombre(), nombreEquipo, tempArray[i].getGolesAnotados());
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
    const char* nombreLocal = leerEquipoPorID(p.getIdEquipoLocal(), loc) ? loc.getNombre() : "Desconocido";
    const char* nombreVisitante = leerEquipoPorID(p.getIdEquipoVisitante(), vis) ? vis.getNombre() : "Desconocido";

    cout << "\n╔══════════════════════════════════════════════════════════╗\n";
    cout << "║              FICHA TÉCNICA DEL PARTIDO                   ║\n";
    cout << "╠══════════════════════════════════════════════════════════╣\n";
    printf("║  Partido ID  : %-10d Fecha: %-21s ║\n", p.getId(), p.getFecha());
    printf("║  Estado      : %-40s ║\n", p.getEstado());
    cout << "╠══════════════════════════════════════════════════════════╣\n";
    printf("║  %-21s %2d  -  %-2d %-21s ║\n", nombreLocal, p.getGolesLocal(), p.getGolesVisitante(), nombreVisitante);
    cout << "╠══════════════════════════════════════════════════════════╣\n";
    cout << "║  GOLES:                                                  ║\n";

    const Gol* listaGoles = p.getGoles();
    for (int i = 0; i < p.getNumGoles(); i++) {
        const char* nombreJugador = "Jugador desconocido";
        Jugador jug;
        if (listaGoles[i].getIdJugador() > 0 && leerJugadorPorID(listaGoles[i].getIdJugador(), jug)) {
            nombreJugador = jug.getNombre();
        }
        printf("║  [%-10s] Min. %3d - %-32s ║\n", listaGoles[i].getEquipo(), listaGoles[i].getMinuto(), nombreJugador);
    }
    cout << "╚══════════════════════════════════════════════════════════╝\n";
}


// ============================================================================
// 9. CAPA DE PRESENTACIÓN / MENÚS
// ============================================================================

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
    char nom[100], ciu[100], ent[100];
    
    leerCadenaObligatoria(nom, 100, "Nombre del Equipo");
    
    Equipo control[100];
    int hallados = buscarEquiposPorNombre(nom, control, 100);
    for (int i = 0; i < hallados; i++) {
        if (compararCadenas(control[i].getNombre(), nom)) {
            cout << "ERROR: Ya existe un equipo con ese nombre.\n";
            return;
        }
    }

    leerCadenaObligatoria(ciu, 100, "Ciudad");
    leerCadenaObligatoria(ent, 100, "Entrenador");
    
    Equipo eq(nom, ciu, ent);

    if (solicitarConfirmacion("¿Desea guardar este equipo?")) {
        if (guardarEquipo(eq)) {
            cout << "Equipo registrado con éxito con el ID: " << eq.getId() << "\n";
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
            eq.mostrarCompleto();
            cout << "[DEBUG OFFSET] Leido en byte: " << sizeof(ArchivoHeader) + ((id - 1) * sizeof(Equipo)) << "\n";
        } else {
            cout << "No encontrado.\n";
        }
    } else {
        char bus[100]; leerCadenaObligatoria(bus, 100, "nombre parcial");
        Equipo resultados[MAX_RESULTADOS];
        int n = buscarEquiposPorNombre(bus, resultados, MAX_RESULTADOS);
        for (int i = 0; i < n; i++) {
            resultados[i].mostrarBasico();
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
    cout << "Actual: " << eq.getNombre() << "\n";
    char buffer[100];
    leerCadenaObligatoria(buffer, 100, "Nuevo Nombre"); eq.setNombre(buffer);
    leerCadenaObligatoria(buffer, 100, "Nueva Ciudad"); eq.setCiudad(buffer);
    leerCadenaObligatoria(buffer, 100, "Nuevo Entrenador"); eq.setEntrenador(buffer);
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
        if (!eq.getEliminado()) {
            eq.mostrarBasico();
        }
    }
    archivo.close();
}

// --- MENÚS CRUD DE JUGADORES ---

void menuRegistrarJugador() {
    cout << "\n--- REGISTRAR NUEVO JUGADOR ---\n";
    int idEq;
    cout << "ID de Equipo: "; cin >> idEq; limpiarBuffer();
    Equipo eq;
    if (!leerEquipoPorID(idEq, eq)) {
        cout << "ERROR: El equipo no existe o esta eliminado.\n"; return;
    }

    char nom[100], ced[20], pos[20];
    int edad, dorsal;

    leerCadenaObligatoria(nom, 100, "Nombre Completo");
    leerCadenaObligatoria(ced, 20, "Cedula");
    
    ifstream f(FILE_JUGADORES, ios::binary);
    if (f) {
        ArchivoHeader h; f.read(reinterpret_cast<char*>(&h), sizeof(ArchivoHeader));
        for (int i = 0; i < h.cantidadRegistros; i++) {
            Jugador temp; f.read(reinterpret_cast<char*>(&temp), sizeof(Jugador));
            if (!temp.getEliminado() && compararCadenas(temp.getCedula(), ced)) {
                cout << "ERROR: La cédula ya está registrada.\n";
                f.close(); return;
            }
        }
        f.close();
    }

    leerCadenaObligatoria(pos, 20, "Posicion (PORTERO, DEFENSA, etc.)");
    cout << "Edad: "; cin >> edad;
    cout << "Numero Dorsal: "; cin >> dorsal; limpiarBuffer();

    Jugador j(idEq, nom, ced, pos, edad, dorsal);

    if (guardarJugador(j)) {
        cout << "Jugador guardado exitosamente con ID: " << j.getId() << "\n";
    }
}

void menuListarJugadores() {
    ifstream f(FILE_JUGADORES, ios::binary);
    if (!f) return;
    ArchivoHeader h; f.read(reinterpret_cast<char*>(&h), sizeof(ArchivoHeader));
    cout << "\n--- LISTADO GENERAL DE JUGADORES ---\n";
    for (int i = 0; i < h.cantidadRegistros; i++) {
        Jugador j; f.read(reinterpret_cast<char*>(&j), sizeof(Jugador));
        if (!j.getEliminado()) {
            Equipo eq;
            const char* nEq = leerEquipoPorID(j.getIdEquipo(), eq) ? eq.getNombre() : "N/A";
            cout << "ID: " << j.getId() << " | " << j.getNombre() << " | Equipo: " << nEq << " | Dorsal: " << j.getNumeroDorsal() << "\n";
        }
    }
    f.close();
}

// --- MENÚS CRUD DE PARTIDOS ---

void menuProgramarPartido() {
    cout << "\n--- PROGRAMAR NUEVO PARTIDO ---\n";
    int idLocal, idVisitante;
    cout << "ID Equipo Local: "; cin >> idLocal;
    cout << "ID Equipo Visitante: "; cin >> idVisitante; limpiarBuffer();

    if (idLocal == idVisitante) {
        cout << "ERROR: No puede ser el mismo equipo.\n"; return;
    }

    Equipo loc, vis;
    if (!leerEquipoPorID(idLocal, loc) || !leerEquipoPorID(idVisitante, vis)) {
        cout << "ERROR: Uno o ambos equipos no existen.\n"; return;
    }

    char fecha[11], desc[200];
    leerCadenaObligatoria(fecha, 11, "Fecha (AAAA-MM-DD)");
    leerCadenaObligatoria(desc, 200, "Descripción");

    Partido p(idLocal, idVisitante, fecha, desc);

    if (guardarPartido(p)) {
        cout << "Partido programado con éxito con ID: " << p.getId() << "\n";
    }
}

void menuRegistrarResultadoCompleto() {
    cout << "\n--- REGISTRAR RESULTADO DE PARTIDO ---\n";
    int idPartido; 
    cout << "ID del Partido: "; cin >> idPartido; limpiarBuffer();
    
    Partido p;
    if (!leerPartidoPorID(idPartido, p)) {
        cout << "No existe o ya no esta disponible.\n"; return;
    }

    int gl, gv;
    cout << "Goles Local: "; cin >> gl;
    cout << "Goles Visitante: "; cin >> gv;

    Gol detalle[22];
    int totalGoles = gl + gv;
    if (totalGoles > 22) totalGoles = 22;

    for (int i = 0; i < totalGoles; i++) {
        cout << "\nDetalle del Gol #" << i+1 << ":\n";
        int idJug, min;
        char eqNombre[12];
        cout << "ID del Jugador anotador (0 si es desconocido/propia puerta): ";
        cin >> idJug;
        cout << "Minuto (1-120): "; cin >> min; limpiarBuffer();
        leerCadenaObligatoria(eqNombre, 12, "Equipo (LOCAL o VISITANTE)");
        
        detalle[i] = Gol(idJug, min, eqNombre);
    }

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
        if (!p.getEliminado()) {
            Equipo loc, vis;
            const char* nLoc = leerEquipoPorID(p.getIdEquipoLocal(), loc) ? loc.getNombre() : "N/A";
            const char* nVis = leerEquipoPorID(p.getIdEquipoVisitante(), vis) ? vis.getNombre() : "N/A";
            cout << "ID: " << p.getId() << " | " << nLoc << " " << p.getGolesLocal() << " - " << p.getGolesVisitante() << " " << nVis << " | Estado: " << p.getEstado() << "\n";
        }
    }
    f.close();
}


// ============================================================================
// 10. BUCLE PRINCIPAL (MAIN)
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