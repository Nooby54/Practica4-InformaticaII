#include "extra.h"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <map>
#include <queue>

using namespace std;

map<char,Enrutador*> cargarRed(string file){
    map<char, Enrutador*> red;
    ifstream archivo(file);
    string linea;
    while (getline(archivo, linea)) {
        char nombreOrigen = linea[0];
        string vecinosStr = linea.substr(2);

        if (red.find(nombreOrigen) == red.end()) {
            red[nombreOrigen] = new Enrutador(nombreOrigen);
        }

        Enrutador* origen = red[nombreOrigen];

        stringstream ss(vecinosStr);
        string parVecino;

        while (getline(ss, parVecino, ',')) {
            if (parVecino.empty()) continue;

            size_t guion = parVecino.find('-');
            if (guion == string::npos || guion == 0) continue;

            char nombreVecino = parVecino[0];
            int costo = stoi(parVecino.substr(guion + 1));

            if (red.find(nombreVecino) == red.end()) {
                red[nombreVecino] = new Enrutador(nombreVecino);
            }

            Enrutador* vecino = red[nombreVecino];

            bool yaConectados = false;
            for (const auto& par : origen->vecinos) {
                if (par.first == vecino) {
                    yaConectados = true;
                    break;
                }
            }

            if (!yaConectados) {
                origen->nuevoVecino(vecino, costo);
            }
        }
    }

    archivo.close();
    return red;
}

void tablaDeEnrutamiento(map<char, Enrutador*>& red) {
    for (auto iter = red.begin(); iter != red.end(); iter++) {
        Enrutador* actual = iter->second;

        // Reiniciar distancias y visitados
        for (auto& nodo : red) {
            nodo.second->distancia = INT_MAX;
            nodo.second->visitado = false;
        }

        map<Enrutador*, Enrutador*> predecesor = dijkstra(actual);
        actual->tabla.clear();

        for (auto d = red.begin(); d != red.end(); d++) {
            vector<Enrutador*> ruta = reconstruirRuta(actual, d->second, predecesor);
            actual->tabla[d->second] = make_pair(d->second->distancia, ruta);
        }
    }
}

vector<Enrutador*> reconstruirRuta(Enrutador* origen, Enrutador* destino, map<Enrutador*, Enrutador*>& predecesor) {
    vector<Enrutador*> ruta;
    Enrutador* actual = destino;

    if (predecesor.find(destino) == predecesor.end() && origen != destino) {
        return ruta; // ruta vacía si no hay camino
    }

    while (actual != origen) {
        ruta.push_back(actual);
        actual = predecesor[actual];
    }

    ruta.push_back(origen);
    reverse(ruta.begin(), ruta.end());
    return ruta;
}

map<Enrutador*, Enrutador*> dijkstra(Enrutador* fuente) {
    map<Enrutador*, Enrutador*> predecesor;

    fuente->distancia = 0;

    priority_queue<pair<int, Enrutador*>> colaEnrut;
    colaEnrut.push({0, fuente});

    while (!colaEnrut.empty()) {
        Enrutador* actual = colaEnrut.top().second;
        colaEnrut.pop();

        if (actual->visitado) continue;
        actual->visitado = true;

        for (auto& veci : actual->vecinos) {
            Enrutador* sigEnrutador = veci.first;
            int costo = veci.second;
            int nuevaDist = actual->distancia + costo;

            if (nuevaDist < sigEnrutador->distancia) {
                sigEnrutador->distancia = nuevaDist;
                predecesor[sigEnrutador] = actual;
                colaEnrut.push({-nuevaDist, sigEnrutador});
            }
        }
    }

    return predecesor;
}

