cmake_minimum_required(VERSION 3.27)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#! ! ! ! ! ! !
#set this to true to ship the game!
#basically this will change the RESOURCES_PATH to be the local path
#! ! ! ! ! ! !
#DELETE THE OUT FOLDER AFTER CHANGING THIS BECAUSE VISUAL STUDIO DOESN'T SEEM TO RECOGNIZE THIS CHANGE AND REBUILD!
option(PRODUCTION_BUILD "Make this a production build" OFF)
#DELETE THE OUT FOLDER AFTER CHANGING THIS BECAUSE VISUAL STUDIO DOESN'T SEEM TO RECOGNIZE THIS CHANGE AND REBUILD!


set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Release>:Release>")
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE) #link time optimization

if(MSVC) 
add_compile_options(/arch:AVX2) #make sure SIMD optimizations take place
endif()

project(EssentiaEngine)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_ASSIMP_TOOLS OFF CACHE BOOL "" FORCE)
set(ASSIMP_WARNINGS_AS_ERRORS OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_GLTF_IMPORTER ON CACHE BOOL "" FORCE)
set(OPENEXR_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(OPENEXR_IS_SUBPROJECT ON CACHE BOOL "" FORCE)
set(OPENEXR_BUILD_LIBS ON CACHE BOOL "" FORCE)
set(OPENEXR_BUILD_IMATH_ONLY ON CACHE BOOL "" FORCE)

set(OPENCV_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

# OpenCV Flags ----------------------------------------------------------------------------------------------------------------

# Configuración básica
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)            # No construir librerías compartidas (.dll o .so).
set(OPENCV_FORCE_3RDPARTY_BUILD ON CACHE BOOL "" FORCE)   # Forzar la construcción de dependencias de terceros.

# Módulos mínimos necesarios
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)               # No construir ejemplos.
set(BUILD_DOCS OFF CACHE BOOL "" FORCE)                   # No construir documentación.
set(BUILD_TESTS OFF CACHE BOOL "" FORCE)                  # No construir tests.
set(BUILD_PERF_TESTS OFF CACHE BOOL "" FORCE)             # No construir tests de rendimiento.
set(BUILD_opencv_apps OFF CACHE BOOL "" FORCE)            # No construir aplicaciones OpenCV.

# Módulos específicos útiles
set(BUILD_opencv_imgproc ON CACHE BOOL "" FORCE)          # Procesamiento de imágenes (filtros, convoluciones, etc.).
set(BUILD_opencv_imgcodecs ON CACHE BOOL "" FORCE)        # Soporte para cargar imágenes (JPEG, PNG, etc.).
set(BUILD_opencv_core ON CACHE BOOL "" FORCE)             # Núcleo de OpenCV (matrices, operaciones básicas).
set(BUILD_opencv_highgui OFF CACHE BOOL "" FORCE)         # Desactiva GUI (no se necesita en motores de juego).
set(BUILD_opencv_video OFF CACHE BOOL "" FORCE)           # Análisis de video (desactivar si no usas tracking).

# Optimización de compilación
set(WITH_OPENGL ON CACHE BOOL "" FORCE)                   # Activar OpenGL si quieres interoperabilidad con texturas.
set(WITH_TBB OFF CACHE BOOL "" FORCE)                     # TBB, procesamiento paralelo.
set(WITH_IPP ON CACHE BOOL "" FORCE)                      # Activar IPP (Intel Performance Primitives, optimización).

# Soporte de formatos de imagen
set(WITH_JPEG ON CACHE BOOL "" FORCE)                     # Soporte para imágenes JPEG.
set(WITH_PNG ON CACHE BOOL "" FORCE)                      # Soporte para imágenes PNG.
set(WITH_TIFF ON CACHE BOOL "" FORCE)                     # Soporte para imágenes TIFF.
set(WITH_OPENEXR OFF CACHE BOOL "" FORCE)                 # Desactivar soporte para OpenEXR.
set(WITH_WEBP OFF CACHE BOOL "" FORCE)                    # Desactivar WebP.

# Soporte de video (opcional)
set(WITH_FFMPEG ON CACHE BOOL "" FORCE)                   # Soporte para cargar videos y streams (opcional).
set(WITH_V4L OFF CACHE BOOL "" FORCE)                     # Desactivar Video4Linux (solo relevante para cámaras).

# Otros módulos redundantes
set(BUILD_opencv_flann OFF CACHE BOOL "" FORCE)           # No incluir FLANN (búsqueda aproximada, poco usado).
set(BUILD_opencv_ml OFF CACHE BOOL "" FORCE)              # Desactivar Machine Learning.
set(BUILD_opencv_dnn OFF CACHE BOOL "" FORCE)             # Desactivar redes neuronales (DNN).
set(BUILD_opencv_calib3d OFF CACHE BOOL "" FORCE)         # Desactivar calibración de cámaras y visión 3D.
set(BUILD_opencv_objdetect OFF CACHE BOOL "" FORCE)       # Desactivar detección de objetos.

