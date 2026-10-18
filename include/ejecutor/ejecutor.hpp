#pragma once
#define EJECUTOR_H


#include <iostream>
#include "raylib.h"
#include <string>
#include <vector>
#include <map>
#include <mutex>
#include <thread>
#include <chrono>

// --- VARIABLES GLOBALES ---
extern std::map<std::string, float> memoriaSaucito;
extern std::mutex candadoMemoria;
extern bool ventanaActiva;

// Estructura para el escritorio
struct ObjetoEscritorio {
    std::string nombre;
    float x, y;
};
extern std::vector<ObjetoEscritorio> listaEscritorio;

// Prototipo de la función que maneja la ventana (Raylib)
void EjecutarVentana() {
    if (ventanaActiva) return;
    SetTraceLogLevel(LOG_NONE);
    ventanaActiva = true;


    // 1. Inicializar la ventana
    InitWindow(800, 600, "Saucito v2 - Motor Grafico");
    SetTargetFPS(60);

    while (!WindowShouldClose() && ventanaActiva) {
        // --- LOGICA DE ACTUALIZACION ---
        float posX, posY;
        {
            // Bloqueamos la memoria para leer x e y de forma segura (Multihilo)
            std::lock_guard<std::mutex> lock(candadoMemoria);
            posX = (memoriaSaucito.count("x")) ? memoriaSaucito["x"] : 400;
            posY = (memoriaSaucito.count("y")) ? memoriaSaucito["y"] : 300;
        }

        // --- DIBUJADO ---
        BeginDrawing();
        ClearBackground(RAYWHITE);

        // 2. Dibujar los objetos del escritorio
        // Usamos el struct ObjetoEscritorio que definiste en ejecutor.h
        for (const auto& objeto : listaEscritorio) {
            DrawRectangle(objeto.x, objeto.y, 60, 60, BLUE);
            DrawText(objeto.nombre.c_str(), objeto.x, objeto.y + 65, 10, DARKGRAY);
        }

        // 3. Dibujar el "cursor" o círculo de control
        DrawCircle(posX, posY, 20, MAROON);

        DrawText("Saucito v2 - Consola Activa", 10, 10, 20, LIGHTGRAY);
        EndDrawing();
    }

    CloseWindow();
    ventanaActiva = false;
}

// --- CLASE BASE DEL AST ---
class NodoAST {
public:
    virtual ~NodoAST() = default;
    virtual void ejecutar() = 0; // El motor recorrerá los nodos llamando a esto
};

// --- NODOS DE EJECUCIÓN ---

class NodoAsignacion : public NodoAST {
    std::string nombre;
    float valor;
public:
    NodoAsignacion(std::string n, float v) : nombre(n), valor(v) {}
    void ejecutar() override {
        std::lock_guard<std::mutex> lock(candadoMemoria);
        memoriaSaucito[nombre] = valor;
        std::cout << "[AST-Exec] " << nombre << " = " << valor << std::endl;
    }
};

class NodoEjecutar : public NodoAST {
    std::string nombreArchivo;
public:
    NodoEjecutar(std::string nombre) : nombreArchivo(nombre) {}
    std::string getNombre() { return nombreArchivo; }
    void ejecutar() override {
        std::cout << "[Sistema] Preparando ejecucion de: " << nombreArchivo << std::endl;
    }
};

class NodoVomitoAzucar : public NodoAST {
public:
    void ejecutar() override {
#ifdef _WIN32
        system("color a");
#else
        std::cout << "\033[1;32m";
#endif
        std::cout << "[Sistema] Terminal contaminada con azucar..." << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(2));
        std::cout << "[Sistema] Saucito ya limpio perdon..." << std::endl;
#ifdef _WIN32
        system("color 07");
#else
        std::cout << "\033[0m";
#endif
    }
};

class NodoVentana : public NodoAST {
public:
    void ejecutar() override {
        if (!ventanaActiva) {
            std::thread(EjecutarVentana).detach();
        }
    }
};

class NodoEscribirTexto : public NodoAST {
    std::string contenido;
public:
    NodoEscribirTexto(std::string texto) : contenido(texto) {}
    void ejecutar() override {
        std::cout << "[Saucito] " << contenido << std::endl;
    }
};

class NodoEscribirVar : public NodoAST {
    std::string nombre;
public:
    NodoEscribirVar(std::string n) : nombre(n) {}
    void ejecutar() override {
        if (memoriaSaucito.count(nombre)) {
            std::cout << "[Saucito] " << nombre << " = " << memoriaSaucito[nombre] << std::endl;
        }
    }
};

class NodoEscritorio : public NodoAST {
    std::string nombre;
    float x, y;
public:
    NodoEscritorio(std::string n, float posX, float posY) : nombre(n), x(posX), y(posY) {}
    void ejecutar() override {
        listaEscritorio.push_back({ nombre, x, y });
        std::cout << "[Escritorio] Objeto '" << nombre << "' creado en (" << x << ", " << y << ")" << std::endl;
    }
};