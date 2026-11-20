#pragma once

// EstadoJuego.h
// Guarda todos los datos del juego. No mueve nada ni toma decisiones:
// LogicaFisica lo cambia y Renderizado solo lo lee.
// (Union del Tipos.h de Daniel con el EstadoJuego del diagrama de clases)

#include "Util.h"

struct Nave {
    int fila;
    int columna;
    int ancho;
    int alto;
    int recarga;      // ms que faltan para poder disparar otra vez
    int vidas;
};

struct Alien {
    int fila;
    int columna;
    bool vivo;
    int puntos;
};

struct Bala {
    int fila;
    int columna;
    int direccion;    // DIRECCION_ARRIBA o DIRECCION_ABAJO
    bool activa;
    int acumulador;
    int periodo;
    TipoBala tipo;
};

struct Barrera {
    int fila;
    int columna;
    int celdas[ALTO_BARRERA][ANCHO_BARRERA];   // 2 = sana, 1 = danada, 0 = destruida
};

struct DatosPartida {
    int puntaje;
    int puntajeMax;
    int nivel;
    int accMovAliens;
    int periodoAliens;
    int accDisparo;
    int periodoDisparo;
    int direccionAliens;   // 1 = derecha, -1 = izquierda
    bool invasion;         // true si un alien llego a la fila de la nave
};

// Los aliens van en el orden aliens[fila * COLUMNAS_ALIENS + columna]
struct EstadoJuego {
    Nave nave;
    Alien aliens[MAX_ALIENS];
    Bala balas[MAX_BALAS];
    Barrera barreras[MAX_BARRERAS];
    DatosPartida datos;

    // Deja todo listo para empezar un nivel.
    // En el nivel 1 tambien se reinician el puntaje y las vidas.
    // El puntaje maximo no se toca (Juego lo pone en 0 una sola vez al iniciar).
    void reiniciar(int nuevoNivel) {
        if (nuevoNivel == 1) {
            datos.puntaje = 0;
            nave.vidas = VIDAS_INICIALES;
        }
        datos.nivel = nuevoNivel;
        datos.accMovAliens = 0;
        datos.periodoAliens = PERIODO_ALIENS_INICIAL_MS;
        datos.accDisparo = 0;
        datos.periodoDisparo = PERIODO_DISPARO_ALIEN_MAX_MS;
        datos.direccionAliens = 1;
        datos.invasion = false;

        nave.fila = FILA_NAVE;
        nave.columna = COLUMNA_INICIAL_NAVE;
        nave.ancho = ANCHO_NAVE;
        nave.alto = ALTO_NAVE;
        nave.recarga = 0;

        for (int i = 0; i < MAX_ALIENS; i++) {
            int f = i / COLUMNAS_ALIENS;
            int c = i % COLUMNAS_ALIENS;
            aliens[i].fila = FILA_INICIAL_ALIENS + f * PASO_FILA_ALIEN;
            aliens[i].columna = COLUMNA_INICIAL_ALIENS + c * PASO_COLUMNA_ALIEN;
            aliens[i].vivo = true;
            if (f == 0) aliens[i].puntos = PUNTOS_FILA_0;
            else if (f == 1) aliens[i].puntos = PUNTOS_FILA_1;
            else if (f == 2) aliens[i].puntos = PUNTOS_FILA_2;
            else if (f == 3) aliens[i].puntos = PUNTOS_FILA_3;
            else aliens[i].puntos = PUNTOS_FILA_4;
        }

        for (int i = 0; i < MAX_BALAS; i++) {
            balas[i].fila = 0;
            balas[i].columna = 0;
            balas[i].direccion = 0;
            balas[i].activa = false;
            balas[i].acumulador = 0;
            balas[i].periodo = PERIODO_BALA_ENEMIGA_MS;
            balas[i].tipo = TipoBala::ENEMIGA;
        }
        balas[INDICE_BALA_JUGADOR].tipo = TipoBala::JUGADOR;
        balas[INDICE_BALA_JUGADOR].periodo = PERIODO_BALA_JUGADOR_MS;

        int columnas[MAX_BARRERAS] = {COLUMNA_BARRERA_0, COLUMNA_BARRERA_1,
                                      COLUMNA_BARRERA_2, COLUMNA_BARRERA_3};
        for (int b = 0; b < MAX_BARRERAS; b++) {
            barreras[b].fila = FILA_BARRERAS;
            barreras[b].columna = columnas[b];
            for (int r = 0; r < ALTO_BARRERA; r++) {
                for (int c = 0; c < ANCHO_BARRERA; c++) {
                    barreras[b].celdas[r][c] = RESISTENCIA_CELDA;
                }
            }
        }
    }
};
