// src/SistemaRecomendacion.hpp
#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include "BPlusTree.hpp"
#include "Cancion.hpp"
#include "Usuario.hpp"
using namespace std;

class SistemaRecomendacion {
public:
    // constructor vacío: use orden fijo M=64 del BPlusTree
    SistemaRecomendacion();

    // carga datos desde csv (idUsuario,idCancion,valoracion)
    void cargarDatosCSV(const string& ruta);

    // registra o actualiza una valoración
    void agregarValoracion(int idUsuario, int idCancion, float valoracion);

    //mostrar datos 
    void mostrarCanciones();
    void mostrarUsuarios();

    // primeros n usuarios que votaron una canción
    void verPrimerosVotantes(int idCancion, int n) const;

    // top k canciones por promedio BENJA
    vector<int> getTopSongs(int k = 10) const;

    // k usuarios más parecidos (Manhattan) BENJA
    vector<int> getSimilarUsers(int idUsuario, int k = 5) const;

    // recomienda k canciones basadas en vecinos BENJA
    vector<int> recommendSongs(int idUsuario, int k = 10) const;

    // clustering simple en k grupos
    vector<vector<int>> clusterUsuarios(int k) const;

    // todas las valoraciones de una canción
    vector<pair<int,float>> buscarValoracionesPorCancion(int idCancion) const;

    // todas las valoraciones de un usuario
    vector<pair<int,float>> buscarValoracionesPorUsuario(int idUsuario) const;

private:
    BPlusTree<int, CancionPtr> arbolCanciones;    // M = 64
    BPlusTree<int, UsuarioPtr>  arbolUsuarios;    // M = 64
    unordered_map<int, CancionPtr> hashCanciones;
    unordered_map<int, UsuarioPtr>  hashUsuarios;
    vector<pair<int,float>> top10;

    // helpers
    void actualizarTop(const CancionPtr& cancion);
    float distanciaManhattan(const unordered_map<int,float>& a,
                             const unordered_map<int,float>& b) const;
};