# World Option
set(BUILD_opencv_world ON CACHE BOOL "" FORCE)            # Construir todo en una sola librería.

# OpenCV Flags ----------------------------------------------------------------------------------------------------------------

add_subdirectory(thirdparty/glfw-3.3.2)			#window oppener
add_subdirectory(thirdparty/glad)				#opengl loader
add_subdirectory(thirdparty/stb_image)			#loading immaged
add_subdirectory(thirdparty/stb_truetype)		#loading ttf files
add_subdirectory(thirdparty/nlohmann)			#json read/write
#add_subdirectory(thirdparty/enet-1.3.17)		#networking
#add_subdirectory(thirdparty/raudio)				#audio
add_subdirectory(thirdparty/glm)				#math
add_subdirectory(thirdparty/imgui-docking)		#ui
												
#add_subdirectory(thirdparty/safeSave)			#saving data, designed for gamedev. It also makes backups so the player won't loose saves.
#add_subdirectory(thirdparty/profilerLib)		#profiling, just a simpe library for measuring elapsed time
#add_subdirectory(thirdparty/gl2d)				#2D rendering library
#add_subdirectory(thirdparty/glui)				#ui library, usefull for making game menus
add_subdirectory(thirdparty/assimp-5.3.1)		#loading models
add_subdirectory(thirdparty/ska)				#ska::flat_hash_map

add_subdirectory(thirdparty/opencv-4.10.0)		#various functionalities
add_subdirectory(thirdparty/openexr-3.3.2)      #exr file read/write

# MY_SOURCES is defined to be a list of all the source files for my game 
# DON'T ADD THE SOURCES BY HAND, they are already added with this macro
file(GLOB_RECURSE MY_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

add_executable("${CMAKE_PROJECT_NAME}")

set_property(TARGET "${CMAKE_PROJECT_NAME}" PROPERTY CXX_STANDARD 17)

target_compile_definitions("${CMAKE_PROJECT_NAME}" PUBLIC GLFW_INCLUDE_NONE=1)
add_compile_definitions(OPENCV_IO_FORCE_OPENEXR=1)
add_compile_definitions(OPENCV_IO_ENABLE_OPENEXR=1)
add_compile_definitions(OPENCV_IMGCODECS_USE_OPENEXR=1)


if(PRODUCTION_BUILD)
	# setup the ASSETS_PATH macro to be in the root folder of your exe
	target_compile_definitions("${CMAKE_PROJECT_NAME}" PUBLIC RESOURCES_PATH="./resources/") 

	# remove the option to debug asserts.
	target_compile_definitions("${CMAKE_PROJECT_NAME}" PUBLIC PRODUCTION_BUILD=1) 

else()
	# This is useful to get an ASSETS_PATH in your IDE during development
	target_compile_definitions("${CMAKE_PROJECT_NAME}" PUBLIC RESOURCES_PATH="${CMAKE_CURRENT_SOURCE_DIR}/resources/")
	target_compile_definitions("${CMAKE_PROJECT_NAME}" PUBLIC PRODUCTION_BUILD=0) 

endif()

target_sources("${CMAKE_PROJECT_NAME}" PRIVATE ${MY_SOURCES} )


if(MSVC) # If using the VS compiler...

	target_compile_definitions("${CMAKE_PROJECT_NAME}" PUBLIC _CRT_SECURE_NO_WARNINGS)

	#set_target_properties("${CMAKE_PROJECT_NAME}" PROPERTIES LINK_FLAGS "/SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup") #no console
	
	#set_property(TARGET "${CMAKE_PROJECT_NAME}" PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreadedDebug<$<CONFIG:Debug>:Debug>")
	#set_property(TARGET "${CMAKE_PROJECT_NAME}" PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Release>:Release>")

endif()

target_include_directories("${CMAKE_PROJECT_NAME}" PUBLIC
	"${CMAKE_CURRENT_SOURCE_DIR}/include/"
    "${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/opencv-4.10.0/include"
    "${CMAKE_BINARY_DIR}"
)
ocv_target_include_modules("${CMAKE_PROJECT_NAME}" ${OPENCV_MODULES_BUILD})

#target_link_libraries("${CMAKE_PROJECT_NAME}" PRIVATE glm glfw 
#	glad stb_image stb_truetype gl2d raudio imgui safeSave profilerLib enet glui)

target_link_libraries("${CMAKE_PROJECT_NAME}" PRIVATE glm glfw glad stb_image stb_truetype imgui assimp ska_flat_hash_map nlohmann opencv_world OpenEXR)