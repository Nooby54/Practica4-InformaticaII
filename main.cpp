#include <iostream>
#include "enrutador.h"
#include "extra.h"

using namespace std;

int main()
{
    map<char, Enrutador *> red;

    string modo;
    do
    {
        cout << "--------------------------" << endl;
        cout << "1. Para crear la red\n2. Para cargar del archivo\n3. Para conocer el costo y el camino entre dos nodos\n4. Para modificar la red\n0. Para salir\nIngrese una opcion: ";
        cin >> modo;

        if (modo == "1")
        {
            cout << "A continuacion debera crear un enrutador" << endl;
            agregarEnrutador(red);
            modificarRed(red);
        }
        else if (modo == "2")
        {
            red = (cargarRed("../../data/archivo.txt"));
            cout << "Red cargada correctamente" << endl;
        }
        else if (modo == "3")
        {
            if (red.size() > 1)
            {
                conocerCosto(red);
            }
            else
            {
                cout << "No hay una red creada o cargada aun" << endl;
            }
        }
        else if(modo == "4"){
            modificarRed(red);
        }
        else if (modo == "0")
        {
            cout << "Saliendo..." << endl;
            break;
        }
        else
        {
            cout << "--------------------------" << endl;
            cout << "Modo incorrecto" << endl;
        }
    } while (modo != "0");

    for (auto &par : red)
    {
        delete par.second;
    }
    red.clear();

    return 0;
}
