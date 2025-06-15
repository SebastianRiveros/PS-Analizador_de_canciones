// src/Usuario.cpp
#include "Usuario.hpp"

Usuario::Usuario(int id)
    : idUsuario(id)
{}

void Usuario::agregarValoracion(int idCancion, float valoracion) {
    valoraciones[idCancion] = valoracion;
}
