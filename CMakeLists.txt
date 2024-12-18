cmake_minimum_required(VERSION 3.12)

# Configuración del proyecto
project("Intermedia-3")

# Especificación de C++ estándar
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Configuración del compilador
set(CMAKE_CXX_COMPILER "g++") # clang++/g++/icx/cl

# Configuración de compilación (Debug/Release)
set(CMAKE_BUILD_TYPE Debug) # Puedes cambiar a Release si lo deseas

# Configuración específica del compilador
if (CMAKE_CXX_COMPILER STREQUAL "g++")
    add_compile_options(
        "$<$<CONFIG:Debug>:-pedantic;-Wall;-Wextra;-Wconversion;-Wshadow;-Wcast-qual;-Wwrite-strings;-O0;-ggdb3;-DDEBUG>"
        "$<$<CONFIG:Release>:-pedantic;-Wall;-O3>"
    )
else()
    message(WARNING "Compilador no soportado. No se aplican banderas específicas.")
endif()

# Verificar compilador activo
message(STATUS "CMAKE_CXX_COMPILER: ${CMAKE_CXX_COMPILER}")
message(STATUS "CMAKE_CXX_COMPILER_ID: ${CMAKE_CXX_COMPILER_ID}")
message(STATUS "CMAKE_CXX_COMPILER_VERSION: ${CMAKE_CXX_COMPILER_VERSION}")

# Configuración del ejecutable como aplicación de consola
add_executable(Intermedia-3 
    src/main.cpp
    src/fecha.cpp
    src/especialidad.cpp
    src/persona.cpp
    src/paciente.cpp
    src/medico.cpp
    src/historial_clinico.cpp
    src/cita.cpp
    src/sistema_gestion_clinica.cpp
)

# Directorios de encabezados
target_include_directories(Intermedia-3 PRIVATE ${PROJECT_SOURCE_DIR}/include)
# Directorio de salida
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Instalación
install(TARGETS Intermedia-3 RUNTIME DESTINATION bin)

# Pruebas automáticas para verificar la configuración
enable_testing()

add_test(
    NAME CompiladorCorrecto
    COMMAND ${CMAKE_COMMAND} -E echo "Compilador: ${CMAKE_CXX_COMPILER_ID}, Versión: ${CMAKE_CXX_COMPILER_VERSION}"
)

