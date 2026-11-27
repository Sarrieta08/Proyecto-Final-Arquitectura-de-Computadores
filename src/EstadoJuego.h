#pragma once
#include "Util.h"

struct Nave {
    int fila;
    int columna;
    int ancho;
    int alto;
    int recarga;
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
    int direccion;
    bool activa;
    int acumulador;
    int periodo;
    TipoBala tipo;
};

struct Barrera {
    int fila;
    int columna;
    int celdas[ALTO_BARRERA][ANCHO_BARRERA];
};

struct DatosPartida {
    int puntaje;
    int puntajeMax;
    int nivel;
    int accMovAliens;
    int periodoAliens;
    int accDisparo;
    int periodoDisparo;
    int direccionAliens;
    bool invasion;
};

class EstadoJuego {
public:
    Nave nave;
    Alien aliens[MAX_ALIENS];
    Bala balas[MAX_BALAS];
    Barrera barreras[MAX_BARRERAS];
    DatosPartida datos;

    EstadoJuego() {
        datos.puntaje = 0;
        datos.puntajeMax = 0;
        nave.vidas = VIDAS_INICIALES;
        reiniciar(1);
    }

    void reiniciar(int nuevoNivel) {
        int puntajeAnterior = datos.puntaje;
        int puntajeMaxAnterior = datos.puntajeMax;
        int vidasAnteriores = nave.vidas;
        
        if (nuevoNivel > 1 && vidasAnteriores < VIDAS_INICIALES) {
            vidasAnteriores++;
        }

        if (nuevoNivel <= 1) {
            puntajeAnterior = 0;
            vidasAnteriores = VIDAS_INICIALES;
            nuevoNivel = 1;
        }

        nave.fila = FILA_NAVE;
        nave.columna = COLUMNA_INICIAL_NAVE;
        nave.ancho = ANCHO_NAVE;
        nave.alto = ALTO_NAVE;
        nave.recarga = 0;
        nave.vidas = vidasAnteriores;

        for (int i = 0; i < MAX_ALIENS; i++) {
            int fila = i / COLUMNAS_ALIENS;
            int columna = i % COLUMNAS_ALIENS;

            aliens[i].fila = FILA_INICIAL_ALIENS + fila * PASO_FILA_ALIEN;
            aliens[i].columna = COLUMNA_INICIAL_ALIENS + columna * PASO_COLUMNA_ALIEN;
            aliens[i].vivo = true;

            if (fila == 0) aliens[i].puntos = PUNTOS_FILA_0;
            else if (fila == 1) aliens[i].puntos = PUNTOS_FILA_1;
            else if (fila == 2) aliens[i].puntos = PUNTOS_FILA_2;
            else if (fila == 3) aliens[i].puntos = PUNTOS_FILA_3;
            else aliens[i].puntos = PUNTOS_FILA_4;
        }

        for (int i = 0; i < MAX_BALAS; i++) {
            balas[i].fila = 0;
            balas[i].columna = 0;
            balas[i].direccion = 0;
            balas[i].activa = false;
            balas[i].acumulador = 0;
            balas[i].periodo = 0;
            balas[i].tipo = TipoBala::ENEMIGA;
        }

        balas[INDICE_BALA_JUGADOR].tipo = TipoBala::JUGADOR;
        balas[INDICE_BALA_JUGADOR].direccion = DIRECCION_ARRIBA;
        balas[INDICE_BALA_JUGADOR].periodo = PERIODO_BALA_JUGADOR_MS;

        int columnas[MAX_BARRERAS] = {
            COLUMNA_BARRERA_0,
            COLUMNA_BARRERA_1,
            COLUMNA_BARRERA_2,
            COLUMNA_BARRERA_3
        };

        for (int b = 0; b < MAX_BARRERAS; b++) {
            barreras[b].fila = FILA_BARRERAS;
            barreras[b].columna = columnas[b];

            for (int f = 0; f < ALTO_BARRERA; f++) {
                for (int c = 0; c < ANCHO_BARRERA; c++) {
                    barreras[b].celdas[f][c] = RESISTENCIA_CELDA;
                }
            }
        }

        datos.puntaje = puntajeAnterior;
        datos.puntajeMax = puntajeMaxAnterior;
        datos.nivel = nuevoNivel;
        datos.accMovAliens = 0;
        datos.periodoAliens = PERIODO_ALIENS_INICIAL_MS - (nuevoNivel - 1) * REDUCCION_PERIODO_NIVEL_MS;
        if (datos.periodoAliens < PERIODO_ALIENS_MIN_MS) {
            datos.periodoAliens = PERIODO_ALIENS_MIN_MS;
        }
        datos.accDisparo = 0;
        datos.periodoDisparo = PERIODO_DISPARO_ALIEN_MAX_MS;
        datos.direccionAliens = 1;
        datos.invasion = false;
    }
};
