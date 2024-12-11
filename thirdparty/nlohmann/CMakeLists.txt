cmake_minimum_required(VERSION 3.27)
project(nlohmann)

# Crea una biblioteca INTERFACE
add_library(nlohmann INTERFACE)

# Agrega el directorio actual como ruta de inclusión
target_include_directories(nlohmann INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}")
