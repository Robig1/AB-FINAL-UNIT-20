#include "paciente.hpp"
#include <iostream>
#include <sstream>
#include <algorithm>

Paciente::Paciente(std::string n, std::string i, const Fecha& f, std::string gs)
    : Persona(n, i), fechaIngreso(f), grupoSanguineo(gs), activo(true) {}

void Paciente::agregarHistorial(const HistorialClinico& entrada) {
    historial.push_back(entrada);
}

void Paciente::agregarAlergia(const std::string& alergia) {
    alergias.insert(alergia);
}

bool Paciente::tieneEnfermedadCronica() const {
    return std::any_of(historial.begin(), historial.end(),
                       [](const HistorialClinico& h) { return h.esCronico; });
}

const std::vector<HistorialClinico>& Paciente::getHistorial() const {
    return historial;
}

Fecha Paciente::getFechaIngreso() const { return fechaIngreso; }
bool Paciente::isActivo() const { return activo; }
void Paciente::setActivo(bool a) { activo = a; }

std::string Paciente::toCSV() const {
    std::string csv = Persona::toCSV() + "," + fechaIngreso.toString() + "," +
                      grupoSanguineo + "," + std::to_string(activo);

    csv += ",";
    for (const auto& alergia : alergias) {
        csv += alergia + ";";
    }

    csv += ",";
    for (const auto& h : historial) {
        csv += h.toString() + ";";
    }

    return csv;
}

void Paciente::fromCSV(const std::string& csv) {
    std::stringstream ss(csv);
    std::string token;
    std::vector<std::string> tokens;

    while (std::getline(ss, token, ',')) {
        tokens.push_back(token);
    }

    if (tokens.size() >= 7) {
        Persona::fromCSV(tokens[0] + "," + tokens[1] + "," + tokens[2] + "," + tokens[3]);
        fechaIngreso = Fecha::fromString(tokens[4]);
        grupoSanguineo = tokens[5];
        activo = (tokens[6] == "1");

        std::stringstream ss_alergias(tokens[7]);
        std::string alergia;
        while (std::getline(ss_alergias, alergia, ';')) {
            if (!alergia.empty()) {
                alergias.insert(alergia);
            }
        }

        if (tokens.size() > 8) {
            std::stringstream ss_historial(tokens[8]);
            std::string entrada;
            while (std::getline(ss_historial, entrada, ';')) {
                if (!entrada.empty()) {
                    // Aquí puedes agregar la lógica para manejar las entradas del historial
                }
            }
        }
    }
}

void Paciente::mostrarInfo() const {
    std::cout << "Paciente: " << nombre << " (ID: " << id << ")\n"
              << "Contacto: " << telefono << " / " << email << "\n"
              << "Fecha de ingreso: " << fechaIngreso.toString() << "\n"
              << "Grupo sanguíneo: " << grupoSanguineo << "\n"
              << "Estado: " << (activo ? "Activo" : "Inactivo") << "\n"
              << "Alergias: ";

    for (const auto& alergia : alergias) {
        std::cout << alergia << ", ";
    }
    std::cout << "\nHistorial Clinico:\n";
    for (const auto& h : historial) {
        std::cout << "- " << h.toString() << "\n";
    }
}

