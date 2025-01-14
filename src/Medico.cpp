#include "Medico.hpp"

Medico::Medico(std::string n, std::string id, std::string fecha, std::string esp, bool disp)
    : Persona(n, id, fecha), especialidad(esp), disponible(disp) {}

std::string Medico::getEspecialidad() const {
    return especialidad;
}

bool Medico::isDisponible() const {
    return disponible;
}

void Medico::setDisponibilidad(bool disp) {
    disponible = disp;
}

std::string Medico::toCSV() const {
    return getNombre() + "," + getIdentificacion() + "," + getFechaIngreso() + "," + especialidad + "," + (disponible ? "Si" : "No");
}

