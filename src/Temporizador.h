// Controla la velocidad del juego (FPS) y devuelve cuanto tardo cada cuadro.
#pragma once

class Temporizador {
public:
    Temporizador();
    int calcularDt();   // espera lo que falte y devuelve los ms que pasaron

private:
    long ultimoTiempo;
    long ahoraMs();
};
