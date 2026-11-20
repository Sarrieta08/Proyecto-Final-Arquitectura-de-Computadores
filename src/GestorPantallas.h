// GestorPantallas.h - Persona 2
// Dice en que pantalla estamos (menu, juego, pausa, etc).
//
//   MENU --CONFIRMAR--> JUEGO --PAUSA--> PAUSA --PAUSA--> JUEGO
//   JUEGO --FIN_PARTIDA--> GAME_OVER --CONFIRMAR--> MENU
//   JUEGO --NIVEL_COMPLETADO--> NIVEL_COMPLETADO --CONFIRMAR--> JUEGO
#pragma once
#include "Util.h"

class GestorPantallas {
public:
    GestorPantallas() {
        pantallaActiva = Pantalla::MENU;
    }

    Pantalla getPantalla() {
        return pantallaActiva;
    }

    void actualizar(Accion a, Evento e) {
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

private:
    Pantalla pantallaActiva;
};
