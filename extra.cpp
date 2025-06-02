#include "extra.h"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <map>
#include <queue>

using namespace std;

map<char, Enrutador *> cargarRed(string file)
{
    map<char, Enrutador *> red;
    ifstream archivo(file);
    string linea;
    while (getline(archivo, linea))
    {
        char nombreOrigen = linea[0];
        string vecinosStr = linea.substr(2);

        if (red.find(nombreOrigen) == red.end())
        {
            red[nombreOrigen] = new Enrutador(nombreOrigen);
        }

        Enrutador *origen = red[nombreOrigen];

        stringstream ss(vecinosStr);
        string parVecino;

        while (getline(ss, parVecino, ','))
        {
            if (parVecino.empty())
                continue;

            size_t guion = parVecino.find('-');
            if (guion == string::npos || guion == 0)
                continue;

            char nombreVecino = parVecino[0];
            int costo = stoi(parVecino.substr(guion + 1));

            if (red.find(nombreVecino) == red.end())
            {
                red[nombreVecino] = new Enrutador(nombreVecino);
            }

            Enrutador *vecino = red[nombreVecino];

            bool yaConectados = false;
            for (const auto &par : origen->vecinos)
            {
                if (par.first == vecino)
                {
                    yaConectados = true;
                    break;
                }
            }

            if (!yaConectados)
            {
                origen->nuevoVecino(vecino, costo);
            }
        }
    }

    archivo.close();
    tablaDeEnrutamiento(red);
    return red;
}

void tablaDeEnrutamiento(map<char, Enrutador *> &red)
{
    for (auto iter = red.begin(); iter != red.end(); iter++)
    {
        Enrutador *actual = iter->second;

        // Reiniciar distancias y visitados
        for (auto &nodo : red)
        {
            nodo.second->reinicio();
        }

        map<Enrutador *, Enrutador *> predecesor = dijkstra(actual);
        actual->tabla.clear();

        for (auto d = red.begin(); d != red.end(); d++)
        {
            vector<Enrutador *> ruta = reconstruirRuta(actual, d->second, predecesor);
            actual->tabla[d->second] = make_pair(d->second->distancia, ruta);
        }
    }
}

vector<Enrutador *> reconstruirRuta(Enrutador *origen, Enrutador *destino, map<Enrutador *, Enrutador *> &predecesor)
{
    vector<Enrutador *> ruta;
    Enrutador *actual = destino;

    if (predecesor.find(destino) == predecesor.end() && origen != destino)
    {
        return ruta; // ruta vacía si no hay camino
    }

    while (actual != origen)
    {
        ruta.push_back(actual);
        actual = predecesor[actual];
    }

    ruta.push_back(origen);
    reverse(ruta.begin(), ruta.end());
    return ruta;
}

map<Enrutador *, Enrutador *> dijkstra(Enrutador *fuente)
{
    map<Enrutador *, Enrutador *> predecesor;

    fuente->distancia = 0;

    priority_queue<pair<int, Enrutador *>> colaEnrut;
    colaEnrut.push({0, fuente});

    while (!colaEnrut.empty())
    {
        Enrutador *actual = colaEnrut.top().second;
        colaEnrut.pop();

        if (actual->visitado)
            continue;
        actual->visitado = true;

        for (auto &veci : actual->vecinos)
        {
            Enrutador *sigEnrutador = veci.first;
            int costo = veci.second;
            int nuevaDist = actual->distancia + costo;

            if (nuevaDist < sigEnrutador->distancia)
            {
                sigEnrutador->distancia = nuevaDist;
                predecesor[sigEnrutador] = actual;
                colaEnrut.push({-nuevaDist, sigEnrutador});
            }
        }
    }

    return predecesor;
}

void imprimirEnrutadores(map<char, Enrutador *> &red)
{
    for (const auto &par : red)
    {
        Enrutador *enrutador = par.second;
        char nombre = static_cast<char>(enrutador->idEnrut);

        cout << "Enrutador " << nombre << endl;
    }
}
void eliminarEnrutador(map<char, Enrutador *> &red)
{
    imprimirEnrutadores(red);
    bool encontrado = false;
    do
    {
        string enrutador;
        cout << "Ingrese el enrutador que desea eliminar: ";
        cin >> enrutador;
        cout << endl;
        char cEnrutador = enrutador[0];
        auto it = red.find(cEnrutador);

        if (it != red.end())
        {
            red.erase(cEnrutador);
            encontrado = true;
            tablaDeEnrutamiento(red);
        }

        else
            cout << "El enrutador no existe" << endl;
    } while (!encontrado);
}

void agregarEnrutador(map<char, Enrutador *> &red)
{
    bool agregado = false;
    do
    {
        string enrutadorStr;
        cout << "Ingrese el enrutador que desea agregar: ";
        cin >> enrutadorStr;
        cout << endl;

        char id = enrutadorStr[0];
        if (red.find(id) != red.end())
        {
            cout << "El enrutador ya existe.\n";
            continue;
        }

        Enrutador *nuevo = new Enrutador(id);
        red[id] = nuevo;

        // Conexiones con otros enrutadores existentes
        if (red.size() > 1)
        {
            cout << "Ahora, ingrese las conexiones desde el enrutador " << id << " a otros existentes.\n";
            for (const auto &par : red)
            {
                Enrutador *otro = par.second;
                if (otro->idEnrut == id)
                    continue; // no conectarse consigo mismo

                cout << "Desea conectar " << id << " con " << char(otro->idEnrut) << "? (s/n): ";
                char respuesta;
                cin >> respuesta;
                if (respuesta == 's')
                {
                    string costoStr;
                    int costo;
                    do
                    {
                        cout << "Ingrese el costo del enlace entre " << id << " y " << char(otro->idEnrut) << ": ";
                        cin >> costoStr;
                        try
                        {
                            costo = stoi(costoStr);
                            if (costo <= 0)
                                throw invalid_argument("costo inválido");
                            break;
                        }
                        catch (...)
                        {
                            cout << "Valor invalido. Intente de nuevo.\n";
                        }
                    } while (true);

                    nuevo->nuevoVecino(otro, costo);
                    otro->nuevoVecino(nuevo, costo);
                }
            }
        }

        tablaDeEnrutamiento(red);
        cout << "Enrutador agregado exitosamente.\n";
        agregado = true;

    } while (!agregado);
}

