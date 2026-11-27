# Makefile - Space Invaders (Grupo 2, IC-3101)
#
#   make          compila el juego
#   make run      compila y ejecuta el juego
#   make pruebas  compila y ejecuta las pruebas de LogicaFisica (necesita pruebas.cpp)
#   make clean    borra los ejecutables

CXX      = g++
CXXFLAGS = -std=c++17 -Wall -Wextra
LIBS     = -lncurses

EJECUTABLE = juego
HEADERS    = $(wildcard *.h)

all: $(EJECUTABLE)

$(EJECUTABLE): main.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) main.cpp -o $(EJECUTABLE) $(LIBS)

run: $(EJECUTABLE)
	./$(EJECUTABLE)

pruebas: pruebas.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) pruebas.cpp -o pruebas
	./pruebas

clean:
	rm -f $(EJECUTABLE) pruebas

.PHONY: all run pruebas clean
