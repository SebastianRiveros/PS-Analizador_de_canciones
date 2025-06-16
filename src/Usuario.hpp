// src/Usuario.hpp
#pragma once
#include <unordered_map>
#include <memory>
using namespace std;

class Usuario {
public:
    int idUsuario;
    unordered_map<int,float> valoraciones; // idCancion → puntuación

    explicit Usuario(int id);

    // Añade o actualiza la valoración de una canción
    void agregarValoracion(int idCancion, float valoracion);
};

using UsuarioPtr = shared_ptr<Usuario>;
