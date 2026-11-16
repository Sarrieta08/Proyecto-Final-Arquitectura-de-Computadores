#pragma once

// Logica y Fisica del juego
// Mueve la nave, los aliens y las balas, revisa los choques
// y avisa si se termino la partida o el nivel.

#include <cstdlib>
#include "Util.h"
#include "EstadoJuego.h"
#include "Impacto.h"

class LogicaFisica {
public:
    EstadoJuego* estado;

    LogicaFisica(EstadoJuego* e) {
        estado = e;
    }

    // Hace todo lo de un cuadro, en orden
    Evento actualizar(Accion accion, int dt) {
        procesarAccion(accion, dt);
        moverAliens(dt);
        disparoAlien(dt);
        moverBalas(dt);

        Impacto lista[MAX_IMPACTOS];
        int cantidad = detectarColisiones(lista);
        resolverImpactos(lista, cantidad);

        return verificarCondiciones();
    }

    // Mueve la nave o dispara
    void procesarAccion(Accion accion, int dt) {
        // la recarga va bajando hasta llegar a 0
        estado->nave.recarga = estado->nave.recarga - dt;
        if (estado->nave.recarga < 0) {
            estado->nave.recarga = 0;
        }

        if (accion == Accion::IZQUIERDA) {
            estado->nave.columna = estado->nave.columna - PASO_NAVE;
            if (estado->nave.columna < COLUMNA_MIN) {
                estado->nave.columna = COLUMNA_MIN;
            }
        }

        if (accion == Accion::DERECHA) {
            estado->nave.columna = estado->nave.columna + PASO_NAVE;
            int ultimaColumna = COLUMNA_MAX - estado->nave.ancho + 1;
            if (estado->nave.columna > ultimaColumna) {
                estado->nave.columna = ultimaColumna;
            }
        }

        if (accion == Accion::DISPARAR) {
            // solo se dispara si la recarga esta en 0 y no hay otra bala del jugador
            if (estado->nave.recarga == 0 && estado->balas[INDICE_BALA_JUGADOR].activa == false) {
                estado->balas[INDICE_BALA_JUGADOR].fila = estado->nave.fila - 1;
                estado->balas[INDICE_BALA_JUGADOR].columna = estado->nave.columna + estado->nave.ancho / 2;
                estado->balas[INDICE_BALA_JUGADOR].direccion = DIRECCION_ARRIBA;
                estado->balas[INDICE_BALA_JUGADOR].activa = true;
                estado->balas[INDICE_BALA_JUGADOR].acumulador = 0;
                estado->balas[INDICE_BALA_JUGADOR].periodo = PERIODO_BALA_JUGADOR_MS;
                estado->balas[INDICE_BALA_JUGADOR].tipo = TipoBala::JUGADOR;
                estado->nave.recarga = RECARGA_NAVE_MS;
            }
        }
    }

    // Mueve el grupo de aliens. Si toca un borde, baja y cambia de direccion.
    void moverAliens(int dt) {
        estado->datos.periodoAliens = calcularPeriodoAliens();
        estado->datos.accMovAliens = estado->datos.accMovAliens + dt;

        // todavia no es momento de moverse
        if (estado->datos.accMovAliens < estado->datos.periodoAliens) {
            return;
        }
        estado->datos.accMovAliens = 0;

        // buscar la columna mas a la izquierda y mas a la derecha de los aliens vivos
        int colMin = COLUMNA_MAX;
        int colMax = COLUMNA_MIN;
        int vivos = 0;
        for (int i = 0; i < MAX_ALIENS; i++) {
            if (estado->aliens[i].vivo) {
                vivos++;
                if (estado->aliens[i].columna < colMin) {
                    colMin = estado->aliens[i].columna;
                }
                int derecha = estado->aliens[i].columna + ANCHO_ALIEN - 1;
                if (derecha > colMax) {
                    colMax = derecha;
                }
            }
        }
        if (vivos == 0) {
            return;
        }

        int paso = estado->datos.direccionAliens * DESPLAZAMIENTO_ALIEN;
        bool tocaBorde = false;
        if (colMax + paso > COLUMNA_MAX) {
            tocaBorde = true;
        }
        if (colMin + paso < COLUMNA_MIN) {
            tocaBorde = true;
        }

        for (int i = 0; i < MAX_ALIENS; i++) {
            if (tocaBorde) {
                estado->aliens[i].fila = estado->aliens[i].fila + BAJADA_ALIEN;
            } else {
                estado->aliens[i].columna = estado->aliens[i].columna + paso;
            }
        }

        if (tocaBorde) {
            estado->datos.direccionAliens = estado->datos.direccionAliens * -1;
        }
    }

