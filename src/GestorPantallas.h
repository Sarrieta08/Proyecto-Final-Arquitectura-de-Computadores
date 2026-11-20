// GestorPantallas
// Dice en que pantalla estamos (menu, juego, pausa, etc).
#pragma once
#include "Util.h"

class GestorPantallas {
public:
    GestorPantallas();
    void actualizar(Accion a, Evento e);
    Pantalla getPantalla();

private:
    Pantalla pantallaActiva;
};
