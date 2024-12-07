#include "especialidad.hpp"

std::string especialidadToString(Especialidad esp) {
    switch(esp) {
        case Especialidad::CARDIOLOGIA: return "Cardiologia";
        case Especialidad::DERMATOLOGIA: return "Dermatologia";
        case Especialidad::PEDIATRIA: return "Pediatria";
        case Especialidad::TRAUMATOLOGIA: return "Traumatologia";
        case Especialidad::NEUROLOGIA: return "Neurologia";
        case Especialidad::GENERAL: return "Medicina General";
        default: return "Desconocida";
    }
}

Especialidad stringToEspecialidad(const std::string& esp) {
    if (esp == "Cardiologia") return Especialidad::CARDIOLOGIA;
    if (esp == "Dermatologia") return Especialidad::DERMATOLOGIA;
    if (esp == "Pediatria") return Especialidad::PEDIATRIA;
    if (esp == "Traumatologia") return Especialidad::TRAUMATOLOGIA;
    if (esp == "Neurologia") return Especialidad::NEUROLOGIA;
    return Especialidad::GENERAL;
}