    // Cada cierto tiempo dispara el alien vivo mas bajo de una columna al azar
    void disparoAlien(int dt) {
        estado->datos.accDisparo = estado->datos.accDisparo + dt;
        if (estado->datos.accDisparo < estado->datos.periodoDisparo) {
            return;
        }
        estado->datos.accDisparo = 0;
        estado->datos.periodoDisparo = sortearPeriodoDisparo();

        // buscar una bala enemiga que no se este usando
        int libre = -1;
        for (int b = 1; b < MAX_BALAS; b++) {
            if (estado->balas[b].activa == false && libre == -1) {
                libre = b;
            }
        }
        if (libre == -1) {
            return;
        }

        // guardar las columnas que todavia tienen algun alien vivo
        int columnas[COLUMNAS_ALIENS];
        int cantidad = 0;
        for (int c = 0; c < COLUMNAS_ALIENS; c++) {
            bool tieneVivo = false;
            for (int f = 0; f < FILAS_ALIENS; f++) {
                if (estado->aliens[f * COLUMNAS_ALIENS + c].vivo) {
                    tieneVivo = true;
                }
            }
            if (tieneVivo) {
                columnas[cantidad] = c;
                cantidad++;
            }
        }
        if (cantidad == 0) {
            return;
        }

        int columna = columnas[rand() % cantidad];

        // buscar el alien vivo mas bajo de esa columna (de abajo hacia arriba)
        int tirador = -1;
        for (int f = FILAS_ALIENS - 1; f >= 0; f--) {
            int i = f * COLUMNAS_ALIENS + columna;
            if (estado->aliens[i].vivo && tirador == -1) {
                tirador = i;
            }
        }

        estado->balas[libre].fila = estado->aliens[tirador].fila + ALTO_ALIEN;
        estado->balas[libre].columna = estado->aliens[tirador].columna + ANCHO_ALIEN / 2;
        estado->balas[libre].direccion = DIRECCION_ABAJO;
        estado->balas[libre].activa = true;
        estado->balas[libre].acumulador = 0;
        estado->balas[libre].periodo = PERIODO_BALA_ENEMIGA_MS;
        estado->balas[libre].tipo = TipoBala::ENEMIGA;
    }

    // Mueve cada bala una fila cuando pasa su periodo
    void moverBalas(int dt) {
        for (int b = 0; b < MAX_BALAS; b++) {
            if (estado->balas[b].activa) {
                estado->balas[b].acumulador = estado->balas[b].acumulador + dt;

                if (estado->balas[b].acumulador >= estado->balas[b].periodo) {
                    estado->balas[b].fila = estado->balas[b].fila + estado->balas[b].direccion;
                    estado->balas[b].acumulador = 0;
                }

                // si se sale de la pantalla se apaga
                if (estado->balas[b].fila < FILA_LIMITE_SUP || estado->balas[b].fila > FILA_LIMITE_INF) {
                    estado->balas[b].activa = false;
                }
            }
        }
    }

