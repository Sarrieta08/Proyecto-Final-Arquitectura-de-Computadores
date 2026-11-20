// Renderizado.h 
// Dibuja el juego en la consola
// Solo LEE el EstadoJuego, no lo cambia.
#pragma once
#include "EstadoJuego.h"
#include <ncurses.h>
#include <cstdio>
#include <cstring>

// Pares de color 
#define C_NORMAL         0
#define C_NAVE           1   // verde
#define C_ALIEN_FILA_0   2   // magenta
#define C_ALIEN_FILA_1_2 3   // cyan
#define C_ALIEN_FILA_3_4 4   // amarillo
#define C_BALA_JUGADOR   5   // blanco
#define C_BALA_ENEMIGA   6   // rojo
#define C_BARRERA_SANA   7   // verde
#define C_BARRERA_DANADA 8   // amarillo
#define C_HUD            9   // blanco
#define C_TITULO         10  // amarillo
#define C_GAME_OVER      11  // rojo
#define C_NIVEL          12  // verde
#define C_CAJA           13  // cyan

class Renderizado {
public:
    Renderizado(EstadoJuego& e) : estado(e) {
        initscr();              // arranca ncurses
        cbreak();               // las teclas llegan sin esperar ENTER
        noecho();               // no se escribe en pantalla lo que se teclea
        curs_set(0);            // esconder el cursor
        keypad(stdscr, TRUE);   // para leer KEY_LEFT / KEY_RIGHT
        nodelay(stdscr, TRUE);  // getch() no se queda esperando (devuelve ERR)
        iniciarColores();
        prepararCuadro();
    }

    ~Renderizado() {
        endwin();               // deja la terminal como estaba
    }

    // llena todo el cuadro con espacios
    void prepararCuadro() {
        for (int f = 0; f < ALTO_PANTALLA; f++) {
            for (int c = 0; c < ANCHO_PANTALLA; c++) {
                cuadro[f][c] = SIMBOLO_VACIO;
                colores[f][c] = C_NORMAL;
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
                        ponerChar(fila, col, SIMBOLO_BARRERA_SANA, C_BARRERA_SANA);
                    }
                    else if (celda == 1) {
                        ponerChar(fila, col, SIMBOLO_BARRERA_DANADA, C_BARRERA_DANADA);
                    }
                }
            }
        }

        // aliens (el sprite y el color dependen de los puntos que da)
        for (int i = 0; i < MAX_ALIENS; i++) {
            if (estado.aliens[i].vivo == false) continue;

            const char* sprite;
            short color;
            if (estado.aliens[i].puntos == PUNTOS_FILA_0) {
                sprite = SPRITE_ALIEN_FILA_0;
                color = C_ALIEN_FILA_0;
            } else if (estado.aliens[i].puntos == PUNTOS_FILA_1) {
                sprite = SPRITE_ALIEN_FILA_1_2;
                color = C_ALIEN_FILA_1_2;
            } else {
                sprite = SPRITE_ALIEN_FILA_3_4;
                color = C_ALIEN_FILA_3_4;
            }

            escribirTexto(estado.aliens[i].fila, estado.aliens[i].columna, sprite, color);
        }

        // balas
        for (int i = 0; i < MAX_BALAS; i++) {
            if (estado.balas[i].activa == false) continue;

            if (estado.balas[i].tipo == TipoBala::JUGADOR) {
                ponerChar(estado.balas[i].fila, estado.balas[i].columna,
                          SIMBOLO_BALA_JUGADOR, C_BALA_JUGADOR);
            } else {
                ponerChar(estado.balas[i].fila, estado.balas[i].columna,
                          SIMBOLO_BALA_ENEMIGA, C_BALA_ENEMIGA);
            }
        }

        // nave
        escribirTexto(estado.nave.fila, estado.nave.columna, SPRITE_NAVE, C_NAVE);
    }

    void dibujarInterfaz() {
        char linea[ANCHO_PANTALLA + 1];
        snprintf(linea, sizeof(linea), "PUNTAJE %05d   MAXIMO %05d   VIDAS %d   NIVEL %d",
                 estado.datos.puntaje, estado.datos.puntajeMax,
                 estado.nave.vidas, estado.datos.nivel);
        escribirTexto(FILA_HUD, 1, linea, C_HUD);
    }

    void dibujarPantalla(Pantalla p) {
        char texto[ANCHO_PANTALLA + 1];

        if (p == Pantalla::JUEGO) {
            
        }
        else if (p == Pantalla::MENU) {
            prepararCuadro();
            escribirCentrado(3, "S P A C E   I N V A D E R S", C_TITULO);
            escribirCentrado(7, "TABLA DE PUNTOS", C_HUD);
            escribirTexto(9, 30, SPRITE_ALIEN_FILA_0, C_ALIEN_FILA_0);
            escribirTexto(9, 36, "= 30 PUNTOS", C_HUD);
            escribirTexto(11, 30, SPRITE_ALIEN_FILA_1_2, C_ALIEN_FILA_1_2);
            escribirTexto(11, 36, "= 20 PUNTOS", C_HUD);
            escribirTexto(13, 30, SPRITE_ALIEN_FILA_3_4, C_ALIEN_FILA_3_4);
            escribirTexto(13, 36, "= 10 PUNTOS", C_HUD);
            escribirCentrado(17, "[ENTER] Jugar     [Q] Salir", C_NAVE);
            escribirCentrado(20, "Mover: flechas o A D   Disparar: ESPACIO   Pausa: P", C_NORMAL);
        }
        else if (p == Pantalla::PAUSA) {
            dibujarCaja(9, 25, 5, 30, C_CAJA);
            escribirCentrado(11, "PAUSA", C_TITULO);
            escribirCentrado(12, "[P] Continuar", C_HUD);
        }
        else if (p == Pantalla::GAME_OVER) {
            prepararCuadro();
            escribirCentrado(8, "G A M E   O V E R", C_GAME_OVER);
            snprintf(texto, sizeof(texto), "Puntaje final: %d", estado.datos.puntaje);
            escribirCentrado(11, texto, C_HUD);
            snprintf(texto, sizeof(texto), "Puntaje maximo: %d", estado.datos.puntajeMax);
            escribirCentrado(12, texto, C_TITULO);
            escribirCentrado(16, "[ENTER] Volver al menu", C_NORMAL);
        }
        else if (p == Pantalla::NIVEL_COMPLETADO) {
            prepararCuadro();
            snprintf(texto, sizeof(texto), "NIVEL %d COMPLETADO", estado.datos.nivel);
            escribirCentrado(9, texto, C_NIVEL);
            snprintf(texto, sizeof(texto), "Puntaje: %d", estado.datos.puntaje);
            escribirCentrado(12, texto, C_HUD);
            escribirCentrado(16, "[ENTER] Siguiente nivel", C_NORMAL);
        }
    }

    // manda el cuadro a la terminal
    void refrescar() {
        for (int f = 0; f < ALTO_PANTALLA; f++) {
            for (int c = 0; c < ANCHO_PANTALLA; c++) {
                short color = colores[f][c];
                if (hayColor && color != C_NORMAL) {
                    attron(COLOR_PAIR(color));
                    mvaddch(f, c, cuadro[f][c]);
                    attroff(COLOR_PAIR(color));
                } else {
                    mvaddch(f, c, cuadro[f][c]);
                }
            }
        }
        refresh();
    }

