#pragma once

// Guarda un choque que paso en el cuadro.
// Lo crea detectarColisiones() y lo usa resolverImpactos().
// Si un campo no se usa, vale -1.
//
//   BALA_ALIEN    -> indiceBala = bala, indiceObjetivo = alien
//   BALA_NAVE     -> indiceBala = bala
//   BALA_BARRERA  -> indiceBala = bala, indiceObjetivo = barrera, celdaFila y celdaColumna = celda
//   ALIEN_BARRERA -> indiceObjetivo = barrera, celdaFila y celdaColumna = celda
//   INVASION      -> todo en -1

#include "Util.h"

struct Impacto {
    TipoImpacto tipo;
    int indiceBala;
    int indiceObjetivo;
    int celdaFila;
    int celdaColumna;
};