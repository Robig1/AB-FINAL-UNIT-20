#include "GestionHospital.hpp"
#include <iostream>
#include <ctime>
#include <sstream>

GestionHospital::GestionHospital() {
    cargarDatos();
}

void GestionHospital::crearDirectorio(const std::string& nombreDirectorio) {
    #ifdef _WIN32
        mkdir(nombreDirectorio.c_str());
    #else
        mkdir(nombreDirectorio.c_str(), 0777);
    #endif
}

void GestionHospital::guardarCSV(const std::string& nombreArchivo, const std::vector<std::string>& lineas) {
    std::ofstream file(nombreArchivo);
    if (file.is_open()) {
        for (const auto& linea : lineas) {
            file << linea << std::endl;
        }
        file.close();
    } else {
        std::cout << "Error al abrir el archivo: " << nombreArchivo << std::endl;
    }
}

void GestionHospital::limpiarBuffer() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

std::string GestionHospital::leerLinea() {
    std::string linea;
    std::getline(std::cin >> std::ws, linea);
    return linea;
}

void GestionHospital::altaPaciente(const std::string& nombre, const std::string& id, const std::string& fecha, bool cronic) {
    pacientes.emplace_back(nombre, id, fecha, cronic);
    std::cout << "Paciente dado de alta exitosamente." << std::endl;
}

void GestionHospital::bajaPaciente(const std::string& id) {
    auto it = std::remove_if(pacientes.begin(), pacientes.end(),
                            [&id](const Paciente& p) { return p.getIdentificacion() == id; });
    if (it != pacientes.end()) {
        pacientes.erase(it, pacientes.end());
        std::cout << "Paciente dado de baja exitosamente." << std::endl;
    } else {
        std::cout << "No se encontró el paciente con ID: " << id << std::endl;
    }
}

void GestionHospital::modificarPaciente(const std::string& id, const std::string& nombre, const std::string& fecha) {
    for (auto& p : pacientes) {
        if (p.getIdentificacion() == id) {
            p.modificarDatos(nombre, id, fecha);
            std::cout << "Datos del paciente modificados exitosamente." << std::endl;
            return;
        }
    }
    std::cout << "No se encontró el paciente con ID: " << id << std::endl;
}

std::vector<Paciente> GestionHospital::buscarPaciente(const std::string& criterio, const std::string& valor) {
    std::vector<Paciente> resultado;
    for (const auto& p : pacientes) {
        if ((criterio == "nombre" && p.getNombre() == valor) ||
            (criterio == "id" && p.getIdentificacion() == valor) ||
            (criterio == "fecha" && p.getFechaIngreso() == valor)) {
            resultado.push_back(p);
        }
    }
    return resultado;
}

void GestionHospital::altaMedico(const std::string& nombre, const std::string& id, const std::string& fecha, const std::string& especialidad, bool disponible) {
    medicos.emplace_back(nombre, id, fecha, especialidad, disponible);
    std::cout << "Médico dado de alta exitosamente." << std::endl;
}

void GestionHospital::bajaMedico(const std::string& id) {
    auto it = std::remove_if(medicos.begin(), medicos.end(),
                            [&id](const Medico& m) { return m.getIdentificacion() == id; });
    if (it != medicos.end()) {
        medicos.erase(it, medicos.end());
        std::cout << "Médico dado de baja exitosamente." << std::endl;
    } else {
        std::cout << "No se encontró el médico con ID: " << id << std::endl;
    }
}

std::vector<Medico> GestionHospital::listarMedicosPorEspecialidad(const std::string& especialidad) {
    std::vector<Medico> resultado;
    for (const auto& m : medicos) {
        if (m.getEspecialidad() == especialidad) {
            resultado.push_back(m);
        }
    }
    return resultado;
}

std::vector<Medico> GestionHospital::listarMedicosPorDisponibilidad(bool disponible) {
    std::vector<Medico> resultado;
    for (const auto& m : medicos) {
        if (m.isDisponible() == disponible) {
            resultado.push_back(m);
        }
    }
    return resultado;
}

