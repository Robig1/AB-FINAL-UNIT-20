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

class GestionHospital {
private:
    std::vector<Paciente> pacientes;
    std::vector<Medico> medicos;
    std::vector<Cita> citas;

    void crearDirectorio(const std::string& nombreDirectorio);
    void guardarCSV(const std::string& nombreArchivo, const std::vector<std::string>& lineas);
    void limpiarBuffer();
    std::string leerLinea();

public:
    void altaPaciente(const std::string& nombre, const std::string& id, const std::string& fecha);
    void bajaPaciente(const std::string& id);
    void altaMedico(const std::string& nombre, const std::string& id, const std::string& fecha, const std::string& especialidad, bool disponible);
    void bajaMedico(const std::string& id);
    std::vector<Paciente> buscarPaciente(const std::string& criterio, const std::string& valor);
    void asignarCita(const std::string& fecha, const std::string& pacienteID, const std::string& medicoID, bool urgencia);
    void listarCitas(bool porUrgencia = false);
    void guardarDatos();
    void menu();
};

#endif // GESTIONHOSPITAL_HPP

