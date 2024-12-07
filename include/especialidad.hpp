#ifndef ESPECIALIDAD_HPP
#define ESPECIALIDAD_HPP

#include <string>

enum class Especialidad {
    CARDIOLOGIA,
    DERMATOLOGIA,
    PEDIATRIA,
    TRAUMATOLOGIA,
    NEUROLOGIA,
    GENERAL
};

std::string especialidadToString(Especialidad esp);
Especialidad stringToEspecialidad(const std::string& esp);

#endif // ESPECIALIDAD_HPP

