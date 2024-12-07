#ifndef SISTEMA_GESTION_CLINICA_HPP
#define SISTEMA_GESTION_CLINICA_HPP

#include "paciente.hpp"
#include "medico.hpp"
#include "cita.hpp"
#include "fecha.hpp"
#include <map>
#include <memory>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include <algorithm>
#include <iostream>

class SistemaGestionClinica {
private:
    std::map<std::string, std::unique_ptr<Paciente>> pacientes;
    std::map<std::string, std::unique_ptr<Medico>> medicos;
    std::map<std::string, std::unique_ptr<Cita>> citas;
    std::string rutaArchivos;

public:
    SistemaGestionClinica(const std::string& ruta = "./datos_hospital/");
    ~SistemaGestionClinica();

    bool agregarPaciente(std::unique_ptr<Paciente> paciente);
    Paciente* buscarPaciente(const std::string& id);

    bool agregarMedico(std::unique_ptr<Medico> medico);
    Medico* buscarMedico(const std::string& id);

    std::vector<Medico*> buscarMedicosPorEspecialidad(Especialidad esp);

    std::string generarIdCita();
    bool agendarCita(std::string idPaciente, std::string idMedico, const Fecha& fecha, 
                     const std::string& hora, const std::string& motivo, bool urgente = false);

    bool cancelarCita(const std::string& idCita);
    std::vector<Cita*> buscarCitasPaciente(const std::string& idPaciente);

    void guardarDatos();
    void cargarDatos();

    std::map<Especialidad, int> obtenerEstadisticasEspecialidades();
    std::vector<std::pair<std::string, int>> obtenerMedicosMasOcupados(const Fecha& inicio, const Fecha& fin);
    void mostrarEstadisticas() const;
};

#endif // SISTEMA_GESTION_CLINICA_HPP

