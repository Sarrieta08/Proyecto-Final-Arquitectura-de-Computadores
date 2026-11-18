#pragma once

// =============================================================================
//  Util.h 
//  Valores compartidos por todas las clases. Si hay que cambiar un valor,
//  se cambia solo aqui.
//  Coordenadas: fila 0 = arriba, columna 0 = izquierda. Tiempos en ms.
// =============================================================================

// ----- Pantalla -----
#define ANCHO_PANTALLA   80
#define ALTO_PANTALLA    24
#define FILA_HUD         0                    // puntaje, maximo, vidas, nivel
#define FILA_LIMITE_SUP  1                    // la bala del jugador desaparece al pasar de aqui
#define FILA_NAVE        (ALTO_PANTALLA - 2)  // 22
#define FILA_LIMITE_INF  (ALTO_PANTALLA - 1)  // las balas enemigas desaparecen al pasar de aqui
#define COLUMNA_MIN      0
#define COLUMNA_MAX      (ANCHO_PANTALLA - 1) // 79

// ----- Tiempo -----
#define FPS                 30
#define DURACION_CUADRO_MS  (1000 / FPS)      // 33 ms

// ----- Nave -----
#define ANCHO_NAVE            5
#define ALTO_NAVE             1
#define COLUMNA_INICIAL_NAVE  ((ANCHO_PANTALLA - ANCHO_NAVE) / 2)  // 37
#define PASO_NAVE             1               // columnas por movimiento
#define VIDAS_INICIALES       3
#define RECARGA_NAVE_MS       400             // tiempo minimo entre disparos

// ----- Aliens -----
#define FILAS_ALIENS            5
#define COLUMNAS_ALIENS         11
#define MAX_ALIENS              (FILAS_ALIENS * COLUMNAS_ALIENS)  // 55
#define ANCHO_ALIEN             3
#define ALTO_ALIEN              1
#define PASO_COLUMNA_ALIEN      5             // distancia entre columnas de la formacion
#define PASO_FILA_ALIEN         2             // distancia entre filas de la formacion
#define FILA_INICIAL_ALIENS     3
#define COLUMNA_INICIAL_ALIENS  5
#define DESPLAZAMIENTO_ALIEN    1             // columnas por movimiento
#define BAJADA_ALIEN            1             // filas que baja al tocar un borde

// Movimiento del grupo: entre menos aliens vivos, menor el periodo (mas rapido)
//   periodo = PERIODO_ALIENS_MIN_MS + (periodoNivel - PERIODO_ALIENS_MIN_MS) * vivos / MAX_ALIENS
//   periodoNivel = PERIODO_ALIENS_INICIAL_MS - (nivel - 1) * REDUCCION_PERIODO_NIVEL_MS
#define PERIODO_ALIENS_INICIAL_MS   500
#define PERIODO_ALIENS_MIN_MS       60
#define REDUCCION_PERIODO_NIVEL_MS  50

// Puntos segun la fila del alien (fila 0 = la de arriba)
#define PUNTOS_FILA_0  30
#define PUNTOS_FILA_1  20
#define PUNTOS_FILA_2  20
#define PUNTOS_FILA_3  10
#define PUNTOS_FILA_4  10

// ----- Disparo de los aliens (periodo aleatorio entre MIN y MAX) -----
#define PERIODO_DISPARO_ALIEN_MIN_MS  600
#define PERIODO_DISPARO_ALIEN_MAX_MS  1500

// ----- Balas -----
#define MAX_BALAS_JUGADOR        1
#define MAX_BALAS_ENEMIGAS       3
#define MAX_BALAS                (MAX_BALAS_JUGADOR + MAX_BALAS_ENEMIGAS)  // 4
#define INDICE_BALA_JUGADOR      0            // balas[0] = jugador, el resto enemigas
#define PERIODO_BALA_JUGADOR_MS  30
#define PERIODO_BALA_ENEMIGA_MS  80
#define DIRECCION_ARRIBA         -1
#define DIRECCION_ABAJO          1

// ----- Barreras -----
#define MAX_BARRERAS       4
#define ANCHO_BARRERA      7
#define ALTO_BARRERA       3
#define FILA_BARRERAS      (FILA_NAVE - 4)    // 18
#define COLUMNA_BARRERA_0  11
#define COLUMNA_BARRERA_1  27
#define COLUMNA_BARRERA_2  43
#define COLUMNA_BARRERA_3  59
#define RESISTENCIA_CELDA  2                  // 2 = intacta, 1 = danada, 0 = destruida

// ----- Colisiones -----
#define MAX_IMPACTOS  (MAX_BALAS * 2 + MAX_ALIENS)
#define SIN_INDICE    -1                      // impacto sin objetivo en un arreglo (nave, invasion)

// ----- Teclado (las flechas se leen con KEY_LEFT / KEY_RIGHT de ncurses) -----
#define SIN_TECLA       -1                    // igual que ERR de ncurses
#define TECLA_IZQ       'a'
#define TECLA_DER       'd'
#define TECLA_DISPARO   ' '
#define TECLA_PAUSA     'p'
#define TECLA_SALIR     'q'
#define TECLA_ENTER     '\n'
#define TECLA_ENTER_CR  '\r'

// ----- Simbolos de dibujo -----
#define SPRITE_NAVE             "/-^-\\"
#define SPRITE_ALIEN_FILA_0     "{@}"
#define SPRITE_ALIEN_FILA_1_2   "/O\\"
#define SPRITE_ALIEN_FILA_3_4   "<->"
#define SIMBOLO_BALA_JUGADOR    '|'
#define SIMBOLO_BALA_ENEMIGA    '!'
#define SIMBOLO_BARRERA_SANA    '#'
#define SIMBOLO_BARRERA_DANADA  '+'
#define SIMBOLO_VACIO           ' '

// ----- Enumeraciones -----
enum class Accion      { NINGUNA, IZQUIERDA, DERECHA, DISPARAR, CONFIRMAR, PAUSA, SALIR };
enum class Pantalla    { MENU, JUEGO, PAUSA, GAME_OVER, NIVEL_COMPLETADO };
enum class Evento      { NINGUNO, FIN_PARTIDA, NIVEL_COMPLETADO };
enum class TipoBala    { JUGADOR, ENEMIGA };
enum class TipoImpacto { BALA_ALIEN, BALA_NAVE, BALA_BARRERA, ALIEN_BARRERA, INVASION };