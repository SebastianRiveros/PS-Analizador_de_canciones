// src/SistemaRecomendacion.cpp
#include "SistemaRecomendacion.hpp"
#include <fstream>
#include <sstream>

// constructor: inicializa árboles b+ con orden fijo del template
SistemaRecomendacion::SistemaRecomendacion()
    : arbolCanciones(), arbolUsuarios()
{}

// carga datos desde csv (idUsuario,idCancion,valoracion,...)
void SistemaRecomendacion::cargarDatosCSV(const string& ruta) {
    ifstream archivo(ruta);
    if (!archivo.is_open()) return;  // no se pudo abrir

    string linea;
    while (getline(archivo, linea)) {
        if (linea.empty()) continue;

        stringstream ss(linea);
        string sU, sC, sV, sIgn;
        getline(ss, sU, ',');
        getline(ss, sC, ',');
        getline(ss, sV, ',');
        getline(ss, sIgn, ',');

        int idU = stoi(sU);
        int idC = stoi(sC);
        float val = stof(sV);

        agregarValoracion(idU, idC, val);
    }

    archivo.close();
}

// registra o actualiza una valoración
void SistemaRecomendacion::agregarValoracion(int idUsuario,
                                             int idCancion,
                                             float valoracion) {
    // obtener o crear usuario
    UsuarioPtr usu;
    auto itU = hashUsuarios.find(idUsuario);
    if (itU == hashUsuarios.end()) {
        usu = make_shared<Usuario>(idUsuario);
        hashUsuarios[idUsuario] = usu;
        arbolUsuarios.insert(idUsuario, usu);
    } else {
        usu = itU->second;
    }

    // obtener o crear canción
    CancionPtr can;
    auto itC = hashCanciones.find(idCancion);
    if (itC == hashCanciones.end()) {
        can = make_shared<Cancion>(idCancion);
        hashCanciones[idCancion] = can;
        arbolCanciones.insert(idCancion, can);
    } else {
        can = itC->second;
    }

    // añadir valoración a ambos objetos
    usu->agregarValoracion(idCancion, valoracion);
    can->agregarValoracion(idUsuario, valoracion);

    // actualizar top 10 global
    // actualizarTop(can);
}

// mostrar los datos
void SistemaRecomendacion::mostrarCanciones(){
    arbolCanciones.display();
}

void SistemaRecomendacion::mostrarUsuarios(){
    arbolUsuarios.display();
}

// devuelve primeros n usuarios que valoraron una canción
vector<int> SistemaRecomendacion::getFirstVoters(int idCancion,
                                                 int n) const {
    // TODO: 
    // 1) buscar CancionPtr con arbolCanciones.search(idCancion)
    // 2) iterar sobre cancion->valoraciones y tomar hasta n ids
    return {};
}

// devuelve top k canciones por promedio
vector<int> SistemaRecomendacion::getTopSongs(int k) const {
    // TODO: leer los primeros k elementos de 'top10' y devolver sus ids
    return {};
}

// calcula distancia manhattan entre dos mapas de valoraciones
float SistemaRecomendacion::distanciaManhattan(
        const unordered_map<int,float>& a,
        const unordered_map<int,float>& b) const {
    // TODO: recorrer claves comunes y acumular |a[key] - b[key]|
    return 0.0f;
}

// devuelve k usuarios más similares a idUsuario
vector<int> SistemaRecomendacion::getSimilarUsers(int idUsuario,
                                                  int k) const {
    // TODO:
    // 1) recuperar mapa base de hashUsuarios[idUsuario]
    // 2) para cada otro usuario calcular distanciaManhattan
    // 3) ordenar por distancia y devolver primeros k ids
    return {};
}

// recomienda k canciones basadas en vecinos de idUsuario
vector<int> SistemaRecomendacion::recommendSongs(int idUsuario,
                                                 int k) const {
    // TODO:
    // 1) obtener vecinos con getSimilarUsers(idUsuario, k)
    // 2) recolectar canciones valoradas por vecinos y no por usuario
    // 3) calcular promedio por canción, ordenar desc y devolver k ids
    return {};
}

// agrupa usuarios en k clusters (implementación opcional)
vector<vector<int>> SistemaRecomendacion::clusterUsuarios(int k) const {
    // TODO: implementar clustering (p.ej. k-means ligero) o dejar vacío
    return {};
}

// lista pares (idUsuario, valoracion) de una canción
vector<pair<int,float>> SistemaRecomendacion::buscarValoracionesPorCancion(
        int idCancion) const {
    // TODO:
    // 1) buscar CancionPtr con arbolCanciones.search(idCancion)
    // 2) copiar todos los pares (idUsuario, valoracion) en un vector
    return {};
}

// lista pares (idCancion, valoracion) de un usuario
vector<pair<int,float>> SistemaRecomendacion::buscarValoracionesPorUsuario(
        int idUsuario) const {
    // TODO:
    // 1) buscar UsuarioPtr con arbolUsuarios.search(idUsuario)
    // 2) copiar todos los pares (idCancion, valoracion) en un vector
    return {};
}

// mantiene actualizado el vector top10 con la canción dada
void SistemaRecomendacion::actualizarTop(const CancionPtr& cancion) {
    // TODO:
    // 1) obtener id y promedio de 'cancion'
    // 2) insertar o actualizar par (id, promedio) en 'top10'
    // 3) ordenar descendente y recortar tamaño a 10
}
