#include <string>
#include <cstdlib>
#include <vector>
#include <iostream>
#include <raylib.h>
#include <fstream>
#include <thread>
#include <map>

// Nuestra memoria: guarda un nombre (string) y un valor (int)
std::map<std::string,float> memoriaSaucito;
struct ClaseSaucito {
    std::string nombre;
    std::map<std::string, float> propiedades; // Ejemplo: "vida" -> 100
};


std::map<std::string, ClaseSaucito> diccionarioClases; // Las plantillas
std::map<std::string, std::map<std::string, float>> objetosInstanciados; // Los datos reales
bool ventanaActiva = false; 
// Nos dirá si la ventana debe estar abierta
void EjecutarVentana() {
    if (ventanaActiva) return;
    ventanaActiva = true;
    SetTraceLogLevel(LOG_NONE);
    InitWindow(800, 450, "Saucito v2 - Motor Grafico");
    SetTargetFPS(60);

    while (!WindowShouldClose() && ventanaActiva) {
        // --- BUSCAR COORDENADAS EN LA MEMORIA ---
        // Si el usuario escribió "x = 100", lo usamos. Si no, usamos 190.
        float posX= (memoriaSaucito.count("x")) ? memoriaSaucito["x"] : 190;
        float posY= (memoriaSaucito.count("y")) ? memoriaSaucito["y"] : 200;

        BeginDrawing();
        ClearBackground(RAYWHITE);  

        // Dibujamos un círculo que se mueve según la memoria
        DrawCircle(posX, posY, 40, MAROON);

        DrawText("Controles desde consola:", 10, 10, 20, DARKGRAY);
        DrawText("x = numero | y = numero", 10, 40, 20, MAROON);
        EndDrawing();
    }

    CloseWindow();
    ventanaActiva = false;
    std::cout << "\n[Sistema] Ventana cerrada correctamente." << std::endl;
}

enum TipoToken {
    FIN,
    SI,
    NO,
    TALVES,
    REPETIR,
    VENTANA,
    ESCRIBIR,
    LEER,
    ID,
    NUMERO,
    PUNTO,
    ERROR,
    MAS,
    MENOS,
    IGUAL,
    ENTRE, 
    DIBUJAR, 
    CLASE, 
    PUBLICO, 
    PRIVADO, 
    PROTEGIDO, 
    CUADRADO, 
    CIRCULO,
    TRIANGULO,
    VOMITOAZUCAR,
    POSCICION,
    AYUDA
};

struct Token {
    TipoToken tipo;
    std::string texto;
};


/*El lexer*/
class Lexer {
    std::string fuente;
    size_t pos;
private:
    Token leerTextoLiteral() {
        pos++; // Saltamos la comilla de apertura (")
        std::string resultado;
        while (pos < fuente.length() && fuente[pos] != '"') {
            resultado += fuente[pos++];
        }
        if (pos < fuente.length()) pos++; // Saltamos la comilla de cierre (")

        // Usamos el tipo ID por ahora para guardar el texto literal
        return { ID, resultado };
    }
public:
    Lexer(std::string codigo) : fuente(codigo), pos(0) {}

    std::vector<Token> analizar() {
        std::vector<Token> tokens;
        while (pos < fuente.length()) {
            char actual = fuente[pos];

            if (isspace(actual)) {
                pos++;
                continue;
            }

            if (actual == '"') {
                tokens.push_back(leerTextoLiteral());
                continue;
            }
            // En tu Lexer, dentro del while de analizar():
            if (actual == '.') {
                tokens.push_back({ PUNTO, "." }); // Agrega PUNTO a tu TipoToken
                pos++;
                continue;
            }
            // Dentro del while de analizar()
            if (actual == '=') {
                tokens.push_back({ IGUAL, "=" });
                pos++;
                continue;
            }
            if (isalpha(actual)) {
                tokens.push_back(leerPalabra());
                continue;
            }

            // --- RECUERDA AÑADIR ESTO PARA LOS NÚMEROS ---
            if (isdigit(actual)) {
                tokens.push_back(leerNumero());
                continue;
            }

            pos++; // Para cualquier otro caracter no reconocido
        }
        tokens.push_back({ FIN, "" });
        return tokens;
    }

private:
    Token leerPalabra() {
        std::string resultado;
        // En leerPalabra(), cambia la condición del while:
        while (pos < fuente.length() && (isalnum(fuente[pos]) || fuente[pos] == '_')) {
            resultado += fuente[pos++];
        }

        // Lógica de palabras reservadas
        if (resultado == "si")       return { SI, resultado };
        if (resultado == "no")       return { NO, resultado };
        if (resultado == "talves")   return { TALVES, resultado };
        if (resultado == "repetir")  return { REPETIR, resultado };
        if (resultado == "leer") return { LEER, resultado };
        if (resultado == "escribir") return { ESCRIBIR, resultado };
        if (resultado == "ventana") return{ VENTANA,resultado };
        if (resultado == "ayuda") return{ AYUDA };
        if (resultado == "vomito_azucar") return{ VOMITOAZUCAR,resultado };



        return { ID, resultado }; // Si no es reserva, es un ID (nombre de variable)
    }

