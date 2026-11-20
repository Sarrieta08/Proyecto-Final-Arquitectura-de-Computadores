#pragma once
#include <cstdlib>
#include <ctime>
#include "EstadoJuego.h"
#include "Entrada.h"
#include "Temporizador.h"
#include "GestorPantallas.h"
#include "LogicaFisica.h"
#include "Renderizado.h"

class Juego {
public:
    Juego() : logica(&estado), render(estado) {
        corriendo = false;
    }

    void inicializar() {
        srand((unsigned int)time(NULL));
        corriendo = true;
    }

    void ejecutar() {
        while (corriendo) {
            int dt = tiempo.calcularDt();
            int tecla = entrada.leerTecla();
            Accion accion = entrada.traducir(tecla);

            if (accion == Accion::SALIR) {
                corriendo = false;
                break;
            }

            Pantalla anterior = pantallas.getPantalla();
            Evento evento = Evento::NINGUNO;

            if (anterior == Pantalla::JUEGO && accion != Accion::PAUSA) {
                evento = logica.actualizar(accion, dt);
            }

            pantallas.actualizar(accion, evento);
            Pantalla actual = pantallas.getPantalla();

            if (anterior == Pantalla::MENU && actual == Pantalla::JUEGO) {
                estado.reiniciar(1);
            }
            else if (anterior == Pantalla::NIVEL_COMPLETADO && actual == Pantalla::JUEGO) {
                estado.reiniciar(estado.datos.nivel + 1);
            }

            render.prepararCuadro();
            if (actual == Pantalla::JUEGO || actual == Pantalla::PAUSA) {
                render.dibujarEntidades();
                render.dibujarInterfaz();
            }
            render.dibujarPantalla(actual);
            render.refrescar();
        }
    }

    void cerrar() {
        corriendo = false;
    }

private:
    EstadoJuego estado;
    Entrada entrada;
    Temporizador tiempo;
    GestorPantallas pantallas;
    LogicaFisica logica;
    Renderizado render;
    bool corriendo;
};