void GestionHospital::asignarCita(const std::string& fecha, const std::string& pacienteID, const std::string& medicoID, bool urgencia) {
    citas.emplace_back(fecha, pacienteID, medicoID, urgencia);
    std::cout << "Cita asignada exitosamente." << std::endl;
}

void GestionHospital::listarCitas(bool porUrgencia) {
    if (citas.empty()) {
        std::cout << "No hay citas programadas." << std::endl;
        return;
    }

    if (porUrgencia) {
        std::sort(citas.begin(), citas.end(), [](const Cita& a, const Cita& b) {
            return a.getUrgencia() && !b.getUrgencia();
        });
    } else {
        std::sort(citas.begin(), citas.end(), [](const Cita& a, const Cita& b) {
            return a.getFecha() < b.getFecha();
        });
    }

    for (const auto& cita : citas) {
        std::cout << "Cita - Fecha: " << cita.getFecha() << ", Paciente: " << cita.getPacienteID()
                 << ", Medico: " << cita.getMedicoID() << ", Urgente: " << (cita.getUrgencia() ? "Si" : "No") << std::endl;
    }
}

void GestionHospital::listarCitasPasadas() {
    std::time_t now = std::time(nullptr);
    std::string nowStr = std::ctime(&now);
    nowStr.pop_back();

    std::vector<Cita> citasPasadas;
    for (const auto& cita : citas) {
        if (cita.getFecha() < nowStr) {
            citasPasadas.push_back(cita);
        }
    }

    if (citasPasadas.empty()) {
        std::cout << "No hay citas pasadas." << std::endl;
        return;
    }

    for (const auto& cita : citasPasadas) {
        std::cout << "Cita - Fecha: " << cita.getFecha() << ", Paciente: " << cita.getPacienteID()
                 << ", Medico: " << cita.getMedicoID() << ", Urgente: " << (cita.getUrgencia() ? "Si" : "No") << std::endl;
    }
}

void GestionHospital::listarCitasFuturas() {
    std::time_t now = std::time(nullptr);
    std::string nowStr = std::ctime(&now);
    nowStr.pop_back();

    std::vector<Cita> citasFuturas;
    for (const auto& cita : citas) {
        if (cita.getFecha() >= nowStr) {
            citasFuturas.push_back(cita);
        }
    }

    if (citasFuturas.empty()) {
        std::cout << "No hay citas futuras." << std::endl;
        return;
    }

    for (const auto& cita : citasFuturas) {
        std::cout << "Cita - Fecha: " << cita.getFecha() << ", Paciente: " << cita.getPacienteID()
                 << ", Medico: " << cita.getMedicoID() << ", Urgente: " << (cita.getUrgencia() ? "Si" : "No") << std::endl;
    }
}

void GestionHospital::cancelarCita(const std::string& fecha, const std::string& pacienteID, const std::string& medicoID) {
    auto it = std::remove_if(citas.begin(), citas.end(),
                            [&fecha, &pacienteID, &medicoID](const Cita& c) {
                                return c.getFecha() == fecha && c.getPacienteID() == pacienteID && c.getMedicoID() == medicoID;
                            });
    if (it != citas.end()) {
        citas.erase(it, citas.end());
        std::cout << "Cita cancelada exitosamente." << std::endl;
    } else {
        std::cout << "No se encontró la cita." << std::endl;
    }
}

void GestionHospital::modificarCita(const std::string& fecha, const std::string& pacienteID, const std::string& medicoID, const std::string& nuevaFecha, bool nuevaUrgencia) {
    for (auto& cita : citas) {
        if (cita.getFecha() == fecha && cita.getPacienteID() == pacienteID && cita.getMedicoID() == medicoID) {
            cita = Cita(nuevaFecha, pacienteID, medicoID, nuevaUrgencia);
            std::cout << "Cita modificada exitosamente." << std::endl;
            return;
        }
    }
    std::cout << "No se encontró la cita." << std::endl;
}

