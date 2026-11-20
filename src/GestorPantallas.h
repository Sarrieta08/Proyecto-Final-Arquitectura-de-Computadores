// GestorPantallas.h
// Dice en que pantalla estamos (menu, juego, pausa, etc).
//
//   MENU --CONFIRMAR--> JUEGO --PAUSA--> PAUSA --PAUSA--> JUEGO
//   MENU --SALIR (Q)--> orden de cierre
//   JUEGO --FIN_PARTIDA--> GAME_OVER --CONFIRMAR--> MENU
//   JUEGO --NIVEL_COMPLETADO--> NIVEL_COMPLETADO --CONFIRMAR--> JUEGO

#pragma once
#include "Util.h"

class GestorPantallas {
public:
    GestorPantallas() {
        pantallaActiva = Pantalla::MENU;
        pantallaAnterior = Pantalla::MENU;
        ordenCierre = false;
    }

    Pantalla getPantalla() {
        return pantallaActiva;
    }

    // la pantalla que habia antes de la ultima llamada a actualizar()
    Pantalla getAnterior() {
        return pantallaAnterior;
    }

    // true cuando el jugador pidio salir desde el menu (tecla Q)
    bool hayOrdenCierre() {
        return ordenCierre;
    }

    void actualizar(Accion a, Evento e) {
        pantallaAnterior = pantallaActiva;

        if (pantallaActiva == Pantalla::MENU) {
            if (a == Accion::CONFIRMAR) {
                pantallaActiva = Pantalla::JUEGO;
            }
            else if (a == Accion::SALIR) {
                ordenCierre = true;
            }
        }
        else if (pantallaActiva == Pantalla::JUEGO) {
            // los eventos van primero que las acciones;
            // si llegan los dos eventos juntos gana el fin de partida
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

private:
    Pantalla pantallaActiva;
    Pantalla pantallaAnterior;
    bool ordenCierre;
};
