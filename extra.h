#ifndef EXTRA_H
#define EXTRA_H

#include "enrutador.h"
#include <map>
#include <string>

using namespace std;

map<char,Enrutador*> cargarRed(string file);
void dijkstra(Enrutador* fuente);

#endif // EXTRA_H
