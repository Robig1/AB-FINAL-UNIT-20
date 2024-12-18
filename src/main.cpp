#include <iostream>
#include <filesystem>
#include "medico.hpp"
#include "paciente.hpp"
#include "fecha.hpp"
#include "sistema_gestion_clinica.hpp"

int main() {
    // Crear el directorio para almacenar datos
    std::filesystem::create_directories("datos_hospital");

    // Crear instancia del sistema de gestion clinica
    SistemaGestionClinica sistema;

    // Crear personal medico inicial
    auto medico1 = std::make_unique<Medico>("Dr. Juan Perez", "M001", Especialidad::CARDIOLOGIA);
    auto medico2 = std::make_unique<Medico>("Dra. Ana Garcia", "M002", Especialidad::PEDIATRIA);

    // Crear pacientes iniciales
    auto paciente1 = std::make_unique<Paciente>("Carlos Lopez", "P001", Fecha(1, 1, 2024));
    auto paciente2 = std::make_unique<Paciente>("Maria Rodriguez", "P002", Fecha(2, 1, 2024));

    // Agregar medicos al sistema
    sistema.agregarMedico(std::move(medico1));
    sistema.agregarMedico(std::move(medico2));

    // Agregar pacientes al sistema
    sistema.agregarPaciente(std::move(paciente1));
    sistema.agregarPaciente(std::move(paciente2));

    // Agendar citas medicas
    sistema.agendarCita("P001", "M001", Fecha(25, 11, 2024), "10:00", "Consulta rutinaria");
    sistema.agendarCita("P002", "M002", Fecha(26, 11, 2024), "11:00", "Control pediatrico");

    // Mostrar estadisticas del sistema
    sistema.mostrarEstadisticas();

    return 0;
}
