// Renderizado.h - Persona 2
// Dibuja el juego en la consola. Solo LEE el EstadoJuego, no lo cambia.
//
// Uso en cada cuadro:
//   render.prepararCuadro();
//   if (p == Pantalla::JUEGO || p == Pantalla::PAUSA) {
//       render.dibujarEntidades();
//       render.dibujarInterfaz();
//   }
//   render.dibujarPantalla(p);
//   render.refrescar();
#pragma once
#include "Tipos.h"
#include <cstdio>
#include <cstring>

class Renderizado {
public:
    Renderizado(EstadoJuego& e) : estado(e) {
        printf("\033[?25l");    // esconder el cursor
        printf("\033[2J");      // limpiar la pantalla
        fflush(stdout);
        prepararCuadro();
    }

    ~Renderizado() {
        printf("\033[?25h\n");  // mostrar el cursor otra vez
        fflush(stdout);
    }

    // llena todo el cuadro con espacios
    void prepararCuadro() {
        for (int f = 0; f < ALTO_PANTALLA; f++) {
            for (int c = 0; c < ANCHO_PANTALLA; c++) {
                cuadro[f][c] = SIMBOLO_VACIO;
            }
        }
    }

    void dibujarEntidades() {
        // barreras
        for (int b = 0; b < MAX_BARRERAS; b++) {
            for (int r = 0; r < ALTO_BARRERA; r++) {
                for (int c = 0; c < ANCHO_BARRERA; c++) {
                    int celda = estado.barreras[b].celdas[r][c];
                    int fila = estado.barreras[b].fila + r;
                    int col = estado.barreras[b].columna + c;
                    if (celda == RESISTENCIA_CELDA) {
                        ponerChar(fila, col, SIMBOLO_BARRERA_SANA);
                    }
                    else if (celda == 1) {
                        ponerChar(fila, col, SIMBOLO_BARRERA_DANADA);
                    }
                }
            }
        }

        // aliens (el sprite depende de los puntos que da)
        for (int i = 0; i < MAX_ALIENS; i++) {
            if (estado.aliens[i].vivo == false) continue;

            const char* sprite;
            if (estado.aliens[i].puntos == PUNTOS_FILA_0) sprite = SPRITE_ALIEN_FILA_0;
            else if (estado.aliens[i].puntos == PUNTOS_FILA_1) sprite = SPRITE_ALIEN_FILA_1_2;
            else sprite = SPRITE_ALIEN_FILA_3_4;

            escribirTexto(estado.aliens[i].fila, estado.aliens[i].columna, sprite);
        }

        // balas
        for (int i = 0; i < MAX_BALAS; i++) {
            if (estado.balas[i].activa == false) continue;

            if (estado.balas[i].tipo == TipoBala::JUGADOR) {
                ponerChar(estado.balas[i].fila, estado.balas[i].columna, SIMBOLO_BALA_JUGADOR);
            } else {
                ponerChar(estado.balas[i].fila, estado.balas[i].columna, SIMBOLO_BALA_ENEMIGA);
            }
        }

        // nave
        escribirTexto(estado.nave.fila, estado.nave.columna, SPRITE_NAVE);
    }

    void dibujarInterfaz() {
        char linea[ANCHO_PANTALLA + 1];
        sprintf(linea, "PUNTAJE %05d   MAXIMO %05d   VIDAS %d   NIVEL %d",
                estado.puntaje, estado.puntajeMax, estado.nave.vidas, estado.nivel);
        escribirTexto(FILA_HUD, 1, linea);
    }

    void dibujarPantalla(Pantalla p) {
        char texto[ANCHO_PANTALLA + 1];

        if (p == Pantalla::JUEGO) {
            // no hay nada extra que dibujar
        }
        else if (p == Pantalla::MENU) {
            prepararCuadro();
            escribirCentrado(3, "S P A C E   I N V A D E R S");
            escribirCentrado(7, "TABLA DE PUNTOS");
            escribirTexto(9, 30, SPRITE_ALIEN_FILA_0);
            escribirTexto(9, 36, "= 30 PUNTOS");
            escribirTexto(11, 30, SPRITE_ALIEN_FILA_1_2);
            escribirTexto(11, 36, "= 20 PUNTOS");
            escribirTexto(13, 30, SPRITE_ALIEN_FILA_3_4);
            escribirTexto(13, 36, "= 10 PUNTOS");
            escribirCentrado(17, "[ENTER] Jugar     [Q] Salir");
            escribirCentrado(20, "Mover: flechas o A D   Disparar: ESPACIO   Pausa: P");
        }
        else if (p == Pantalla::PAUSA) {
            dibujarCaja(9, 25, 5, 30);
            escribirCentrado(11, "PAUSA");
            escribirCentrado(12, "[P] Continuar");
        }
        else if (p == Pantalla::GAME_OVER) {
            prepararCuadro();
            escribirCentrado(8, "G A M E   O V E R");
            sprintf(texto, "Puntaje final: %d", estado.puntaje);
            escribirCentrado(11, texto);
            sprintf(texto, "Puntaje maximo: %d", estado.puntajeMax);
            escribirCentrado(12, texto);
            escribirCentrado(16, "[ENTER] Volver al menu");
        }
        else if (p == Pantalla::NIVEL_COMPLETADO) {
            prepararCuadro();
            sprintf(texto, "NIVEL %d COMPLETADO", estado.nivel);
            escribirCentrado(9, texto);
            sprintf(texto, "Puntaje: %d", estado.puntaje);
            escribirCentrado(12, texto);
            escribirCentrado(16, "[ENTER] Siguiente nivel");
        }
    }

    // manda el cuadro a la terminal
    void refrescar() {
        printf("\033[H");   // cursor arriba a la izquierda
        for (int f = 0; f < ALTO_PANTALLA; f++) {
            for (int c = 0; c < ANCHO_PANTALLA; c++) {
                putchar(cuadro[f][c]);
            }
            if (f < ALTO_PANTALLA - 1) putchar('\n');
        }
        fflush(stdout);
    }

private:
    char cuadro[ALTO_PANTALLA][ANCHO_PANTALLA];
    EstadoJuego& estado;

    // pone un caracter en el cuadro (si se sale de la pantalla no hace nada)
    void ponerChar(int fila, int col, char c) {
        if (fila < 0 || fila >= ALTO_PANTALLA) return;
        if (col < COLUMNA_MIN || col > COLUMNA_MAX) return;
        cuadro[fila][col] = c;
    }

    void escribirTexto(int fila, int col, const char* texto) {
        for (int i = 0; texto[i] != '\0'; i++) {
            ponerChar(fila, col + i, texto[i]);
        }
    }

    void escribirCentrado(int fila, const char* texto) {
        int col = (ANCHO_PANTALLA - strlen(texto)) / 2;
        escribirTexto(fila, col, texto);
    }

    void dibujarCaja(int fila, int col, int alto, int ancho) {
        for (int r = 0; r < alto; r++) {
            for (int c = 0; c < ancho; c++) {
                char ch = SIMBOLO_VACIO;
                bool bordeH = (r == 0 || r == alto - 1);
                bool bordeV = (c == 0 || c == ancho - 1);
                if (bordeH && bordeV) ch = '+';
                else if (bordeH) ch = '-';
                else if (bordeV) ch = '|';
                ponerChar(fila + r, col + c, ch);
            }
        }
    }
};
