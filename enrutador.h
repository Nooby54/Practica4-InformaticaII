#ifndef ENRUTADOR_H
#define ENRUTADOR_H

#include <vector>
#include <utility>
using namespace std;

class Enrutador{
public:
    // Atributos
    Enrutador(int id);
    int idEnrut;
    vector<pair<Enrutador*, int>> vecinos;
    int distancia; //Distancia de este enrutador a la fuente
    bool visitado;

    // Metodos
    void nuevoVecino(Enrutador* vecino, int costo);
    void reinicio();
};

#endif // ENRUTADOR_H

