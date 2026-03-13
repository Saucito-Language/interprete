#pragma once
#define PARSER_H
#include <vector>
#include <iostream>
#include "lexer.h"
#include "EJECUTOR.h"

class Parser {
    std::vector<Token> tokens;
    size_t actual;

public:
    Parser(std::vector<Token> t) : tokens(t), actual(0) {}

    std::vector<NodoAST*> analizarPrograma() {
        std::vector<NodoAST*> ast;
        while (actual < tokens.size() && tokens[actual].tipo != FIN) {
            NodoAST* nodo = sentencia();
            if (nodo) ast.push_back(nodo);

            // Consumir comas si existen entre sentencias
            if (actual < tokens.size() && tokens[actual].tipo == COMA) consumir(COMA);
        }
        return ast;
    }

private:
    Token& actualToken() { return tokens[actual]; }

    void consumir(TipoToken tipo) {
        if (actual < tokens.size() && tokens[actual].tipo == tipo) actual++;
    }

    NodoAST* sentencia() {
        Token t = actualToken();

        switch (t.tipo) {
        case ID: {
            std::string nombre = t.texto;
            consumir(ID);
            if (actualToken().tipo == IGUAL) {
                consumir(IGUAL);
                if (actualToken().tipo == NUMERO) {
                    float val = std::stof(actualToken().texto);
                    consumir(NUMERO);
                    return new NodoAsignacion(nombre, val);
                }
            }
            break;
        }
        case ESCRITORIO: {
            consumir(ESCRITORIO);
            std::string nombre = actualToken().texto;
            consumir(ID);
            consumir(COMA);
            float x = std::stof(actualToken().texto);
            consumir(NUMERO);
            consumir(COMA);
            float y = std::stof(actualToken().texto);
            consumir(NUMERO);
            return new NodoEscritorio(nombre, x, y);
        }
        case VOMITOAZUCAR:
            consumir(VOMITOAZUCAR);
            return new NodoVomitoAzucar();
        case VENTANA:
            consumir(VENTANA);
            return new NodoVentana();
        case ESCRIBIR_TEXTO: {
            consumir(ESCRIBIR_TEXTO);
            std::string texto = actualToken().texto;
            consumir(ID);
            return new NodoEscribirTexto(texto);
        }
                           // ... añade aquí el resto de tus casos (EJECUCION, AYUDA, etc.)
        default:
            actual++; // Saltar tokens desconocidos
            break;
        }
        return nullptr;
    }
};