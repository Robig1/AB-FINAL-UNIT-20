#include "cita.hpp"
#include <iostream>

Cita::Cita(std::string i, std::string ip, std::string im, const Fecha& f, std::string h, std::string m, bool u)
    : id(i), idPaciente(ip), idMedico(im), fecha(f), hora(h), motivo(m), urgente(u), estado("Pendiente") {}

std::string Cita::getID() const { return id; }
std::string Cita::getIdPaciente() const { return idPaciente; }
std::string Cita::getIdMedico() const { return idMedico; }
Fecha Cita::getFecha() const { return fecha; }
std::string Cita::getHora() const { return hora; }
std::string Cita::getMotivo() const { return motivo; }
bool Cita::isUrgente() const { return urgente; }
std::string Cita::getEstado() const { return estado; }

void Cita::setFecha(const Fecha& f) { fecha = f; }
void Cita::setHora(std::string h) { hora = h; }
void Cita::setMotivo(std::string m) { motivo = m; }
void Cita::setUrgente(bool u) { urgente = u; }
void Cita::setEstado(std::string e) { estado = e; }

std::string Cita::toCSV() const {
    return id + "," + idPaciente + "," + idMedico + "," + fecha.toString() + "," +
           hora + "," + motivo + "," + std::to_string(urgente) + "," + estado;
}

void Cita::fromCSV(const std::string& csv) {
    std::stringstream ss(csv);
    std::string token;
    std::vector<std::string> tokens;
    
    while (getline(ss, token, ',')) {
        tokens.push_back(token);
    }

    if (tokens.size() >= 8) {
        id = tokens[0];
        idPaciente = tokens[1];
        idMedico = tokens[2];
        fecha = Fecha::fromString(tokens[3]);
        hora = tokens[4];
        motivo = tokens[5];
        urgente = (tokens[6] == "1");
        estado = tokens[7];
    }
}

void Cita::mostrarInfo() const {
    std::cout << "Cita ID: " << id << "\n"
              << "Paciente ID: " << idPaciente << "\n"
              << "Medico ID: " << idMedico << "\n"
              << "Fecha: " << fecha.toString() << " " << hora << "\n"
              << "Motivo: " << motivo << "\n"
              << "Urgente: " << (urgente ? "Sí" : "No") << "\n"
              << "Estado: " << estado << "\n";
}

