#pragma once

#include <string>
#include <vector>
#include <memory>
#include <unordered_map>
#include "BPlusTree.hpp"
#include "Usuario.cpp"
#include "Cancion.cpp"

using namespace std;

class SistemaRecomendacion {
public:
    // Constructor: orden de los B+Trees
    explicit SistemaRecomendacion(int orden = 4);

    // Carga CSV (idUsuario,idCancion,valoracion)
    void cargarDatosCSV(const string& ruta);

    // Primeros N usuarios que votaron una canción
    vector<int> getFirstVoters(int idCancion, int n = 5) const;

    // Top K canciones por promedio
    vector<int> getTopSongs(int k = 10) const;

    // Usuarios más similares (Manhattan)
    vector<int> getSimilarUsers(int idUsuario, int k = 5) const;

    // Recomendaciones colaborativas
    vector<int> recommendSongs(int idUsuario, int k = 10) const;

    // Clustering simple de usuarios (k grupos)
    vector<vector<int>> clusterUsuarios(int k) const;

    // Todas las valoraciones de una canción/usuario
    vector<pair<int,float>> buscarValoracionesPorCancion(int idCancion) const;
    vector<pair<int,float>> buscarValoracionesPorUsuario(int idUsuario) const;

private:
    BPlusTree<int, CancionPtr> arbolCanciones;      // Índice ordenado de canciones
    BPlusTree<int, UsuarioPtr> arbolUsuarios;       // Índice ordenado de usuarios
    unordered_map<int, CancionPtr> hashCanciones;   // Acceso O(1) a canciones
    unordered_map<int, UsuarioPtr> hashUsuarios;    // Acceso O(1) a usuarios
    vector<pair<int,float>> top10;                  // Top 10 dinámico

    // Helpers:
    void actualizarTop(const CancionPtr& c);  // Mantiene top10
    float distanciaManhattan(const unordered_map<int,float>& a,
                             const unordered_map<int,float>& b) const;  // Similitud
};
