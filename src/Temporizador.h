// Temporizador.h - Persona 2
// Controla la velocidad del juego (FPS) y devuelve cuanto tardo cada cuadro.
#pragma once
#include "Util.h"
#include <time.h>

class Temporizador {
public:
    Temporizador() {
        ultimoTiempo = ahoraMs();
    }

    // espera lo que falte y devuelve los ms que pasaron
    int calcularDt() {
        long ahora = ahoraMs();
        long dt = ahora - ultimoTiempo;

        // si el cuadro fue muy rapido, esperamos lo que falta
        if (dt < DURACION_CUADRO_MS) {
            struct timespec espera;
            espera.tv_sec = 0;
            espera.tv_nsec = (DURACION_CUADRO_MS - dt) * 1000000;
            nanosleep(&espera, NULL);
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
    long ultimoTiempo;

    long ahoraMs() {
        struct timespec ts;
        clock_gettime(CLOCK_MONOTONIC, &ts);
        return ts.tv_sec * 1000 + ts.tv_nsec / 1000000;
    }
};
