#ifndef SISTEMA_RECOMENDACION_H
#define SISTEMA_RECOMENDACION_H

#include <string>
#include <vector>
#include <memory>
#include <utility>
#include "BPlusTree.h"
#include "Persona.h"
#include "Cancion.h"
#include "Valoracion.h"

using namespace std;

class SistemaRecomendacion {
public:
    // Constructor con orden configurable para los B+Trees
    explicit SistemaRecomendacion(int orden = 4);

    // Carga datos desde CSV de valoraciones (ruta de archivo)
    void cargarDatosCSV(const string& ruta);

    // Muestra en consola el contenido de personas, canciones y valoraciones
    void mostrarContenido() const;

    // Registro de una nueva valoración manual
    void agregarValoracion(int idUsuario, int idCancion,
                           double puntuacion, const string& momento);

    // Obtiene los primeros N usuarios que valoraron una canción
    vector<int> getFirstVoters(int idCancion, int limite = 5) const;

    // Obtiene el top K de canciones más valoradas globalmente
    vector<int> getTopSongs(int k = 10) const;

    // Encuentra K usuarios similares a un usuario dado
    vector<int> getSimilarUsers(int idUsuario, int k = 5) const;

    // Recomienda K canciones a un usuario basadas en vecinos
    vector<int> recommendSongs(int idUsuario, int k = 10) const;

    // Agrupa usuarios en kClusters según afinidad musical
    vector<vector<int>> clusterUsuarios(int kClusters) const;

    // Búsqueda de valoraciones por canción o por usuario
    vector<shared_ptr<Valoracion>> buscarValoracionesPorCancion(int idCancion) const;
    vector<shared_ptr<Valoracion>> buscarValoracionesPorUsuario(int idUsuario) const;

private:
    // Árboles B+ para almacenamiento y consulta eficiente
    BPlusTree<int, shared_ptr<Persona>>                       arbolPersonas;
    BPlusTree<int, shared_ptr<Cancion>>                       arbolCanciones;

    // vectores solo para almacenar las listas :y
};

#endif 
