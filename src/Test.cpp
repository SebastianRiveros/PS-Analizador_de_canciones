#include <iostream>
#include "SistemaRecomendacion.hpp"
#include "BPlusTree.hpp"

/*int main() {
    // Crear un B+ Tree de orden 64 (M = 4)
    BPlusTree<int, std::string, 64> tree;

    // Pruebas de inserción
    std::cout << "Insertando valores..." << std::endl;
    tree.insert(5, "cinco");
    tree.insert(15, "quince");
    tree.insert(25, "veinticinco");
    tree.insert(35, "treinta y cinco");
    tree.insert(45, "cuarenta y cinco");
    tree.insert(55, "cincuenta y cinco");
    tree.insert(65, "sesenta y cinco");

    std::cout << "Estructura del árbol tras inserciones:" << std::endl;
    tree.display();

    // Pruebas de búsqueda
    int keys_to_search[] = {15, 45, 100};
    for (int k : keys_to_search) {
        auto val = tree.search(k);
        if (val) std::cout << "Encontrado key=" << k << " -> " << *val << std::endl;
        else    std::cout << "No encontrado key=" << k << std::endl;
    }

    // Pruebas de eliminación
    std::cout << "\nEliminando algunos valores..." << std::endl;
    tree.remove(25);
    tree.remove(5);
    tree.remove(65);
    std::cout << "Estructura del árbol tras eliminaciones:" << std::endl;
    tree.display();

    // Verificar que las eliminaciones funcionaron
    int keys_to_check[] = {5, 25, 35};
    for (int k : keys_to_check) {
        auto val = tree.search(k);
        if (val) std::cout << "key=" << k << " sigue presente -> " << *val << std::endl;
        else    std::cout << "key=" << k << " correctamente eliminada" << std::endl;
    }
    
    // Pruebas de carga de datos:
    SistemaRecomendacion sistema;
<<<<<<< HEAD
    sistema.cargarDatosCSV("../data/ratings.csv");
    sistema.mostrarCanciones();
    cout << "Mostrando Usuarios" << std::endl;
    sistema.mostrarUsuarios();
    sistema.verPrimerosVotantes(1, 2);
=======
<<<<<<< HEAD
    sistema.cargarDatosCSV("../data/ratings_s.csv");
=======
    cout << "\nCargando datos desde CSV..." << std::endl;
    sistema.cargarDatosCSV("../data/ratings.csv");
    cout << "Datos cargados. Mostrando canciones y usuarios:" << std::endl;
>>>>>>> 7d70f8249ace2d0b7f481320f43d4b081399ceb8
    sistema.mostrarCanciones();
    cout << "Mostrando Usuarios" << std::endl;
    sistema.mostrarUsuarios();
<<<<<<< HEAD
    sistema.verPrimerosVotantes(1, 100);
=======
    cout << "Primeros votantes de la canción 1:" << std::endl;
    sistema.verPrimerosVotantes(1, 2);
    cout << "Top 5 canciones:" << std::endl;
    sistema.getTopSongs(5);
    cout << "Usuarios 5 usuarios similares al usuario 1:" << std::endl;
    sistema.getSimilarUsers(1, 5);
    cout << "Recomendando 5 canciones para el usuario 1:" << std::endl;
    sistema.recommendSongs(1, 5);

>>>>>>> 7d70f8249ace2d0b7f481320f43d4b081399ceb8
>>>>>>> 62fc9f5 (ajustes)
    return 0;
}
*/