    Token leerNumero() {
        std::string resultado;
        while (pos < fuente.length() && isdigit(fuente[pos])) {
            resultado += fuente[pos++];
        }
        return { NUMERO, resultado };
    }
};

//Parser
class Parser {
    std::vector<Token> tokens;
    size_t actual;

public:
    Parser(std::vector<Token> t) : tokens(t), actual(0) {}

    // Función principal para iniciar el análisis
    void parsear() {
        while (actualToken().tipo != FIN) {
            sentencia();
        }
    }

private:
    Token actualToken() {
        return tokens[actual];
    }

    void consumir(TipoToken tipo) {
        if (actualToken().tipo == tipo) {
            actual++;
        }
        else {
            std::cout << "Error sintáctico: se esperaba otro token." << std::endl;
        }
    }

    // Aquí defines las reglas de tu lenguaje
    void sentencia() {
        if (actualToken().tipo == SI) {
            consumir(SI);
            std::cout << "Se detectó un inicio de condición (SI)" << std::endl;

        }
        else if (actualToken().tipo == ID) {
            std::string nombreVar = actualToken().texto;
            consumir(ID);

            if (actualToken().tipo == IGUAL) {
                consumir(IGUAL);
                if (actualToken().tipo == NUMERO) {
                    // Guardamos en el map convirtiendo el texto a número
                    memoriaSaucito[nombreVar] = std::stof(actualToken().texto);
                    std::cout << "[RAM] " << nombreVar << " guardado como " << memoriaSaucito[nombreVar] << std::endl;
                    consumir(NUMERO);
                }
            }
        }
        else if (actualToken().tipo == AYUDA) {
            consumir(AYUDA);
            std::cout << "Los comandos actuales de saucito es:\n1.-escribir(escribe en la consola)\n2.-ventana(abre una ventana grafica)" << std::endl;

        }
        else if (actualToken().tipo == ESCRIBIR) {
            consumir(ESCRIBIR);

            if (actualToken().tipo == ID) {
                std::string nombreVar = actualToken().texto;

                // --- AQUÍ ESTÁ EL TRUCO ---
                // Buscamos en la memoria si existe una variable con ese nombre
                if (memoriaSaucito.count(nombreVar)) {
                    std::cout << "[Saucito] " << nombreVar << " vale: " << memoriaSaucito[nombreVar] << std::endl;
                }
                else {
                    // Si no existe en memoria, imprimimos el texto tal cual (como antes)
                    std::cout << "Escribiendo texto: " << nombreVar << std::endl;
                }
                actual++;
            }
            else if (actualToken().tipo == NUMERO) {
                std::cout << "Escribiendo numero: " << actualToken().texto << std::endl;
                actual++;
            }
        }
        // Dentro de la clase Parser, en el bloque else if (actualToken().tipo == LEER)
        else if (actualToken().tipo == LEER) {
            consumir(LEER); // ¡No olvides consumir el token!
            std::string valor;
            std::cout << "Leyendo valor (escribelo en la consola): ";
            std::cin >> valor;
            std::cin.ignore(); // Limpia el buffer para que el siguiente getline no falle
            std::cout << "Saucito ha guardado: " << valor << std::endl;
        }
        else if (actualToken().tipo == VENTANA) {
            consumir(VENTANA);
            if (!ventanaActiva) {
                std::cout << "Iniciando interfaz..." << std::endl;
                std::thread(EjecutarVentana).detach();
            }
            else {
                std::cout << "La ventana ya esta abierta." << std::endl;
            }
        }
        else {
            // Si no reconoce la frase, salta para no entrar en bucle infinito
            actual++;
        }
    }
};

int main() {
    // 1. Quitamos el while(!WindowShouldClose()) de aquí arriba.
    system("clear");    
    std::string n;
    std::cout << "Que tal iniciaste saucito." << std::endl;
    std::cout << "Escribe el codigo a analizar : " << std::endl;

    while (true) { // Bucle para que puedas escribir varios comandos
        std::cout << "> ";
        std::getline(std::cin, n);

        if (n == "salir") break;
        

        Lexer lexer(n);
        std::vector<Token> misTokens = lexer.analizar();
        try {
    Parser parser(misTokens);
    parser.parsear();
}
catch (const std::exception& e) {
    std::cout << "Error detectado: " << e.what() << std::endl;
}
catch (...) {
    std::cout << "Error desconocido en el parser." << std::endl;
}
    }

    return 0;
}
