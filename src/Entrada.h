#pragma once
#include "Util.h"
#include <ncurses.h>

class Entrada {
public:
    int leerTecla() {
        return getch();
    }

    Accion traducir(int tecla) {
        if (tecla == KEY_LEFT || tecla == TECLA_IZQ) {
            return Accion::IZQUIERDA;
        }

        if (tecla == KEY_RIGHT || tecla == TECLA_DER) {
            return Accion::DERECHA;
        }

        if (tecla == TECLA_DISPARO) {
            return Accion::DISPARAR;
        }

        if (tecla == TECLA_PAUSA) {
            return Accion::PAUSA;
        }

        if (tecla == TECLA_SALIR) {
            return Accion::SALIR;
        }

        if (tecla == TECLA_ENTER || tecla == TECLA_ENTER_CR) {
            return Accion::CONFIRMAR;
        }

        return Accion::NINGUNA;
    }
};
