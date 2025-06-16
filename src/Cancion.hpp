// src/Cancion.hpp
#pragma once
#include <unordered_map>
#include <memory>
using namespace std;

class Cancion {
public:
    int idCancion;
    unordered_map<int,float> valoraciones; // idUsuario → puntuación
    int sumaPuntos = 0;                    // total de puntos *100
    int cuenta = 0;                        // número de valoraciones

    explicit Cancion(int id);

    // Añade o actualiza la valoración de un usuario
    void agregarValoracion(int idUsuario, float valoracion);

    // Devuelve el promedio real de la canción
    float obtenerPromedio() const;
};

using CancionPtr = shared_ptr<Cancion>;
