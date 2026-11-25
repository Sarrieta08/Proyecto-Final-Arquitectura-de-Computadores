// Temporizador.h - Persona 2
// Controla la velocidad del juego (FPS) y devuelve cuanto tardo cada cuadro.
// Usa solo C++ estandar (chrono y thread), asi funciona en Windows y en Linux.
#pragma once
#include "Util.h"
#include <chrono>
#include <thread>

class Temporizador {
public:
    Temporizador() {
        ultimoTiempo = ahoraMs();
    }

    // espera lo que falte y devuelve los ms que pasaron
    int calcularDt() {
        long long ahora = ahoraMs();
        long long dt = ahora - ultimoTiempo;

        // si el cuadro fue muy rapido, esperamos lo que falta
        if (dt < DURACION_CUADRO_MS) {
            std::this_thread::sleep_for(std::chrono::milliseconds(DURACION_CUADRO_MS - dt));
            ahora = ahoraMs();
            dt = ahora - ultimoTiempo;
        }

        ultimoTiempo = ahora;

        // si hubo una pausa larga no dejamos que dt sea gigante
        if (dt > 4 * DURACION_CUADRO_MS) {
            dt = 4 * DURACION_CUADRO_MS;
        }
        return (int)dt;
    }

private:
    long long ultimoTiempo;

    // milisegundos desde que arranco el reloj
    long long ahoraMs() {
        auto ahora = std::chrono::steady_clock::now().time_since_epoch();
        return std::chrono::duration_cast<std::chrono::milliseconds>(ahora).count();
    }
};
