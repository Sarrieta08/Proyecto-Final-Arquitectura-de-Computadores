// Renderizado
// Dibuja el juego en la consola. Solo LEE el EstadoJuego, no lo cambia.

#include "Tipos.h"

class Renderizado {
public:
    Renderizado(EstadoJuego& e);
    ~Renderizado();

    void prepararCuadro();
    void dibujarEntidades();
    void dibujarInterfaz();
    void dibujarPantalla(Pantalla p);
    void refrescar();

private:
    char cuadro[ALTO_PANTALLA][ANCHO_PANTALLA];
    EstadoJuego& estado;

    void ponerChar(int fila, int col, char c);
    void escribirTexto(int fila, int col, const char* texto);
    void escribirCentrado(int fila, const char* texto);
    void dibujarCaja(int fila, int col, int alto, int ancho);
};
