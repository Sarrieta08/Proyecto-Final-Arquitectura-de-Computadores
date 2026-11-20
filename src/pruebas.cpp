// pruebas.cpp
// Pruebas de LogicaFisica
//
// Ejecutar:  ./pruebas

#include <iostream>
#include "LogicaFisica.h"

using namespace std;

int pasaron = 0;
int fallaron = 0;

void revisar(bool resultado, string nombre) {
    if (resultado) {
        cout << "[OK]    " << nombre << endl;
        pasaron++;
    } else {
        cout << "[FALLA] " << nombre << endl;
        fallaron++;
    }
}

// Deja el juego vacio: solo la nave, sin aliens, sin balas y sin barreras
void limpiar(EstadoJuego& e) {
    e = EstadoJuego{};   // pone todo en 0
    e.nave.fila = FILA_NAVE;
    e.nave.columna = COLUMNA_INICIAL_NAVE;
    e.nave.ancho = ANCHO_NAVE;
    e.nave.alto = ALTO_NAVE;
    e.nave.vidas = 3;
    e.datos.nivel = 1;
    e.datos.direccionAliens = 1;
    e.datos.periodoDisparo = 100000;   // para que los aliens no disparen solos
}

int main() {
    EstadoJuego e;
    LogicaFisica logica(&e);
    Impacto lista[MAX_IMPACTOS];
    int n;

    // ---------- Nave ----------

    // 1. La nave se mueve a la izquierda
    limpiar(e);
    logica.procesarAccion(Accion::IZQUIERDA, 33);
    revisar(e.nave.columna == 36, "la nave se mueve a la izquierda");

    // 2. La nave no se sale por la izquierda
    limpiar(e);
    e.nave.columna = 0;
    logica.procesarAccion(Accion::IZQUIERDA, 33);
    revisar(e.nave.columna == 0, "la nave no se sale por la izquierda");

    // 3. La nave dispara
    limpiar(e);
    logica.procesarAccion(Accion::DISPARAR, 33);
    revisar(e.balas[0].activa == true, "la nave dispara");

    // 4. No puede disparar dos veces seguidas (recarga)
    e.balas[0].activa = false;
    logica.procesarAccion(Accion::DISPARAR, 33);
    revisar(e.balas[0].activa == false, "no dispara dos veces seguidas");

    // ---------- Aliens ----------

    // 5. Los aliens se mueven a la derecha
    limpiar(e);
    e.aliens[0].vivo = true;
    e.aliens[0].fila = 5;
    e.aliens[0].columna = 20;
    logica.moverAliens(1000);
    revisar(e.aliens[0].columna == 21, "los aliens se mueven a la derecha");

    // 6. En el borde bajan una fila
    limpiar(e);
    e.aliens[0].vivo = true;
    e.aliens[0].fila = 5;
    e.aliens[0].columna = 77;   // pegado al borde derecho
    logica.moverAliens(1000);
    revisar(e.aliens[0].fila == 6, "en el borde los aliens bajan");

    // 7. Y cambian de direccion
    revisar(e.datos.direccionAliens == -1, "en el borde cambian de direccion");

    // ---------- Disparo de los aliens ----------

    // 8. Dispara el alien de mas abajo de la columna
    limpiar(e);
    e.aliens[4].vivo = true;        // fila 0, columna 4
    e.aliens[4].fila = 3;
    e.aliens[4].columna = 25;
    e.aliens[26].vivo = true;       // fila 2, columna 4 (mas abajo)
    e.aliens[26].fila = 7;
    e.aliens[26].columna = 25;
    e.datos.periodoDisparo = 0;
    logica.disparoAlien(33);
    revisar(e.balas[1].activa == true && e.balas[1].fila == 8, "dispara el alien de mas abajo");

    // ---------- Balas ----------

    // 9. La bala del jugador sube
    limpiar(e);
    e.balas[0].activa = true;
    e.balas[0].fila = 15;
    e.balas[0].direccion = -1;
    e.balas[0].periodo = 30;
    logica.moverBalas(33);
    revisar(e.balas[0].fila == 14, "la bala del jugador sube");

    // 10. La bala se apaga al salir de la pantalla
    e.balas[0].fila = 1;
    logica.moverBalas(33);
    revisar(e.balas[0].activa == false, "la bala se apaga al salir de la pantalla");

    // ---------- Choques ----------

    // 11. La bala le pega al alien (el alien ocupa columnas 10, 11 y 12)
    limpiar(e);
    e.aliens[0].vivo = true;
    e.aliens[0].fila = 5;
    e.aliens[0].columna = 10;
    e.aliens[0].puntos = 30;
    e.balas[0].activa = true;
    e.balas[0].tipo = TipoBala::JUGADOR;
    e.balas[0].fila = 5;
    e.balas[0].columna = 12;
    n = logica.detectarColisiones(lista);
    revisar(n == 1, "la bala le pega al alien en su borde");

    // 12. El alien muere y da puntos
    logica.resolverImpactos(lista, n);
    revisar(e.aliens[0].vivo == false && e.datos.puntaje == 30, "el alien muere y da 30 puntos");

    // 13. Una bala al lado del alien no le pega
    limpiar(e);
    e.aliens[0].vivo = true;
    e.aliens[0].fila = 5;
    e.aliens[0].columna = 10;
    e.balas[0].activa = true;
    e.balas[0].tipo = TipoBala::JUGADOR;
    e.balas[0].fila = 5;
    e.balas[0].columna = 13;
    n = logica.detectarColisiones(lista);
    revisar(n == 0, "una bala al lado del alien no le pega");

    // 14. La bala enemiga le quita una vida a la nave
    limpiar(e);
    e.balas[1].activa = true;
    e.balas[1].tipo = TipoBala::ENEMIGA;
    e.balas[1].fila = FILA_NAVE;
    e.balas[1].columna = 39;
    n = logica.detectarColisiones(lista);
    logica.resolverImpactos(lista, n);
    revisar(e.nave.vidas == 2, "la bala enemiga quita una vida");

    // 15. La barrera se dana con un golpe
    limpiar(e);
    e.barreras[0].fila = 18;
    e.barreras[0].columna = 11;
    e.barreras[0].celdas[0][0] = 2;
    e.balas[0].activa = true;
    e.balas[0].tipo = TipoBala::JUGADOR;
    e.balas[0].fila = 18;
    e.balas[0].columna = 11;
    n = logica.detectarColisiones(lista);
    logica.resolverImpactos(lista, n);
    revisar(e.barreras[0].celdas[0][0] == 1, "la barrera se dana con un golpe");

    // 16. Un alien en la fila de la nave es invasion
    limpiar(e);
    e.aliens[0].vivo = true;
    e.aliens[0].fila = FILA_NAVE;
    e.aliens[0].columna = 5;
    n = logica.detectarColisiones(lista);
    logica.resolverImpactos(lista, n);
    revisar(e.datos.invasion == true, "un alien en la fila de la nave es invasion");

    // ---------- Fin del juego ----------

    // 17. Con invasion se pierde
    revisar(logica.verificarCondiciones() == Evento::FIN_PARTIDA, "con invasion se pierde");

    // 18. Sin vidas se pierde
    limpiar(e);
    e.aliens[0].vivo = true;
    e.nave.vidas = 0;
    revisar(logica.verificarCondiciones() == Evento::FIN_PARTIDA, "sin vidas se pierde");

    // 19. Sin aliens se gana el nivel
    limpiar(e);
    revisar(logica.verificarCondiciones() == Evento::NIVEL_COMPLETADO, "sin aliens se gana el nivel");

    // 20. El puntaje maximo se actualiza
    limpiar(e);
    e.datos.puntaje = 500;
    e.datos.puntajeMax = 200;
    logica.verificarCondiciones();
    revisar(e.datos.puntajeMax == 500, "el puntaje maximo se actualiza");

    // ---------- Resultado ----------
    cout << endl;
    cout << "Pasaron: " << pasaron << "   Fallaron: " << fallaron << endl;
    return 0;
}