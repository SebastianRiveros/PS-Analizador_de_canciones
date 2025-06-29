#include "SistemaRecomendacion.hpp"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cmath>
#include <iostream>

using namespace std;

// constructor: inicializa árboles b+ con orden fijo del template
SistemaRecomendacion::SistemaRecomendacion()
    : arbolCanciones(), arbolUsuarios()
{}

// carga datos desde csv (idUsuario,idCancion,valoracion,...)
void SistemaRecomendacion::cargarDatosCSV(const string& ruta) {
    ifstream archivo(ruta);
    if (!archivo.is_open()) return;

    string linea;
    while (getline(archivo, linea)) {
        if (linea.empty()) continue;
        stringstream ss(linea);
        string sU, sC, sV, sIgn;
        getline(ss, sU, ',');
        getline(ss, sC, ',');
        getline(ss, sV, ',');
        // ignoramos campo extra
        //getline(ss, sIgn, ',');
        int idU = stoi(sU), idC = stoi(sC);
        float val = stof(sV);
        agregarValoracion(idU, idC, val);
    }
}

// registra o actualiza una valoración
void SistemaRecomendacion::agregarValoracion(int idUsuario,
                                             int idCancion,
                                             float valoracion) {
    // usuario
    UsuarioPtr usu;
    auto itU = hashUsuarios.find(idUsuario);
    if (itU == hashUsuarios.end()) {
        usu = make_shared<Usuario>(idUsuario);
        hashUsuarios[idUsuario] = usu;
        arbolUsuarios.insert(idUsuario, usu);
    } else {
        usu = itU->second;
    }

    // canción
    CancionPtr can;
    auto itC = hashCanciones.find(idCancion);
    if (itC == hashCanciones.end()) {
        can = make_shared<Cancion>(idCancion);
        hashCanciones[idCancion] = can;
        arbolCanciones.insert(idCancion, can);
    } else {
        can = itC->second;
    }

    // añadir valoración
    usu->agregarValoracion(idCancion, valoracion);
    can->agregarValoracion(idUsuario, valoracion);

    // actualizar top10
    actualizarTop(can);
}

// muestra árboles
void SistemaRecomendacion::mostrarCanciones() {
    arbolCanciones.display();
}
void SistemaRecomendacion::mostrarUsuarios() {
    arbolUsuarios.display();
}

// devuelve primeros n ids de usuarios que votaron una canción
vector<int> SistemaRecomendacion::getFirstVoters(int idCancion,
                                                 int n) const {
    vector<int> res;
    auto it = hashCanciones.find(idCancion);
    if (it == hashCanciones.end()) return res;
    for (auto& p : it->second->valoraciones) {
        res.push_back(p.first);
        if ((int)res.size() >= n) break;
    }
    return res;
}

// imprime los primeros n usuarios que votaron la canción
void SistemaRecomendacion::verPrimerosVotantes(int idCancion,
                                               int n) const {
    auto v = getFirstVoters(idCancion, n);
    if (v.empty()) {
        cout << "No hay votantes para la canción " << idCancion << ".\n";
        return;
    }
    cout << "Primeros " << v.size()
         << " votantes de la canción " << idCancion << ":\n";
    for (int u : v) cout << "  - Usuario " << u << "\n";
}

// devuelve top k canciones por promedio
vector<int> SistemaRecomendacion::getTopSongs(int k) const {
    vector<pair<int,float>> tmp;
    for (auto& p : hashCanciones) {
        tmp.emplace_back(p.first, p.second->obtenerPromedio());
    }
    sort(tmp.begin(), tmp.end(),
         [](auto& a, auto& b){ return a.second > b.second; });
    vector<int> res;
    for (int i = 0; i < k && i < (int)tmp.size(); ++i)
        res.push_back(tmp[i].first);
    return res;
}

// calcula distancia manhattan entre dos valoraciones
float SistemaRecomendacion::distanciaManhattan(
        const unordered_map<int,float>& a,
        const unordered_map<int,float>& b) const {
    float s = 0.f;
    for (auto& p : a) {
        auto it = b.find(p.first);
        if (it != b.end())
            s += fabs(p.second - it->second);
    }
    return s;
}

// devuelve k usuarios más similares a idUsuario
vector<int> SistemaRecomendacion::getSimilarUsers(int idUsuario,
                                                  int k) const {
    vector<pair<float,int>> dist;
    auto it = hashUsuarios.find(idUsuario);
    if (it == hashUsuarios.end()) return {};
    auto& base = it->second->valoraciones;
    for (auto& p : hashUsuarios) {
        if (p.first == idUsuario) continue;
        float d = distanciaManhattan(base, p.second->valoraciones);
        dist.emplace_back(d, p.first);
    }
    sort(dist.begin(), dist.end());
    vector<int> res;
    for (int i = 0; i < k && i < (int)dist.size(); ++i)
        res.push_back(dist[i].second);
    return res;
}

// recomienda k canciones basadas en vecinos
vector<int> SistemaRecomendacion::recommendSongs(int idUsuario,
                                                 int k) const {
    auto vecinos = getSimilarUsers(idUsuario, k);
    auto it = hashUsuarios.find(idUsuario);
    if (it == hashUsuarios.end()) return {};
    auto& vistos = it->second->valoraciones;
    unordered_map<int,pair<float,int>> acc;
    for (int u : vecinos) {
        for (auto& p : hashUsuarios.at(u)->valoraciones) {
            if (!vistos.count(p.first)) {
                acc[p.first].first += p.second;
                acc[p.first].second++;
            }
        }
    }
    vector<pair<float,int>> tmp;
    for (auto& p : acc)
        tmp.emplace_back(p.second.first / p.second.second, p.first);
    sort(tmp.rbegin(), tmp.rend());
    vector<int> res;
    for (int i = 0; i < k && i < (int)tmp.size(); ++i)
        res.push_back(tmp[i].second);
    return res;
}

// agrupa usuarios en k clusters de forma sencilla
vector<vector<int>> SistemaRecomendacion::clusterUsuarios(int k) const {
    vector<vector<int>> out(k);
    int i = 0;
    for (auto& p : hashUsuarios) {
        out[i % k].push_back(p.first);
        ++i;
    }
    return out;
}

// retorna pares (idUsuario, valoracion) de una canción
vector<pair<int,float>> SistemaRecomendacion::buscarValoracionesPorCancion(
        int idCancion) const {
    vector<pair<int,float>> out;
    auto it = hashCanciones.find(idCancion);
    if (it == hashCanciones.end()) return out;
    for (auto& p : it->second->valoraciones)
        out.emplace_back(p.first, p.second);
    return out;
}

// retorna pares (idCancion, valoracion) de un usuario
vector<pair<int,float>> SistemaRecomendacion::buscarValoracionesPorUsuario(
        int idUsuario) const {
    vector<pair<int,float>> out;
    auto it = hashUsuarios.find(idUsuario);
    if (it == hashUsuarios.end()) return out;
    for (auto& p : it->second->valoraciones)
        out.emplace_back(p.first, p.second);
    return out;
}

// mantiene actualizado el vector top10 con la canción dada
void SistemaRecomendacion::actualizarTop(const CancionPtr& cancion) {
    int id = cancion->idCancion;
    float prom = cancion->obtenerPromedio();
    auto it = find_if(top10.begin(), top10.end(),
                      [&](auto& a){ return a.first == id; });
    if (it != top10.end()) it->second = prom;
    else top10.emplace_back(id, prom);
    sort(top10.begin(), top10.end(),
         [](auto& a, auto& b){ return a.second > b.second; });
    if (top10.size() > 10) top10.resize(10);
}
