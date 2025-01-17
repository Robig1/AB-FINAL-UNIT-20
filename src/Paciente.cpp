#include "Paciente.hpp"

Paciente::Paciente(std::string n, std::string id, std::string fecha, bool cronic)
    : Persona(n, id, fecha), tieneEnfermedadCronica_(cronic) {}

void Paciente::registrarHistorial(std::string historial) {
    historialClinico.push_back(historial);
}

std::vector<std::string> Paciente::getHistorialClinico() const {
    return historialClinico;
}

std::string Paciente::toCSV() const {
    return getNombre() + "," + getIdentificacion() + "," + getFechaIngreso() + "," + (tieneEnfermedadCronica_ ? "Si" : "No");
}

bool Paciente::tieneEnfermedadCronica() const {
    return tieneEnfermedadCronica_;
}

void Paciente::setEnfermedadCronica(bool cronic) {
    tieneEnfermedadCronica_ = cronic;
}