void gestionarEnlace(map<char, Enrutador *> &red)
{
    imprimirEnrutadores(red);

    string origenStr, destinoStr;
    cout << "Ingrese el enrutador origen: ";
    cin >> origenStr;
    cout << "Ingrese el enrutador destino: ";
    cin >> destinoStr;

    char cOrigen = origenStr[0];
    char cDestino = destinoStr[0];

    if (red.find(cOrigen) == red.end() || red.find(cDestino) == red.end())
    {
        cout << "Uno o ambos enrutadores no existen.\n";
        return;
    }

    Enrutador *origen = red[cOrigen];
    Enrutador *destino = red[cDestino];

    // Verificar si ya existe el enlace
    bool existe = false;
    for (auto &par : origen->vecinos)
    {
        if (par.first == destino)
        {
            existe = true;
            break;
        }
    }

    cout << "\nActualmente, el enlace " << cOrigen << " - " << cDestino
         << (existe ? " EXISTE.\n" : " NO EXISTE.\n");

    cout << "Que desea hacer?\n1. Agregar enlace\n2. Eliminar enlace\n3. Modificar costo\nOpcion: ";
    string opcion;
    cin >> opcion;

    if (opcion == "1")
    {
        if (existe)
        {
            cout << "Ya existe un enlace entre esos enrutadores.\n";
            return;
        }

        int costo;
        string costoStr;
        bool valido = false;
        do
        {
            try
            {
                cout << "Ingrese el costo del nuevo enlace: ";
                cin >> costoStr;
                costo = stoi(costoStr);
                if (costo >= 0 && costo <= INT_MAX)
                {
                    valido = true;
                }
                else
                {
                    cout << "El costo debe estar entre 0 y " << INT_MAX << ".\n";
                }
            }
            catch (...)
            {
                cout << "Ingrese un numero valido.\n";
            }
        } while (!valido);

        origen->nuevoVecino(destino, costo);
        cout << "Enlace agregado exitosamente.\n";
    }

    else if (opcion == "2")
    {
        if (!existe)
        {
            cout << "No hay enlace entre esos enrutadores.\n";
            return;
        }

        auto &vecinosO = origen->vecinos;
        vecinosO.erase(remove_if(vecinosO.begin(), vecinosO.end(),
                                 [destino](const pair<Enrutador *, int> &par)
                                 {
                                     return par.first == destino;
                                 }),
                       vecinosO.end());

        auto &vecinosD = destino->vecinos;
        vecinosD.erase(remove_if(vecinosD.begin(), vecinosD.end(),
                                 [origen](const pair<Enrutador *, int> &par)
                                 {
                                     return par.first == origen;
                                 }),
                       vecinosD.end());

        cout << "Enlace eliminado correctamente.\n";
    }

    else if (opcion == "3")
    {
        if (!existe)
        {
            cout << "No hay enlace para modificar. Use la opción 1 para crearlo.\n";
            return;
        }

        int costo;
        string costoStr;
        bool valido = false;
        do
        {
            try
            {
                cout << "Ingrese el nuevo costo del enlace: ";
                cin >> costoStr;
                costo = stoi(costoStr);
                if (costo >= 0 && costo <= INT_MAX)
                {
                    valido = true;
                }
                else
                {
                    cout << "El costo debe estar entre 0 y " << INT_MAX << ".\n";
                }
            }
            catch (...)
            {
                cout << "Ingrese un numero valido.\n";
            }
        } while (!valido);

        for (auto &par : origen->vecinos)
        {
            if (par.first == destino)
            {
                par.second = costo;
                break;
            }
        }

        for (auto &par : destino->vecinos)
        {
            if (par.first == origen)
            {
                par.second = costo;
                break;
            }
        }

        cout << "Costo actualizado correctamente.\n";
    }

    else
    {
        cout << "Opcion no valida.\n";
    }

    tablaDeEnrutamiento(red);
}

void modificarRed(map<char, Enrutador *> &red)
{
    string modo;
    cout << "--------------------------" << endl;
    cout << "1. Para agregar enrutador\n2. Eliminar enrutador\n3. Gestionar enlaces\n0. Para dejar de modificar la red\nIngrese una opcion: ";
    cin >> modo;
    do
    {
        if (modo == "1")
        {
            agregarEnrutador(red);
        }
        else if (modo == "2")
        {
            eliminarEnrutador(red);
        }
        else if (modo == "3")
        {
            gestionarEnlace(red);
        }
        else if (modo == "0")
        {
            break;
        }
        else
        {
            cout << "Modo incorrecto" << endl;
        }
        cout << "--------------------------" << endl;
        cout << "1. Para agregar enrutador\n2. Eliminar enrutador\n3. Gestionar enlaces\n0. Para dejar de modificar la red\nIngrese una opcion: ";
        cin >> modo;
    } while (modo != "0");
}
