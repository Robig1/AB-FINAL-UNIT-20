#include "sistema_gestion_clinica.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>

SistemaGestionClinica::SistemaGestionClinica(const std::string& ruta) : rutaArchivos(ruta) {
    cargarDatos();
}

SistemaGestionClinica::~SistemaGestionClinica() {
    guardarDatos();
}

bool SistemaGestionClinica::agregarPaciente(std::unique_ptr<Paciente> paciente) {
    std::string id = paciente->getID();
    if (pacientes.find(id) != pacientes.end()) return false;
    pacientes[id] = std::move(paciente);
    return true;
}

Paciente* SistemaGestionClinica::buscarPaciente(const std::string& id) {
    auto it = pacientes.find(id);
    return it != pacientes.end() ? it->second.get() : nullptr;
}

bool SistemaGestionClinica::agregarMedico(std::unique_ptr<Medico> medico) {
    std::string id = medico->getID();
    if (medicos.find(id) != medicos.end()) return false;
    medicos[id] = std::move(medico);
    return true;
}

Medico* SistemaGestionClinica::buscarMedico(const std::string& id) {
    auto it = medicos.find(id);
    return it != medicos.end() ? it->second.get() : nullptr;
}

std::vector<Medico*> SistemaGestionClinica::buscarMedicosPorEspecialidad(Especialidad esp) {
    std::vector<Medico*> resultado;
    for (const auto& [_, medico] : medicos) {
        if (medico->getEspecialidad() == esp && medico->isDisponible()) {
            resultado.push_back(medico.get());
        }
    }
    return resultado;
}

std::string SistemaGestionClinica::generarIdCita() {
    return "CIT" + std::to_string(citas.size() + 1);
}

bool SistemaGestionClinica::agendarCita(std::string idPaciente, std::string idMedico, const Fecha& fecha, 
                                         const std::string& hora, const std::string& motivo, bool urgente) {

    auto paciente = buscarPaciente(idPaciente);
    auto medico = buscarMedico(idMedico);
    if (!paciente || !medico) return false;

    if (!medico->tieneDisponibilidad(fecha)) return false;

    std::string idCita = generarIdCita();
    auto cita = std::make_unique<Cita>(idCita, idPaciente, idMedico, fecha, hora, motivo, urgente);
    medico->agregarCita(fecha, idCita);
    citas[idCita] = std::move(cita);
    return true;
}

bool SistemaGestionClinica::cancelarCita(const std::string& idCita) {
    auto it = citas.find(idCita);
    if (it == citas.end()) return false;

    auto& cita = it->second;
    if (cita->getEstado() == "Cancelada") return false;

    auto medico = buscarMedico(cita->getIdMedico());
    if (medico) {
        medico->eliminarCita(cita->getFecha(), idCita);
    }

    cita->setEstado("Cancelada");
    return true;
}

std::vector<Cita*> SistemaGestionClinica::buscarCitasPaciente(const std::string& idPaciente) {
    std::vector<Cita*> citasPaciente;
    for (const auto& [_, cita] : citas) {
        if (cita->getIdPaciente() == idPaciente) {
            citasPaciente.push_back(cita.get());
        }
    }
    return citasPaciente;
}

void SistemaGestionClinica::guardarDatos() {
    std::ofstream archivoPacientes(rutaArchivos + "pacientes.csv");
    for (const auto& [_, paciente] : pacientes) {
        archivoPacientes << paciente->toCSV() << "\n";
    }

    std::ofstream archivoMedicos(rutaArchivos + "medicos.csv");
    for (const auto& [_, medico] : medicos) {
        archivoMedicos << medico->toCSV() << "\n";
    }

    std::ofstream archivoCitas(rutaArchivos + "citas.csv");
    for (const auto& [_, cita] : citas) {
        archivoCitas << cita->toCSV() << "\n";
    }
}

void SistemaGestionClinica::cargarDatos() {
    std::ifstream archivoPacientes(rutaArchivos + "pacientes.csv");
    std::string linea;
    while (getline(archivoPacientes, linea)) {
        auto paciente = std::make_unique<Paciente>("", "", Fecha()); 
        paciente->fromCSV(linea);
        pacientes[paciente->getID()] = std::move(paciente);
    }

    std::ifstream archivoMedicos(rutaArchivos + "medicos.csv");
    while (getline(archivoMedicos, linea)) {
        auto medico = std::make_unique<Medico>("", "", Especialidad::GENERAL);
        medico->fromCSV(linea);
        medicos[medico->getID()] = std::move(medico);
    }

    std::ifstream archivoCitas(rutaArchivos + "citas.csv");
    while (getline(archivoCitas, linea)) {
        auto cita = std::make_unique<Cita>("", "", "", Fecha(), "", "");
        cita->fromCSV(linea);
        citas[cita->getID()] = std::move(cita);
    }
}

std::map<Especialidad, int> SistemaGestionClinica::obtenerEstadisticasEspecialidades() {
    std::map<Especialidad, int> estadisticas;
    for (const auto& [_, cita] : citas) {
        if (cita->getEstado() == "Completada") {
            auto medico = buscarMedico(cita->getIdMedico());
            if (medico) {
                estadisticas[medico->getEspecialidad()]++;
            }
        }
    }
    return estadisticas;
}

std::vector<std::pair<std::string, int>> SistemaGestionClinica::obtenerMedicosMasOcupados(const Fecha& inicio, const Fecha& fin) {
    std::map<std::string, int> conteo;
    for (const auto& [_, cita] : citas) {
        if (cita->getFecha().estaEnRango(inicio, fin)) {
            conteo[cita->getIdMedico()]++;
        }
    }

    std::vector<std::pair<std::string, int>> resultado(conteo.begin(), conteo.end());
    std::sort(resultado.begin(), resultado.end(), [](const auto& a, const auto& b) { return a.second > b.second; });
    
    return resultado;
}

void SistemaGestionClinica::mostrarEstadisticas() const {
    std::cout << "\n=== Estadisticas del Sistema ===\n"
              << "Total de pacientes: " << pacientes.size() << "\n"
              << "Total de medicos: " << medicos.size() << "\n"
              << "Total de citas: " << citas.size() << "\n\n";

    std::map<Especialidad, int> distEsp;
    for (const auto& [_, medico] : medicos) {
        distEsp[medico->getEspecialidad()]++;
    }

    std::cout << "Distribucion de especialidades:\n";
    for (const auto& [esp, cant] : distEsp) {
        std::cout << especialidadToString(esp) << ": " << cant << " medicos\n";
    }
}

