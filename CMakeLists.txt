cmake_minimum_required(VERSION 3.10)
project(SaucitoV2)

# Configuración del estándar C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 1. Encontrar Raylib (Asegúrate de tenerla instalada en tu sistema)
find_package(raylib REQUIRED)

# 2. Definir los archivos fuentes
set(SOURCES 
    "Interprete_Saucito_v4_12-032026.cpp"

    # Si decides mover el main a un archivo independiente, añádelo aquí
)

# 3. Crear el ejecutable
add_executable(SaucitoV2 ${SOURCES})

# 4. Vincular las dependencias
target_link_libraries(SaucitoV2 PRIVATE raylib)

# Opcional: Si estás en Linux o usas threads, vincula pthreads
if(UNIX AND NOT APPLE)
    target_link_libraries(SaucitoV2 PRIVATE pthread)
endif()