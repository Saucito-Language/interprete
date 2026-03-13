#include <iostream>
#include <fstream>
#include <string>
#include <curl/curl.h>
#include <cstdlib> // Para system()
#include <chrono>  // Para pausas estándar
#include <thread>  // Para std::this_thread::sleep_for

size_t writeData(void* ptr, size_t size, size_t nmemb, FILE* stream) {
    return fwrite(ptr, size, nmemb, stream);
}

void AplicarActualizacion() {
    std::cout << "\n[Saucito] Instalando actualizacion..." << std::endl;

    // Usamos 'start /b' para que el comando corra en segundo plano mientras este programa se cierra
    // El comando espera 2 segundos para asegurar que el .exe original se libere
    std::string comando = "cmd /C timeout /t 2 /nobreak && del Saucito.exe && move update_temp.exe Saucito.exe && start Saucito.exe";

    std::cout << "[Sistema] Ejecutando script de reemplazo..." << std::endl;

    // Ejecutar el comando de forma asíncrona (Windows)
    system(comando.c_str());

    exit(0);
}

int main() {
    const char* url = "https://tuservidor.com/Saucito_v3.exe";
    const char* output = "update_temp.exe";

    CURL* curl = curl_easy_init();
    if (curl) {
        FILE* file = fopen(output, "wb");
        if (!file) {
            std::cerr << "Error: No se pudo crear el archivo temporal." << std::endl;
            return 1;
        }

        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeData);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, file);

        // IMPORTANTE para HTTPS en servidores sin certificados configurados:
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);

        std::cout << "Descargando Saucito v3 desde la nube..." << std::endl;
        CURLcode res = curl_easy_perform(curl);

        curl_easy_cleanup(curl);
        fclose(file);

        if (res == CURLE_OK) {
            std::cout << "¡Descarga completada con exito!" << std::endl;
            AplicarActualizacion();
        }
        else {
            std::cerr << "Error de CURL: " << curl_easy_strerror(res) << std::endl;
            // Si falló, borramos el archivo basura
            remove(output);
        }
    }

    return 0;
}