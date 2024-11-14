cmake_minimum_required(VERSION 3.27)
project(ska_flat_hash_map)

# Crea una biblioteca INTERFACE
add_library(ska_flat_hash_map INTERFACE)

# Agrega el directorio actual como ruta de inclusión
target_include_directories(ska_flat_hash_map INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}")
