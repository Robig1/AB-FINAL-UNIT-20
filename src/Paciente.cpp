#include "Paciente.hpp"

Paciente::Paciente(std::string n, std::string id, std::string fecha)
    : Persona(n, id, fecha) {}

void Paciente::registrarHistorial(std::string historial) {
    historialClinico.push_back(historial);
}

std::vector<std::string> Paciente::getHistorialClinico() const {
    return historialClinico;
}

std::string Paciente::toCSV() const {
    return getNombre() + "," + getIdentificacion() + "," + getFechaIngreso();
}

