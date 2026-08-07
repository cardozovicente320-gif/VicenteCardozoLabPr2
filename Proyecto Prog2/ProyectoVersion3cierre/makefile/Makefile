CXX = g++
CXXFLAGS = -Wall -std=c++11 -I.
TARGET = torneo

SRCS = main.cpp \
       utilidades/Formatos.cpp \
       utilidades/Validaciones.cpp \
       torneo/Torneo.cpp \
       equipos/Equipo.cpp \
       equipos/operacionesEquipos.cpp \
       jugadores/Jugador.cpp \
       jugadores/operacionesJugadores.cpp \
       partidos/Gol.cpp \
       partidos/Partido.cpp \
       partidos/operacionesPartidos.cpp \
       persistencia/GestorArchivos.cpp \
       interfaz/Interfaz.cpp

OBJS = $(SRCS:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run 