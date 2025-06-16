// src/SistemaRecomendacion.cpp
#include "SistemaRecomendacion.hpp"
#include <fstream>
#include <sstream>
#include <algorithm> // Para std::sort
#include <cstdlib> // para rand, srand
#include <ctime>   // para time

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
void SistemaRecomendacion::agregarValoracion(int idUsuario, int idCancion, float valoracion) {
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
    actualizarTop(can);
}

// mostrar los datos
void SistemaRecomendacion::mostrarCanciones(){
    arbolCanciones.display();
}

void SistemaRecomendacion::mostrarUsuarios(){
    arbolUsuarios.display();
}

// devuelve primeros n usuarios que valoraron una canción
void SistemaRecomendacion::verPrimerosVotantes(int idCancion, int n) const {
    auto it = hashCanciones.find(idCancion);
    if (it == hashCanciones.end()) {
        cout << "Canción " << idCancion << " no encontrada.\n";
        return;
    }
    const auto& mapa = it->second->valoraciones;
    if (mapa.empty()) {
        cout << "La canción " << idCancion << " no tiene valoraciones.\n";
        return;
    }

    cout << "Primeros " << n << " votantes de la canción " << idCancion << ":\n";
    int contador = 0;
    for (const auto& par : mapa) {
        cout << "  - Usuario " << par.first 
             << " valoró " << par.second << "\n";
        if (++contador >= n) break;
    }
}

// devuelve top k canciones por promedio
vector<int> SistemaRecomendacion::getTopSongs(int k) const {
    // Recorrer todos los nodos hoja del árbol de canciones y recolectar (id, promedio)
    using Pair = pair<int, float>;
    vector<Pair> canciones;
    // Eliminada la línea de acceso a root, usamos hashCanciones
    for (const auto& par : hashCanciones) {
        int id = par.first;
        float prom = par.second->obtenerPromedio();
        canciones.emplace_back(id, prom);
    }
    // Ordenar por promedio descendente
    sort(canciones.begin(), canciones.end(), [](const Pair& a, const Pair& b) {
        return a.second > b.second;
    });
    // Tomar los primeros k
    vector<int> result;
    int n = std::min(k, static_cast<int>(canciones.size()));
    if (canciones.size() < k) {
        std::cout << "Solo hay " << canciones.size() << " canciones disponibles.\n";
    }
    for (int i = 0; i < n; ++i) {
        result.push_back(canciones[i].first);
    }
    // Imprimir las k canciones del top k
    std::cout << "Top " << n << " canciones (id, promedio):\n";
    for (int i = 0; i < n; ++i) {
        std::cout << i+1 << ". ID: " << canciones[i].first << ", Promedio: " << canciones[i].second << std::endl;
    }
    return result;
}

// calcula distancia manhattan entre dos mapas de valoraciones
// TODO: recorrer claves comunes y acumular |a[key] - b[key]|
float SistemaRecomendacion::distanciaManhattan(
        const unordered_map<int,float>& a,
        const unordered_map<int,float>& b) const {
    float suma = 0.0f;
    for (const auto& par : a) {
        int key = par.first;
        auto it = b.find(key);
        if (it != b.end()) {
            suma += std::abs(par.second - it->second);
        }
    }
    return suma;
}

// devuelve k usuarios más similares a idUsuario
    // TODO:
    // 1) recuperar mapa base de hashUsuarios[idUsuario]
    // 2) para cada otro usuario calcular distanciaManhattan
    // 3) ordenar por distancia y devolver primeros k ids
vector<int> SistemaRecomendacion::getSimilarUsers(int idUsuario,
                                                  int k) const {
    // 1) recuperar mapa base de hashUsuarios[idUsuario]
    auto itBase = hashUsuarios.find(idUsuario);
    if (itBase == hashUsuarios.end()) {
        std::cout << "Usuario " << idUsuario << " no encontrado.\n";
        return {};
    }
    const auto& mapaBase = itBase->second->valoraciones;
    // 2) para cada otro usuario calcular distanciaManhattan
    using DistUser = pair<float, int>; // (distancia, id)
    vector<DistUser> distancias;
    for (const auto& par : hashUsuarios) {
        int otroId = par.first;
        if (otroId == idUsuario) continue;
        const auto& mapaOtro = par.second->valoraciones;
        float dist = distanciaManhattan(mapaBase, mapaOtro);
        distancias.emplace_back(dist, otroId);
    }
    // 3) ordenar por distancia y devolver primeros k ids
    sort(distancias.begin(), distancias.end());
    vector<int> similares;
    int n = std::min(k, static_cast<int>(distancias.size()));
    for (int i = 0; i < n; ++i) {
        similares.push_back(distancias[i].second);
    }
    // Imprimir resultado
    std::cout << "Los usuarios con gustos parecidos son: ";
    for (int i = 0; i < n; ++i) {
        std::cout << similares[i];
        if (i < n-1) std::cout << ", ";
    }
    std::cout << std::endl;
    return similares;
}