private:
    char cuadro[ALTO_PANTALLA][ANCHO_PANTALLA];     // que caracter va en cada celda
    short colores[ALTO_PANTALLA][ANCHO_PANTALLA];   // que par de color usa cada celda
    bool hayColor;
    EstadoJuego& estado;

    void iniciarColores() {
        hayColor = has_colors();
        if (!hayColor) return;

        start_color();
        init_pair(C_NAVE,           COLOR_GREEN,   COLOR_BLACK);
        init_pair(C_ALIEN_FILA_0,   COLOR_MAGENTA, COLOR_BLACK);
        init_pair(C_ALIEN_FILA_1_2, COLOR_CYAN,    COLOR_BLACK);
        init_pair(C_ALIEN_FILA_3_4, COLOR_YELLOW,  COLOR_BLACK);
        init_pair(C_BALA_JUGADOR,   COLOR_WHITE,   COLOR_BLACK);
        init_pair(C_BALA_ENEMIGA,   COLOR_RED,     COLOR_BLACK);
        init_pair(C_BARRERA_SANA,   COLOR_GREEN,   COLOR_BLACK);
        init_pair(C_BARRERA_DANADA, COLOR_YELLOW,  COLOR_BLACK);
        init_pair(C_HUD,            COLOR_WHITE,   COLOR_BLACK);
        init_pair(C_TITULO,         COLOR_YELLOW,  COLOR_BLACK);
        init_pair(C_GAME_OVER,      COLOR_RED,     COLOR_BLACK);
        init_pair(C_NIVEL,          COLOR_GREEN,   COLOR_BLACK);
        init_pair(C_CAJA,           COLOR_CYAN,    COLOR_BLACK);
    }

    // pone un caracter en el cuadro (si se sale de la pantalla no hace nada)
    void ponerChar(int fila, int col, char c, short color) {
        if (fila < 0 || fila >= ALTO_PANTALLA) return;
        if (col < COLUMNA_MIN || col > COLUMNA_MAX) return;
        cuadro[fila][col] = c;
        colores[fila][col] = color;
    }

    void escribirTexto(int fila, int col, const char* texto, short color) {
        for (int i = 0; texto[i] != '\0'; i++) {
            ponerChar(fila, col + i, texto[i], color);
        }
    }

    void escribirCentrado(int fila, const char* texto, short color) {
        int col = (ANCHO_PANTALLA - (int)strlen(texto)) / 2;
        escribirTexto(fila, col, texto, color);
    }

    void dibujarCaja(int fila, int col, int alto, int ancho, short color) {
        for (int r = 0; r < alto; r++) {
            for (int c = 0; c < ancho; c++) {
                char ch = SIMBOLO_VACIO;
                bool bordeH = (r == 0 || r == alto - 1);
                bool bordeV = (c == 0 || c == ancho - 1);
                if (bordeH && bordeV) ch = '+';
                else if (bordeH) ch = '-';
                else if (bordeV) ch = '|';
                ponerChar(fila + r, col + c, ch, color);
            }
        }
    }
};
