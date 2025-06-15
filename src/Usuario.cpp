#include <unordered_map>
#include <memory>
using namespace std;

class Usuario {
public:
    int idUsuario;
    unordered_map<int, float> valoraciones; // idCancion → valoración
    explicit Usuario(int id): idUsuario(id) {}
};

using UsuarioPtr = shared_ptr<Usuario>;
