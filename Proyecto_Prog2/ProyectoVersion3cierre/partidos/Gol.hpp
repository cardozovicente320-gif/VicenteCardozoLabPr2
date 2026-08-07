#ifndef GOL_HPP
#define GOL_HPP

#include <cstddef>   // ✅ NECESARIO para size_t

class Gol {
private:
    int idJugador;
    int minuto;
    char equipo[12];

public:
    Gol();
    Gol(int idJugador, int minuto, const char* equipo);
    Gol(const Gol& other);
    ~Gol();
    
    int getIdJugador() const;
    int getMinuto() const;
    const char* getEquipo() const;
    
    bool setIdJugador(int id);
    bool setMinuto(int minuto);
    bool setEquipo(const char* equipo);
    
    bool esValido() const;
    void mostrar() const;
    static size_t obtenerTamano();   // ✅ size_t definido por <cstddef>
};

#endif