void GestionHospital::generarReportePacientesAtendidos(const std::string& fechaInicio, const std::string& fechaFin) {
    std::vector<Paciente> pacientesAtendidos;
    for (const auto& cita : citas) {
        if (cita.getFecha() >= fechaInicio && cita.getFecha() <= fechaFin) {
            for (const auto& paciente : pacientes) {
                if (paciente.getIdentificacion() == cita.getPacienteID()) {
                    pacientesAtendidos.push_back(paciente);
                    break;
                }
            }
        }
    }

    if (pacientesAtendidos.empty()) {
        std::cout << "No hay pacientes atendidos en el rango de fechas." << std::endl;
        return;
    }

    for (const auto& paciente : pacientesAtendidos) {
        std::cout << "Paciente - Nombre: " << paciente.getNombre() << ", ID: " << paciente.getIdentificacion()
                 << ", Fecha de Ingreso: " << paciente.getFechaIngreso() << std::endl;
    }
}

void GestionHospital::generarReporteCitasPendientesPorMedico(const std::string& medicoID) {
    std::time_t now = std::time(nullptr);
    std::string nowStr = std::ctime(&now);
    nowStr.pop_back();

    std::vector<Cita> citasPendientes;
    for (const auto& cita : citas) {
        if (cita.getMedicoID() == medicoID && cita.getFecha() >= nowStr) {
            citasPendientes.push_back(cita);
        }
    }

    if (citasPendientes.empty()) {
        std::cout << "No hay citas pendientes para el médico con ID: " << medicoID << std::endl;
        return;
    }

    for (const auto& cita : citasPendientes) {
        std::cout << "Cita - Fecha: " << cita.getFecha() << ", Paciente: " << cita.getPacienteID()
                 << ", Medico: " << cita.getMedicoID() << ", Urgente: " << (cita.getUrgencia() ? "Si" : "No") << std::endl;
    }
}

void GestionHospital::generarReporteCitasPendientesPorEspecialidad(const std::string& especialidad) {
    std::time_t now = std::time(nullptr);
    std::string nowStr = std::ctime(&now);
    nowStr.pop_back();

    std::vector<Cita> citasPendientes;
    for (const auto& cita : citas) {
        for (const auto& medico : medicos) {
            if (medico.getIdentificacion() == cita.getMedicoID() && medico.getEspecialidad() == especialidad && cita.getFecha() >= nowStr) {
                citasPendientes.push_back(cita);
                break;
            }
        }
    }

    if (citasPendientes.empty()) {
        std::cout << "No hay citas pendientes para la especialidad: " << especialidad << std::endl;
        return;
    }

    for (const auto& cita : citasPendientes) {
        std::cout << "Cita - Fecha: " << cita.getFecha() << ", Paciente: " << cita.getPacienteID()
                 << ", Medico: " << cita.getMedicoID() << ", Urgente: " << (cita.getUrgencia() ? "Si" : "No") << std::endl;
    }
}

void GestionHospital::generarReportePacientesCronicos() {
    std::vector<Paciente> pacientesCronicos;
    for (const auto& paciente : pacientes) {
        if (paciente.tieneEnfermedadCronica()) {
            pacientesCronicos.push_back(paciente);
        }
    }

    if (pacientesCronicos.empty()) {
        std::cout << "No hay pacientes con enfermedades crónicas." << std::endl;
        return;
    }

    for (const auto& paciente : pacientesCronicos) {
        std::cout << "Paciente - Nombre: " << paciente.getNombre() << ", ID: " << paciente.getIdentificacion()
                 << ", Fecha de Ingreso: " << paciente.getFechaIngreso() << std::endl;
    }
}

void GestionHospital::guardarDatos() {
    std::string directorio = "datos_hospital";
    crearDirectorio(directorio);

    std::vector<std::string> pacientesCSV;
    for (const auto& p : pacientes) {
        pacientesCSV.push_back(p.toCSV());
    }
    guardarCSV(directorio + "/pacientes.csv", pacientesCSV);

    std::vector<std::string> medicosCSV;
    for (const auto& m : medicos) {
        medicosCSV.push_back(m.toCSV());
    }
    guardarCSV(directorio + "/medicos.csv", medicosCSV);

    std::vector<std::string> citasCSV;
    for (const auto& c : citas) {
        citasCSV.push_back(c.toCSV());
    }
    guardarCSV(directorio + "/citas.csv", citasCSV);

    std::cout << "Datos guardados exitosamente." << std::endl;
}

