#pragma once
#define LEXER_H

enum TipoToken {
    EJECUCION,
    ESCRITORIO,
    /*Condicionales*/
    SI,
    NO,
    TALVES,
    REPETIR,
    FIN,
    COMPARACION_IGUAL,
    MAYOR,
    /*Interaccion con connsola o ventanas de Linux o Windows */
    VENTANA,
    COMA,
    ESCRIBIR_TEXTO,
    ESCRIBIR_VAR,
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
    CUADRADO,
    CIRCULO,
    TRIANGULO,
    VOMITOAZUCAR,
    POSCICION,
    /*Funciones futuras para POO (Proge¿amcion orientada a objetos)*/
    CLASE,
    PUBLICO,
    PRIVADO,
    PROTEGIDO,
    /*Es para recibir orientacion de los comandos */
    AYUDA
};
struct Token {
    TipoToken tipo;
    std::string texto;
};

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
        return { ID, resultado };
    }

    Token leerPalabra() {
        std::string resultado;
        while (pos < fuente.length() && (isalnum(fuente[pos]) || fuente[pos] == '_' || fuente[pos] == '.')) {
            resultado += fuente[pos++];
        }

        if (resultado == "si")               return { SI, resultado };
        if (resultado == "escritorio")       return { ESCRITORIO, resultado };
        if (resultado == "ejecutar")         return { EJECUCION, resultado };
        if (resultado == "no")               return { NO, resultado };
        if (resultado == "talves")           return { TALVES, resultado };
        if (resultado == "repetir")          return { REPETIR, resultado };
        if (resultado == "leer")             return { LEER, resultado };
        if (resultado == "escribir_texto")   return { ESCRIBIR_TEXTO, resultado };
        if (resultado == "escribir_variable") return { ESCRIBIR_VAR, resultado };
        if (resultado == "ventana")          return { VENTANA, resultado };
        if (resultado == "ayuda")            return { AYUDA, resultado };
        if (resultado == "vomito_azucar")    return { VOMITOAZUCAR, resultado };

        return { ID, resultado };
    }

    Token leerNumero() {
        std::string resultado;
        while (pos < fuente.length() && isdigit(fuente[pos])) {
            resultado += fuente[pos++];
        }
        return { NUMERO, resultado };
    }

public:
    Lexer(std::string codigo) : fuente(codigo), pos(0) {}

    // Tu método analizar principal
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
            if (actual == ',') {
                tokens.push_back({ COMA, "," });
                pos++;
                continue;
            }
            if (actual == '.') {
                tokens.push_back({ PUNTO, "." });
                pos++;
                continue;
            }
            if (actual == '>') {
                tokens.push_back({ MAYOR, ">" });
                pos++;
                continue;
            }

            if (actual == '=') {
                if (pos + 1 < fuente.length() && fuente[pos + 1] == '=') {
                    tokens.push_back({ COMPARACION_IGUAL, "==" });
                    pos += 2;
                }
                else {
                    tokens.push_back({ IGUAL, "=" });
                    pos++;
                }
                continue;
            }

            if (isalpha(actual)) {
                tokens.push_back(leerPalabra());
                continue;
            }

            if (isdigit(actual)) {
                tokens.push_back(leerNumero());
                continue;
            }

            pos++;
        }
        tokens.push_back({ FIN, "" });
        return tokens;
    }
};