    // Revisa todos los choques y los guarda en la lista.
    // Devuelve cuantos choques encontro.
    int detectarColisiones(Impacto lista[]) {
        int cantidad = 0;

        // 1) Balas
        for (int b = 0; b < MAX_BALAS; b++) {
            if (estado->balas[b].activa) {
                int filaBala = estado->balas[b].fila;
                int colBala = estado->balas[b].columna;

                if (estado->balas[b].tipo == TipoBala::JUGADOR) {
                    // bala del jugador contra aliens
                    for (int a = 0; a < MAX_ALIENS; a++) {
                        if (estado->aliens[a].vivo) {
                            if (solapan(filaBala, colBala, 1, 1,
                                        estado->aliens[a].fila, estado->aliens[a].columna, ALTO_ALIEN, ANCHO_ALIEN)) {
                                agregarImpacto(lista, cantidad, TipoImpacto::BALA_ALIEN, b, a, -1, -1);
                            }
                        }
                    }
                } else {
                    // bala enemiga contra la nave
                    if (solapan(filaBala, colBala, 1, 1,
                                estado->nave.fila, estado->nave.columna, estado->nave.alto, estado->nave.ancho)) {
                        agregarImpacto(lista, cantidad, TipoImpacto::BALA_NAVE, b, -1, -1, -1);
                    }
                }

                // cualquier bala contra las barreras
                for (int r = 0; r < MAX_BARRERAS; r++) {
                    int cf = filaBala - estado->barreras[r].fila;
                    int cc = colBala - estado->barreras[r].columna;
                    bool dentro = cf >= 0 && cf < ALTO_BARRERA && cc >= 0 && cc < ANCHO_BARRERA;
                    if (dentro && estado->barreras[r].celdas[cf][cc] > 0) {
                        agregarImpacto(lista, cantidad, TipoImpacto::BALA_BARRERA, b, r, cf, cc);
                    }
                }
            }
        }

        // 2) Aliens
        bool invasion = false;
        for (int a = 0; a < MAX_ALIENS; a++) {
            if (estado->aliens[a].vivo) {
                // alien contra las barreras
                for (int r = 0; r < MAX_BARRERAS; r++) {
                    for (int cf = 0; cf < ALTO_BARRERA; cf++) {
                        for (int cc = 0; cc < ANCHO_BARRERA; cc++) {
                            if (estado->barreras[r].celdas[cf][cc] > 0) {
                                int filaCelda = estado->barreras[r].fila + cf;
                                int colCelda = estado->barreras[r].columna + cc;
                                if (solapan(estado->aliens[a].fila, estado->aliens[a].columna, ALTO_ALIEN, ANCHO_ALIEN,
                                            filaCelda, colCelda, 1, 1)) {
                                    agregarImpacto(lista, cantidad, TipoImpacto::ALIEN_BARRERA, -1, r, cf, cc);
                                }
                            }
                        }
                    }
                }

                // el alien llego a la fila de la nave
                if (estado->aliens[a].fila + ALTO_ALIEN - 1 >= FILA_NAVE) {
                    invasion = true;
                }
            }
        }
        if (invasion) {
            agregarImpacto(lista, cantidad, TipoImpacto::INVASION, -1, -1, -1, -1);
        }

        return cantidad;
    }

