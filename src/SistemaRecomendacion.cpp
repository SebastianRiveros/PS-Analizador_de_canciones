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
    // TODO: abrir archivo 'ruta', leer línea a línea, parsear campos
    //       y llamar a agregarValoracion(idUsuario, idCancion, valoracion)
}

// registra o actualiza una valoración
void SistemaRecomendacion::agregarValoracion(int idUsuario,
                                             int idCancion,
                                             float valoracion) {
    // TODO: 
    // 1) crear o recuperar UsuarioPtr en hashUsuarios y arbolUsuarios
    // 2) crear o recuperar CancionPtr en hashCanciones y arbolCanciones
    // 3) llamar a usuario->agregarValoracion(...) y cancion->agregarValoracion(...)
    // 4) llamar a actualizarTop(cancion)
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
