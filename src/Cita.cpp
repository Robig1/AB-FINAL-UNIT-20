#include "Cita.hpp"

Cita::Cita(std::string f, std::string pID, std::string mID, bool urgente)
    : fecha(f), pacienteID(pID), medicoID(mID), esUrgente(urgente) {}

std::string Cita::getFecha() const {
    return fecha;
}

std::string Cita::getPacienteID() const {
    return pacienteID;
}

std::string Cita::getMedicoID() const {
    return medicoID;
}

bool Cita::getUrgencia() const {
    return esUrgente;
}

std::string Cita::toCSV() const {
    return fecha + "," + pacienteID + "," + medicoID + "," + (esUrgente ? "Si" : "No");
}