    // Aplica lo que pasa con cada choque
    void resolverImpactos(Impacto lista[], int cantidad) {
        for (int i = 0; i < cantidad; i++) {
            int b = lista[i].indiceBala;
            int obj = lista[i].indiceObjetivo;

            // si la bala ya se apago por otro choque, este se ignora
            if (b != -1 && estado->balas[b].activa == false) {
                continue;
            }

            if (lista[i].tipo == TipoImpacto::BALA_ALIEN) {
                if (estado->aliens[obj].vivo) {
                    estado->aliens[obj].vivo = false;
                    estado->datos.puntaje = estado->datos.puntaje + estado->aliens[obj].puntos;
                    estado->balas[b].activa = false;
                }
            } else if (lista[i].tipo == TipoImpacto::BALA_NAVE) {
                if (estado->nave.vidas > 0) {
                    estado->nave.vidas--;
                }
                estado->balas[b].activa = false;
            } else if (lista[i].tipo == TipoImpacto::BALA_BARRERA) {
                int cf = lista[i].celdaFila;
                int cc = lista[i].celdaColumna;
                if (estado->barreras[obj].celdas[cf][cc] > 0) {
                    estado->barreras[obj].celdas[cf][cc]--;
                    estado->balas[b].activa = false;
                }
            } else if (lista[i].tipo == TipoImpacto::ALIEN_BARRERA) {
                // el alien rompe toda la celda
                int cf = lista[i].celdaFila;
                int cc = lista[i].celdaColumna;
                estado->barreras[obj].celdas[cf][cc] = 0;
            } else if (lista[i].tipo == TipoImpacto::INVASION) {
                estado->datos.invasion = true;
            }
        }
    }

    // Revisa si se termino la partida o el nivel
    Evento verificarCondiciones() {
        Evento evento = Evento::NINGUNO;

        // primero se revisa el fin de partida porque tiene prioridad
        if (estado->datos.invasion || estado->nave.vidas <= 0) {
            evento = Evento::FIN_PARTIDA;
        } else if (contarAliensVivos() == 0) {
            evento = Evento::NIVEL_COMPLETADO;
        }

        // el puntaje maximo se revisa siempre
        if (estado->datos.puntaje > estado->datos.puntajeMax) {
            estado->datos.puntajeMax = estado->datos.puntaje;
        }

        return evento;
    }

    // ---------------- funciones de ayuda ----------------

    // Revisa si dos rectangulos se tocan
    bool solapan(int fila1, int col1, int alto1, int ancho1,
                 int fila2, int col2, int alto2, int ancho2) {
        if (col1 + ancho1 <= col2) return false;   // 1 esta a la izquierda de 2
        if (col2 + ancho2 <= col1) return false;   // 1 esta a la derecha de 2
        if (fila1 + alto1 <= fila2) return false;  // 1 esta arriba de 2
        if (fila2 + alto2 <= fila1) return false;  // 1 esta abajo de 2
        return true;
    }

    // Agrega un choque al final de la lista
    void agregarImpacto(Impacto lista[], int& cantidad, TipoImpacto tipo,
                        int bala, int objetivo, int celdaFila, int celdaColumna) {
        if (cantidad < MAX_IMPACTOS) {
            lista[cantidad].tipo = tipo;
            lista[cantidad].indiceBala = bala;
            lista[cantidad].indiceObjetivo = objetivo;
            lista[cantidad].celdaFila = celdaFila;
            lista[cantidad].celdaColumna = celdaColumna;
            cantidad++;
        }
    }

    int contarAliensVivos() {
        int vivos = 0;
        for (int i = 0; i < MAX_ALIENS; i++) {
            if (estado->aliens[i].vivo) {
                vivos++;
            }
        }
        return vivos;
    }

    // Entre menos aliens quedan, mas rapido se mueven
    int calcularPeriodoAliens() {
        int periodoNivel = PERIODO_ALIENS_INICIAL_MS - (estado->datos.nivel - 1) * REDUCCION_PERIODO_NIVEL_MS;
        if (periodoNivel < PERIODO_ALIENS_MIN_MS) {
            periodoNivel = PERIODO_ALIENS_MIN_MS;
        }
        int vivos = contarAliensVivos();
        return PERIODO_ALIENS_MIN_MS + (periodoNivel - PERIODO_ALIENS_MIN_MS) * vivos / MAX_ALIENS;
    }

    // Numero al azar entre el minimo y el maximo
    int sortearPeriodoDisparo() {
        int rango = PERIODO_DISPARO_ALIEN_MAX_MS - PERIODO_DISPARO_ALIEN_MIN_MS + 1;
        return PERIODO_DISPARO_ALIEN_MIN_MS + rand() % rango;
    }
};