#include "GestionHospital.hpp"
#include <iostream>

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

void GestionHospital::altaPaciente(const std::string& nombre, const std::string& id, const std::string& fecha) {
    pacientes.emplace_back(nombre, id, fecha);
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

void GestionHospital::menu() {
    int opcion;
    do {
        std::cout << "\nMenu de Gestion del Hospital" << std::endl;
        std::cout << "1. Alta Paciente" << std::endl;
        std::cout << "2. Baja Paciente" << std::endl;
        std::cout << "3. Alta Medico" << std::endl;
        std::cout << "4. Baja Medico" << std::endl;
        std::cout << "5. Asignar Cita" << std::endl;
        std::cout << "6. Listar Citas" << std::endl;
        std::cout << "7. Guardar Datos" << std::endl;
        std::cout << "8. Salir" << std::endl;
        std::cout << "Seleccione una opcion: ";
        
        if (!(std::cin >> opcion)) {
            std::cout << "Entrada inválida. Por favor, ingrese un número." << std::endl;
            limpiarBuffer();
            continue;
        }

        switch (opcion) {
            case 1: {
                std::string nombre, id, fecha;
                std::cout << "Nombre: "; nombre = leerLinea();
                std::cout << "ID: "; id = leerLinea();
                std::cout << "Fecha de Ingreso: "; fecha = leerLinea();
                altaPaciente(nombre, id, fecha);
                break;
            }
            case 2: {
                std::string id;
                std::cout << "ID del Paciente: "; id = leerLinea();
                bajaPaciente(id);
                break;
            }
            case 3: {
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
            case 4: {
                std::string id;
                std::cout << "ID del Medico: "; id = leerLinea();
                bajaMedico(id);
                break;
            }
            case 5: {
                std::string fecha, pacienteID, medicoID;
                bool urgencia;
                std::cout << "Fecha: "; fecha = leerLinea();
                std::cout << "ID del Paciente: "; pacienteID = leerLinea();
                std::cout << "ID del Medico: "; medicoID = leerLinea();
                std::cout << "Es urgente (1: Si, 0: No): "; std::cin >> urgencia;
                asignarCita(fecha, pacienteID, medicoID, urgencia);
                break;
            }
            case 6: {
                char ordenUrgencia;
                std::cout << "¿Ordenar por urgencia? (S/N): "; std::cin >> ordenUrgencia;
                listarCitas(ordenUrgencia == 'S' || ordenUrgencia == 's');
                break;
            }
            case 7: {
                guardarDatos();
                break;
            }
            case 8: {
                std::cout << "Saliendo del programa..." << std::endl;
                break;
            }
            default: {
                std::cout << "Opción no válida. Por favor, intente nuevamente." << std::endl;
                break;
            }
        }
    } while (opcion != 8);
}

