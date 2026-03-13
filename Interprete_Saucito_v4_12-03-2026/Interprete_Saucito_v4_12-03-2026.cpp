#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <mutex>
#include "lexer.h"
#include "parser.h"
#include "ejecutor.h"

// --- DEFINICIÓN DE VARIABLES GLOBALES (Obligatorio para evitar errores de Linker) ---
std::map<std::string, float> memoriaSaucito;
std::mutex candadoMemoria;
bool ventanaActiva = false;
std::vector<ObjetoEscritorio> listaEscritorio;

int main(int argc, char* argv[]) {
    // 1. Verificación de argumentos
    if (argc < 2) {
        std::cout << "Uso: Saucito <archivo.sau>" << std::endl;
        // Para pruebas dentro de Visual Studio, podrías poner un nombre por defecto aquí
        return 1;
    }

    std::string filename = argv[1];
    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cerr << "Error: No se pudo abrir el archivo " << filename << std::endl;
        return 1;
    }

    // 2. Proceso de interpretación
    try {
        std::cout << "Interpretando: " << filename << "..." << std::endl;

        // Lectura del contenido
        std::string source((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

        // 1. Tokenización (Lexer)
        Lexer lexer(source);
        auto tokens = lexer.analizar();

        // 2. Análisis Sintáctico (Parser)
        Parser parser(tokens);
        auto ast = parser.analizarPrograma();

        // 3. Ejecución (Nodos del AST)
        for (auto nodo : ast) {
            if (nodo) {
                nodo->ejecutar();
            }
        }

        // Si la ventana de Raylib se abrió, esperamos a que se cierre
        if (ventanaActiva) {
            std::cout << "Esperando a que la ventana se cierre..." << std::endl;
            while (ventanaActiva) {
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
            }
        }

        std::cout << "Ejecucion finalizada con exito." << std::endl;

    }
    catch (const std::exception& e) {
        std::cerr << "Error en tiempo de ejecucion: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}