void GestionHospital::hacerBackup() {
    std::string directorio = "backup_hospital";
    crearDirectorio(directorio);

    std::vector<std::string> pacientesCSV;
    for (const auto& p : pacientes) {
        pacientesCSV.push_back(p.toCSV());
    }
    guardarCSV(directorio + "/pacientes.csv", pacientesCSV);

    std::vector<std::string> medicosCSV;
    for (const auto& m : medicos) {
        medicosCSV.push_back(m.toCSV());
    }
    guardarCSV(directorio + "/medicos.csv", medicosCSV);

    std::vector<std::string> citasCSV;
    for (const auto& c : citas) {
        citasCSV.push_back(c.toCSV());
    }
    guardarCSV(directorio + "/citas.csv", citasCSV);

    std::cout << "Backup realizado exitosamente." << std::endl;
}

void GestionHospital::cargarDatos() {
    std::ifstream pacientesFile("datos_hospital/pacientes.csv");
    if (pacientesFile.is_open()) {
        std::string line;
        while (std::getline(pacientesFile, line)) {
            std::stringstream ss(line);
            std::string nombre, id, fechaIngreso, cronicStr;
            std::getline(ss, nombre, ',');
            std::getline(ss, id, ',');
            std::getline(ss, fechaIngreso, ',');
            std::getline(ss, cronicStr, ',');
            bool cronic = (cronicStr == "Si");
            pacientes.emplace_back(nombre, id, fechaIngreso, cronic);
        }
        pacientesFile.close();
    }

    std::ifstream medicosFile("datos_hospital/medicos.csv");
    if (medicosFile.is_open()) {
        std::string line;
        while (std::getline(medicosFile, line)) {
            std::stringstream ss(line);
            std::string nombre, id, fechaIngreso, especialidad, disponibleStr;
            std::getline(ss, nombre, ',');
            std::getline(ss, id, ',');
            std::getline(ss, fechaIngreso, ',');
            std::getline(ss, especialidad, ',');
            std::getline(ss, disponibleStr, ',');
            bool disponible = (disponibleStr == "Si");
            medicos.emplace_back(nombre, id, fechaIngreso, especialidad, disponible);
        }
        medicosFile.close();
    }

    std::ifstream citasFile("datos_hospital/citas.csv");
    if (citasFile.is_open()) {
        std::string line;
        while (std::getline(citasFile, line)) {
            std::stringstream ss(line);
            std::string fecha, pacienteID, medicoID, urgenciaStr, pasadaStr;
            std::getline(ss, fecha, ',');
            std::getline(ss, pacienteID, ',');
            std::getline(ss, medicoID, ',');
            std::getline(ss, urgenciaStr, ',');
            std::getline(ss, pasadaStr, ',');
            bool urgencia = (urgenciaStr == "Si");
            bool pasada = (pasadaStr == "Si");
            Cita cita(fecha, pacienteID, medicoID, urgencia);
            cita.setPasada(pasada);
            citas.emplace_back(cita);
        }
        citasFile.close();
    }
}

