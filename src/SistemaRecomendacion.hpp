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
    // constructor: árboles B+ de orden fijo
    SistemaRecomendacion();

    // cargar CSV (idUsuario,idCancion,valoracion)
    void cargarDatosCSV(const string& ruta);

    // registrar o actualizar valoración
    void agregarValoracion(int idUsuario, int idCancion, float valoracion);

    // mostrar árboles por consola
    void mostrarCanciones();
    void mostrarUsuarios();

    // primeros n usuarios que votaron una canción
    vector<int> getFirstVoters(int idCancion, int n) const;
    void verPrimerosVotantes(int idCancion, int n) const;

    // top k canciones por promedio
    vector<int> getTopSongs(int k = 10) const;

    // k usuarios más parecidos (distancia Manhattan)
    vector<int> getSimilarUsers(int idUsuario, int k = 5) const;

    // recomendar k canciones basadas en vecinos
    vector<int> recommendSongs(int idUsuario, int k = 10) const;

    // clustering simple en k grupos
    vector<vector<int>> clusterUsuarios(int k) const;

    // listar valoraciones de una canción o de un usuario
    vector<pair<int,float>> buscarValoracionesPorCancion(int idCancion) const;
    vector<pair<int,float>> buscarValoracionesPorUsuario(int idUsuario) const;

private:
    // orden del B+ Tree
    static constexpr int ORDEN = 64;
    BPlusTree<int, CancionPtr, ORDEN> arbolCanciones;
    BPlusTree<int, UsuarioPtr, ORDEN>  arbolUsuarios;
    unordered_map<int, CancionPtr> hashCanciones;
    unordered_map<int, UsuarioPtr>  hashUsuarios;
    vector<pair<int,float>> top10;

    // actualizar top10 tras nueva valoración
    void actualizarTop(const CancionPtr& cancion);
    // distancia Manhattan entre dos mapas de valoraciones
    float distanciaManhattan(const unordered_map<int,float>& a,
                             const unordered_map<int,float>& b) const;
};
