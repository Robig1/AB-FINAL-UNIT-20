cmake_minimum_required(VERSION 3.5.0)
project(AB-FINAL VERSION 0.1.0 LANGUAGES C CXX)

# Set the source directory
set(SOURCE_DIR ${CMAKE_SOURCE_DIR}/src)

# Add the executable
add_executable(AB-FINAL
    ${SOURCE_DIR}/main.cpp
    ${SOURCE_DIR}/Persona.cpp
    ${SOURCE_DIR}/Paciente.cpp
    ${SOURCE_DIR}/Medico.cpp
    ${SOURCE_DIR}/Cita.cpp
    ${SOURCE_DIR}/GestionHospital.cpp
)

# Include directories
include_directories(${SOURCE_DIR})

# Enable testing
include(CTest)
enable_testing()

# CPack settings
set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

