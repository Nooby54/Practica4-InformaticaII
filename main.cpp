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
        cout << "1. Para crear la red\n2. Para cargar del archivo\n0. Para salir\nIngrese una opcion: ";
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
            modificarRed(red);
        }
        else if (modo == "0")
        {
            break;
        }
        else
        {
            cout << "--------------------------" << endl;
            cout << "Modo incorrecto" << endl;
        }
        cout << "1. Para crear la red\n2. Para cargar del archivo\n0. Para salir\nIngrese una opcion: ";
        cin >> modo;
    } while (modo != "0");

    cout << "Ya no podra modificar mas la red, ahora solo puede consultar los costos y las rutas" << endl;
    do
    {

    } while (true);
    //imprimirRed(red);
    return 0;
}
