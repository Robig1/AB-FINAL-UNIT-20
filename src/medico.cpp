#include "medico.hpp"
#include <iostream>
#include <sstream>
#include <algorithm>

Medico::Medico(std::string n, std::string i, Especialidad esp)
    : Persona(n, i), especialidad(esp), disponible(true) {}

Especialidad Medico::getEspecialidad() const { return especialidad; }
bool Medico::isDisponible() const { return disponible; }
void Medico::setDisponibilidad(bool d) { disponible = d; }

void Medico::agregarCita(const Fecha& fecha, const std::string& idCita) {
    citasAsignadas[fecha].push_back(idCita);
}

void Medico::eliminarCita(const Fecha& fecha, const std::string& idCita) {
    auto it = citasAsignadas.find(fecha);
    if (it != citasAsignadas.end()) {
        auto& citas = it->second;
        citas.erase(std::remove(citas.begin(), citas.end(), idCita), citas.end());
    }
}

bool Medico::tieneDisponibilidad(const Fecha& fecha) const {
    auto it = citasAsignadas.find(fecha);
    return it == citasAsignadas.end() || it->second.size() < 8;
}

std::string Medico::toCSV() const {
    return Persona::toCSV() + "," + especialidadToString(especialidad) + "," +
           std::to_string(disponible);
}

void Medico::fromCSV(const std::string& csv) {
    std::stringstream ss(csv);
    std::string token;
    std::vector<std::string> tokens;

    while (std::getline(ss, token, ',')) {
        tokens.push_back(token);
    }

    if (tokens.size() >= 6) {
        Persona::fromCSV(tokens[0] + "," + tokens[1] + "," + tokens[2] + "," + tokens[3]);
        especialidad = stringToEspecialidad(tokens[4]);
        disponible = (tokens[5] == "1");
    }
}

void Medico::mostrarInfo() const {
    std::cout << "Medico: " << nombre << " (ID: " << id << ")\n"
              << "Contacto: " << telefono << " / " << email << "\n"
              << "Especialidad: " << especialidadToString(especialidad) << "\n"
              << "Estado: " << (disponible ? "Disponible" : "No disponible") << "\n"
              << "Citas asignadas:\n";

    for (const auto& [fecha, citas] : citasAsignadas) {
        std::cout << fecha.toString() << ": " << citas.size() << " citas\n";
    }
}

