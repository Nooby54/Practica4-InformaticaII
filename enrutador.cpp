#include "enrutador.h"
#include <climits>

Enrutador::Enrutador(int id) : idEnrut(id), distancia(INT_MAX), visitado(false) {}

void Enrutador::nuevoVecino(Enrutador *vecino, int costo)
{
    vecinos.emplace_back(vecino, costo);
    vecino->vecinos.emplace_back(this, costo);
}

void Enrutador::reinicio()
{
    distancia = INT_MAX;
    visitado = false;
}
