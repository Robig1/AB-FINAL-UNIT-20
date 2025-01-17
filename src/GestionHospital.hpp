#ifndef GESTIONHOSPITAL_HPP
#define GESTIONHOSPITAL_HPP

#include "Paciente.hpp"
#include "Medico.hpp"
#include "Cita.hpp"
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>
#include <limits>
#include <sys/stat.h>
#include <sys/types.h>
#include <ctime>

class GestionHospital {
private:
    std::vector<Paciente> pacientes;
    std::vector<Medico> medicos;
    std::vector<Cita> citas;

    void crearDirectorio(const std::string& nombreDirectorio);
    void guardarCSV(const std::string& nombreArchivo, const std::vector<std::string>& lineas);
    void limpiarBuffer();
    std::string leerLinea();
    void cargarDatos();

public:
    GestionHospital();
    void altaPaciente(const std::string& nombre, const std::string& id, const std::string& fecha, bool cronic = false);
    void bajaPaciente(const std::string& id);
    void modificarPaciente(const std::string& id, const std::string& nombre, const std::string& fecha);
    std::vector<Paciente> buscarPaciente(const std::string& criterio, const std::string& valor);
    void altaMedico(const std::string& nombre, const std::string& id, const std::string& fecha, const std::string& especialidad, bool disponible);
    void bajaMedico(const std::string& id);
    std::vector<Medico> listarMedicosPorEspecialidad(const std::string& especialidad);
    std::vector<Medico> listarMedicosPorDisponibilidad(bool disponible);
    void asignarCita(const std::string& fecha, const std::string& pacienteID, const std::string& medicoID, bool urgencia);
    void listarCitas(bool porUrgencia = false);
    void listarCitasPasadas();
    void listarCitasFuturas();
    void cancelarCita(const std::string& fecha, const std::string& pacienteID, const std::string& medicoID);
    void modificarCita(const std::string& fecha, const std::string& pacienteID, const std::string& medicoID, const std::string& nuevaFecha, bool nuevaUrgencia);
    void generarReportePacientesAtendidos(const std::string& fechaInicio, const std::string& fechaFin);
    void generarReporteCitasPendientesPorMedico(const std::string& medicoID);
    void generarReporteCitasPendientesPorEspecialidad(const std::string& especialidad);
    void generarReportePacientesCronicos();
    void guardarDatos();
    void hacerBackup();
    void menu();
};

#endif // GESTIONHOSPITAL_HPP

