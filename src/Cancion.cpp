#include "Cancion.hpp"

Cancion::Cancion(int id)
    : idCancion(id)
{}

void Cancion::agregarValoracion(int idUsuario, float valoracion) {
    auto it = valoraciones.find(idUsuario);
    int puntosEscalados = int(valoracion * 100);
    if (it == valoraciones.end()) {
        // nueva valoración
        valoraciones[idUsuario] = valoracion;
        sumaPuntos += puntosEscalados;
        ++cuenta;
    } else {
        // actualizar existente
        sumaPuntos -= int(it->second * 100);
        it->second = valoracion;
        sumaPuntos += puntosEscalados;
    }
}

float Cancion::obtenerPromedio() const {
    return cuenta
        ? (sumaPuntos / 100.0f) / cuenta
        : 0.0f;
}
