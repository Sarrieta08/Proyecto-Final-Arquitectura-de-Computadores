#include "GestorPantallas.h"

GestorPantallas::GestorPantallas() {
    pantallaActiva = Pantalla::MENU;
}

Pantalla GestorPantallas::getPantalla() {
    return pantallaActiva;
}

void GestorPantallas::actualizar(Accion a, Evento e) {
    if (pantallaActiva == Pantalla::MENU) {
        if (a == Accion::CONFIRMAR) {
            pantallaActiva = Pantalla::JUEGO;
        }
    }
    else if (pantallaActiva == Pantalla::JUEGO) {
        // los eventos van primero que las acciones
        if (e == Evento::FIN_PARTIDA) {
            pantallaActiva = Pantalla::GAME_OVER;
        }
        else if (e == Evento::NIVEL_COMPLETADO) {
            pantallaActiva = Pantalla::NIVEL_COMPLETADO;
        }
        else if (a == Accion::PAUSA) {
            pantallaActiva = Pantalla::PAUSA;
        }
    }
    else if (pantallaActiva == Pantalla::PAUSA) {
        if (a == Accion::PAUSA || a == Accion::CONFIRMAR) {
            pantallaActiva = Pantalla::JUEGO;
        }
    }
    else if (pantallaActiva == Pantalla::GAME_OVER) {
        if (a == Accion::CONFIRMAR) {
            pantallaActiva = Pantalla::MENU;
        }
    }
    else if (pantallaActiva == Pantalla::NIVEL_COMPLETADO) {
        if (a == Accion::CONFIRMAR) {
            pantallaActiva = Pantalla::JUEGO;
        }
    }
}
