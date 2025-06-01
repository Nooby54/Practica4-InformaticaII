#include "extra.h"

#include <fstream>
#include <sstream>
#include <map>
#include <queue>

using namespace std;

map<char,Enrutador*> cargarRed(string file){
    map<char, Enrutador*> red;
    ifstream archivo(file);
    string linea;
    while (getline(archivo, linea)) {
        if (linea.empty()) continue;

        size_t separador = linea.find('|');
        if (separador == string::npos) continue;

        char nombreOrigen = linea[0];
        string vecinosStr = linea.substr(separador + 1);

        if (red.find(nombreOrigen) == red.end()) {
            red[nombreOrigen] = new Enrutador(nombreOrigen);
        }

        Enrutador* origen = red[nombreOrigen];

        stringstream ss(vecinosStr);
        string parVecino;

        while (getline(ss, parVecino, ',')) {
            if (parVecino.empty()) continue;

            size_t guion = parVecino.find('-');
            if (guion == string::npos) continue;

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

void dijkstra(Enrutador* fuente){

    fuente->distancia = 0;

    priority_queue<pair<int, Enrutador*>> colaEnrut; //El elemento con mayor prior (int) se pone al principio

    colaEnrut.push({0,fuente});

    while(!colaEnrut.empty()){
        Enrutador* actual = colaEnrut.top().second;
        colaEnrut.pop();

        if(actual->visitado) continue;
        actual->visitado = true;
        for(auto& veci : actual->vecinos){ //Leo vecinos del router actual
            Enrutador* sigEnrutador = veci.first;
            int costoEnlace = veci.second;
            int nuevaDist = actual->distancia + costoEnlace;
            if (nuevaDist< sigEnrutador->distancia){
                sigEnrutador->distancia = nuevaDist;
                colaEnrut.push({-nuevaDist,sigEnrutador});
            }
        }
    }
}
