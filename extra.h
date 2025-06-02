#ifndef EXTRA_H
#define EXTRA_H

#include "enrutador.h"
#include <map>
#include <string>

using namespace std;

map<char,Enrutador*> cargarRed(string file);
void tablaDeEnrutamiento(map<char,Enrutador*>& red);
vector<Enrutador*> reconstruirRuta(Enrutador* origen, Enrutador* destino, map<Enrutador*, Enrutador*>& predecesor);
map<Enrutador*, Enrutador*> dijkstra(Enrutador* fuente);

#endif // EXTRA_H