// TODO:
// 1) obtener vecinos con getSimilarUsers(idUsuario, k)
// 2) recolectar canciones valoradas por vecinos y no por usuario
// 3) calcular promedio por canción, ordenar desc y devolver k ids
// recomienda k canciones basadas en vecinos de idUsuario
vector<int> SistemaRecomendacion::recommendSongs(int idUsuario,
                                                 int k) const {
    // 1) Obtener vecinos similares
    vector<int> vecinos = getSimilarUsers(idUsuario, k);
    // 2) Obtener canciones ya escuchadas por el usuario
    auto itU = hashUsuarios.find(idUsuario);
    if (itU == hashUsuarios.end()) {
        std::cout << "Usuario " << idUsuario << " no encontrado.\n";
        return {};
    }
    const auto& valoracionesUsuario = itU->second->valoraciones;
    // 3) Recolectar canciones valoradas por vecinos y no por el usuario
    unordered_map<int, vector<float>> cancionesVecinos; // idCancion -> lista de valoraciones
    for (int vecinoId : vecinos) {
        auto itVecino = hashUsuarios.find(vecinoId);
        if (itVecino == hashUsuarios.end()) continue;
        for (const auto& par : itVecino->second->valoraciones) {
            int idCancion = par.first;
            float val = par.second;
            if (valoracionesUsuario.find(idCancion) == valoracionesUsuario.end()) {
                cancionesVecinos[idCancion].push_back(val);
            }
        }
    }
    // 4) Calcular promedio por canción
    using Pair = pair<int, float>;
    vector<Pair> recomendaciones;
    for (const auto& par : cancionesVecinos) {
        float suma = 0.0f;
        for (float v : par.second) suma += v;
        float prom = suma / par.second.size();
        recomendaciones.emplace_back(par.first, prom);
    }
    // 5) Ordenar por promedio descendente
    sort(recomendaciones.begin(), recomendaciones.end(), [](const Pair& a, const Pair& b) {
        return a.second > b.second;
    });
    // 6) Tomar los primeros k
    vector<int> result;
    int n = std::min(k, static_cast<int>(recomendaciones.size()));
    if (recomendaciones.size() < k) {
        std::cout << "Solo hay " << recomendaciones.size() << " recomendaciones posibles.\n";
    }
    std::cout << "Canciones recomendadas para el usuario " << idUsuario << ": ";
    for (int i = 0; i < n; ++i) {
        result.push_back(recomendaciones[i].first);
        std::cout << recomendaciones[i].first;
        if (i < n-1) std::cout << ", ";
    }
    std::cout << std::endl;
    return result;
}

// agrupa usuarios en k clusters (implementación opcional)
vector<vector<int>> SistemaRecomendacion::clusterUsuarios(int k) const {
    // TODO: implementar clustering (p.ej. k-means ligero) o dejar vacío
    vector<vector<int>> clusters(k);
    vector<unordered_map<int, float>> centroides(k); // Cada centroide es un mapa de valoraciones
    vector<int> userIds;

    // 1. Obtener ids de todos los usuarios
    for (const auto& par : hashUsuarios) {
        userIds.push_back(par.first);
    }

    if (userIds.size() < k || k <= 0) {
        std::cout << "No se puede agrupar en " << k << " clusters con solo " << userIds.size() << " usuarios.\n";
        return {};
    }

    // 2. Inicializar centroides aleatorios
    srand(time(nullptr));
    for (int i = 0; i < k; ++i) {
        int randomIndex = rand() % userIds.size();
        centroides[i] = hashUsuarios.at(userIds[randomIndex])->valoraciones;
    }

    // 3. Iterar (fijo a 5 iteraciones para simplicidad)
    for (int iter = 0; iter < 5; ++iter) {
        // Limpiar clusters
        for (auto& cluster : clusters) cluster.clear();

        // Asignar usuarios al centroide más cercano
        for (int id : userIds) {
            const auto& valoraciones = hashUsuarios.at(id)->valoraciones;
            float minDist = distanciaManhattan(valoraciones, centroides[0]);
            int minIndex = 0;
            for (int i = 1; i < k; ++i) {
                float dist = distanciaManhattan(valoraciones, centroides[i]);
                if (dist < minDist) {
                    minDist = dist;
                    minIndex = i;
                }
            }
            clusters[minIndex].push_back(id);
        }

        // Recalcular centroides promediando los usuarios de cada cluster
        for (int i = 0; i < k; ++i) {
            unordered_map<int, float> nuevoCentroide;
            unordered_map<int, int> conteo;

            for (int id : clusters[i]) {
                const auto& valoraciones = hashUsuarios.at(id)->valoraciones;
                for (const auto& par : valoraciones) {
                    nuevoCentroide[par.first] += par.second;
                    conteo[par.first]++;
                }
            }

            for (auto& par : nuevoCentroide) {
                int key = par.first;
                par.second /= conteo[key]; // Promedio
            }

            centroides[i] = nuevoCentroide;
        }
    }

    // Mostrar clusters
    for (int i = 0; i < k; ++i) {
        std::cout << "Cluster " << i+1 << ": ";
        for (int id : clusters[i]) {
            std::cout << id << " ";
        }
        std::cout << "\n";
    }

    return clusters;    //Se retorna un vector<vector<int>>, donde cada subvector tiene los IDs de los usuarios en ese cluster.

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
    int id = cancion->idCancion;
    float promedio = cancion->obtenerPromedio();

    // eliminar si ya estaba
    top10.erase(remove_if(top10.begin(), top10.end(),
                        [id](const pair<int, float>& p) { return p.first == id; }),
                top10.end());

    top10.emplace_back(id, promedio);

    // ordenar y mantener top 10
    sort(top10.begin(), top10.end(),
        [](const pair<int, float>& a, const pair<int, float>& b) {
            return a.second > b.second;
        });

    if (top10.size() > 10) {
        top10.resize(10);
    }
}
