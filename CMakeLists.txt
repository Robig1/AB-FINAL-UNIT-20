cmake_minimum_required(VERSION 3.12)

# Habilite Recarga activa para los compiladores de MSVC si se admiten.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project("Intermedia-3")

# Configuración del estándar de C++
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Incluir directorios de encabezados
include_directories(include)

# Agregar archivos fuente
file(GLOB_RECURSE SOURCES "src/*.cpp")

# Crear el ejecutable
add_executable(Intermedia-3 ${SOURCES} )

# Opcional: Agregar pruebas y destinos de instalación
# TODO: Agregue pruebas y destinos de instalación si es necesario.