void GestionHospital::menu() {
    int opcion;
    do {
        std::cout << "\nMenu de Gestion del Hospital" << std::endl;
        std::cout << "1. Alta Paciente" << std::endl;
        std::cout << "2. Baja Paciente" << std::endl;
        std::cout << "3. Modificar Paciente" << std::endl;
        std::cout << "4. Buscar Paciente" << std::endl;
        std::cout << "5. Alta Medico" << std::endl;
        std::cout << "6. Baja Medico" << std::endl;
        std::cout << "7. Listar Medicos por Especialidad" << std::endl;
        std::cout << "8. Listar Medicos por Disponibilidad" << std::endl;
        std::cout << "9. Asignar Cita" << std::endl;
        std::cout << "10. Listar Citas" << std::endl;
        std::cout << "11. Listar Citas Pasadas" << std::endl;
        std::cout << "12. Listar Citas Futuras" << std::endl;
        std::cout << "13. Cancelar Cita" << std::endl;
        std::cout << "14. Modificar Cita" << std::endl;
        std::cout << "15. Generar Reporte de Pacientes Atendidos" << std::endl;
        std::cout << "16. Generar Reporte de Citas Pendientes por Medico" << std::endl;
        std::cout << "17. Generar Reporte de Citas Pendientes por Especialidad" << std::endl;
        std::cout << "18. Generar Reporte de Pacientes Cronicos" << std::endl;
        std::cout << "19. Guardar Datos" << std::endl;
        std::cout << "20. Hacer Backup" << std::endl;
        std::cout << "21. Salir" << std::endl;
        std::cout << "Seleccione una opcion: ";
        
        if (!(std::cin >> opcion)) {
            std::cout << "Entrada inválida. Por favor, ingrese un número." << std::endl;
            limpiarBuffer();
            continue;
        }

        switch (opcion) {
            case 1: {
                std::string nombre, id, fecha;
                bool cronic;
                std::cout << "Nombre: "; nombre = leerLinea();
                std::cout << "ID: "; id = leerLinea();
                std::cout << "Fecha de Ingreso: "; fecha = leerLinea();
                std::cout << "Tiene enfermedad crónica (1: Si, 0: No): "; std::cin >> cronic;
                altaPaciente(nombre, id, fecha, cronic);
                break;
            }
            case 2: {
                std::string id;
                std::cout << "ID del Paciente: "; id = leerLinea();
                bajaPaciente(id);
                break;
            }
            case 3: {
                std::string id, nombre, fecha;
                std::cout << "ID del Paciente: "; id = leerLinea();
                std::cout << "Nuevo Nombre: "; nombre = leerLinea();
                std::cout << "Nueva Fecha de Ingreso: "; fecha = leerLinea();
                modificarPaciente(id, nombre, fecha);
                break;
            }
            case 4: {
                std::string criterio, valor;
                std::cout << "Criterio (nombre/id/fecha): "; criterio = leerLinea();
                std::cout << "Valor: "; valor = leerLinea();
                std::vector<Paciente> resultado = buscarPaciente(criterio, valor);
                if (resultado.empty()) {
                    std::cout << "No se encontraron pacientes con el criterio especificado." << std::endl;
                } else {
                    for (const auto& p : resultado) {
                        std::cout << "Paciente - Nombre: " << p.getNombre() << ", ID: " << p.getIdentificacion()
                                 << ", Fecha de Ingreso: " << p.getFechaIngreso() << std::endl;
                    }
                }
                break;
            }
            case 5: {
                std::string nombre, id, fecha, especialidad;
                bool disponible;
                std::cout << "Nombre: "; nombre = leerLinea();
                std::cout << "ID: "; id = leerLinea();
                std::cout << "Fecha de Ingreso: "; fecha = leerLinea();
                std::cout << "Especialidad: "; especialidad = leerLinea();
                std::cout << "Disponible (1: Si, 0: No): "; std::cin >> disponible;
                altaMedico(nombre, id, fecha, especialidad, disponible);
                break;
            }
            case 6: {
                std::string id;
                std::cout << "ID del Medico: "; id = leerLinea();
                bajaMedico(id);
                break;
            }
            case 7: {
                std::string especialidad;
                std::cout << "Especialidad: "; especialidad = leerLinea();
                std::vector<Medico> resultado = listarMedicosPorEspecialidad(especialidad);
                if (resultado.empty()) {
                    std::cout << "No se encontraron médicos con la especialidad especificada." << std::endl;
                } else {
                    for (const auto& m : resultado) {
                        std::cout << "Medico - Nombre: " << m.getNombre() << ", ID: " << m.getIdentificacion()
                                 << ", Fecha de Ingreso: " << m.getFechaIngreso() << ", Especialidad: " << m.getEspecialidad()
                                 << ", Disponible: " << (m.isDisponible() ? "Si" : "No") << std::endl;
                    }
                }
                break;
            }
            case 8: {
                bool disponible;
                std::cout << "Disponible (1: Si, 0: No): "; std::cin >> disponible;
                std::vector<Medico> resultado = listarMedicosPorDisponibilidad(disponible);
                if (resultado.empty()) {
                    std::cout << "No se encontraron médicos con la disponibilidad especificada." << std::endl;
                } else {
                    for (const auto& m : resultado) {
                        std::cout << "Medico - Nombre: " << m.getNombre() << ", ID: " << m.getIdentificacion()
                                 << ", Fecha de Ingreso: " << m.getFechaIngreso() << ", Especialidad: " << m.getEspecialidad()
                                 << ", Disponible: " << (m.isDisponible() ? "Si" : "No") << std::endl;
                    }
                }
                break;
            }
            case 9: {
                std::string fecha, pacienteID, medicoID;
                bool urgencia;
                std::cout << "Fecha: "; fecha = leerLinea();
                std::cout << "ID del Paciente: "; pacienteID = leerLinea();
                std::cout << "ID del Medico: "; medicoID = leerLinea();
                std::cout << "Urgente (1: Si, 0: No): "; std::cin >> urgencia;
                asignarCita(fecha, pacienteID, medicoID, urgencia);
                break;
            }
            case 10: {
                bool porUrgencia;
                std::cout << "Ordenar por urgencia (1: Si, 0: No): "; std::cin >> porUrgencia;
                listarCitas(porUrgencia);
                break;
            }
            case 11: {
                listarCitasPasadas();
                break;
            }
            case 12: {
                listarCitasFuturas();
                break;
            }
            case 13: {
                std::string fecha, pacienteID, medicoID;
                std::cout << "Fecha: "; fecha = leerLinea();
                std::cout << "ID del Paciente: "; pacienteID = leerLinea();
                std::cout << "ID del Medico: "; medicoID = leerLinea();
                cancelarCita(fecha, pacienteID, medicoID);
                break;
            }
            case 14: {
                std::string fecha, pacienteID, medicoID, nuevaFecha;
                bool nuevaUrgencia;
                std::cout << "Fecha Actual: "; fecha = leerLinea();
                std::cout << "ID del Paciente: "; pacienteID = leerLinea();
                std::cout << "ID del Medico: "; medicoID = leerLinea();
                std::cout << "Nueva Fecha: "; nuevaFecha = leerLinea();
                std::cout << "Nueva Urgencia (1: Si, 0: No): "; std::cin >> nuevaUrgencia;
                modificarCita(fecha, pacienteID, medicoID, nuevaFecha, nuevaUrgencia);
                break;
            }
            case 15: {
                std::string fechaInicio, fechaFin;
                std::cout << "Fecha Inicio: "; fechaInicio = leerLinea();
                std::cout << "Fecha Fin: "; fechaFin = leerLinea();
                generarReportePacientesAtendidos(fechaInicio, fechaFin);
                break;
            }
            case 16: {
                std::string medicoID;
                std::cout << "ID del Medico: "; medicoID = leerLinea();
                generarReporteCitasPendientesPorMedico(medicoID);
                break;
            }
            case 17: {
                std::string especialidad;
                std::cout << "Especialidad: "; especialidad = leerLinea();
                generarReporteCitasPendientesPorEspecialidad(especialidad);
                break;
            }
            case 18: {
                generarReportePacientesCronicos();
                break;
            }
            case 19: {
                guardarDatos();
                break;
            }
            case 20: {
                hacerBackup();
                break;
            }
            case 21: {
                std::cout << "Saliendo del programa." << std::endl;
                break;
            }
            default:
                std::cout << "Opcion invalida. Por favor, intente de nuevo." << std::endl;
        }
        limpiarBuffer();
    } while (opcion != 21);
}

