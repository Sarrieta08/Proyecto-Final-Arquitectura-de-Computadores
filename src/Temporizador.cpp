#include "Temporizador.h"
#include "Util.h"
#include <time.h>

long Temporizador::ahoraMs() {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec * 1000 + ts.tv_nsec / 1000000;
}

Temporizador::Temporizador() {
    ultimoTiempo = ahoraMs();
}

int Temporizador::calcularDt() {
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
