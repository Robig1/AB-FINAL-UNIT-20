#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>
#include <map>
#include <ctime>
#include <sys/stat.h>
#include <sys/types.h>
#include <limits>

using namespace std;

class Persona {
protected:
    string nombre;
    string identificacion;
    string fechaIngreso;
public:
    Persona(string n, string id, string fecha) : nombre(n), identificacion(id), fechaIngreso(fecha) {}
    virtual ~Persona() {}
    string getNombre() const { return nombre; }
    string getIdentificacion() const { return identificacion; }
    string getFechaIngreso() const { return fechaIngreso; }
    void modificarDatos(string n, string id, string fecha) {
        nombre = n;
        identificacion = id;
        fechaIngreso = fecha;
    }
};

class Paciente : public Persona {
private:
    vector<string> historialClinico;
public:
    Paciente(string n, string id, string fecha) : Persona(n, id, fecha) {}
    void registrarHistorial(string historial) {
        historialClinico.push_back(historial);
    }
    vector<string> getHistorialClinico() const {
        return historialClinico;
    }
    string toCSV() const {
        return nombre + "," + identificacion + "," + fechaIngreso;
    }
};

class Medico : public Persona {
private:
    string especialidad;
    bool disponible;
public:
    Medico(string n, string id, string fecha, string esp, bool disp = true)
        : Persona(n, id, fecha), especialidad(esp), disponible(disp) {}
    string getEspecialidad() const { return especialidad; }
    bool isDisponible() const { return disponible; }
    void setDisponibilidad(bool disp) { disponible = disp; }
    string toCSV() const {
        return nombre + "," + identificacion + "," + fechaIngreso + "," + especialidad + "," + (disponible ? "Si" : "No");
    }
};

class Cita {
private:
    string fecha;
    string pacienteID;
    string medicoID;
    bool esUrgente;
public:
    Cita(string f, string pID, string mID, bool urgente)
        : fecha(f), pacienteID(pID), medicoID(mID), esUrgente(urgente) {}
    string getFecha() const { return fecha; }
    string getPacienteID() const { return pacienteID; }
    string getMedicoID() const { return medicoID; }
    bool getUrgencia() const { return esUrgente; }
    string toCSV() const {
        return fecha + "," + pacienteID + "," + medicoID + "," + (esUrgente ? "Si" : "No");
    }
};

class GestionHospital {
private:
    vector<Paciente> pacientes;
    vector<Medico> medicos;
    vector<Cita> citas;

    void crearDirectorio(const string& nombreDirectorio) {
        #ifdef _WIN32
            mkdir(nombreDirectorio.c_str());
        #else
            mkdir(nombreDirectorio.c_str(), 0777);
        #endif
    }

    void guardarCSV(const string& nombreArchivo, const vector<string>& lineas) {
        ofstream file(nombreArchivo);
        if (file.is_open()) {
            for (const auto& linea : lineas) {
                file << linea << endl;
            }
            file.close();
        } else {
            cout << "Error al abrir el archivo: " << nombreArchivo << endl;
        }
    }

    void limpiarBuffer() {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    string leerLinea() {
        string linea;
        getline(cin >> ws, linea);
        return linea;
    }

public:
    void altaPaciente(const string& nombre, const string& id, const string& fecha) {
        pacientes.emplace_back(nombre, id, fecha);
        cout << "Paciente dado de alta exitosamente." << endl;
    }

    void bajaPaciente(const string& id) {
        auto it = remove_if(pacientes.begin(), pacientes.end(),
                            [&id](const Paciente& p) { return p.getIdentificacion() == id; });
        if (it != pacientes.end()) {
            pacientes.erase(it, pacientes.end());
            cout << "Paciente dado de baja exitosamente." << endl;
        } else {
            cout << "No se encontró el paciente con ID: " << id << endl;
        }
    }

    void altaMedico(const string& nombre, const string& id, const string& fecha, const string& especialidad, bool disponible) {
        medicos.emplace_back(nombre, id, fecha, especialidad, disponible);
        cout << "Médico dado de alta exitosamente." << endl;
    }

    void bajaMedico(const string& id) {
        auto it = remove_if(medicos.begin(), medicos.end(),
                            [&id](const Medico& m) { return m.getIdentificacion() == id; });
        if (it != medicos.end()) {
            medicos.erase(it, medicos.end());
            cout << "Médico dado de baja exitosamente." << endl;
        } else {
            cout << "No se encontró el médico con ID: " << id << endl;
        }
    }

    vector<Paciente> buscarPaciente(const string& criterio, const string& valor) {
        vector<Paciente> resultado;
        for (const auto& p : pacientes) {
            if ((criterio == "nombre" && p.getNombre() == valor) ||
                (criterio == "id" && p.getIdentificacion() == valor) ||
                (criterio == "fecha" && p.getFechaIngreso() == valor)) {
                resultado.push_back(p);
            }
        }
        return resultado;
    }

    void asignarCita(const string& fecha, const string& pacienteID, const string& medicoID, bool urgencia) {
        citas.emplace_back(fecha, pacienteID, medicoID, urgencia);
        cout << "Cita asignada exitosamente." << endl;
    }

    void listarCitas(bool porUrgencia = false) {
        if (citas.empty()) {
            cout << "No hay citas programadas." << endl;
            return;
        }

        if (porUrgencia) {
            sort(citas.begin(), citas.end(), [](const Cita& a, const Cita& b) {
                return a.getUrgencia() && !b.getUrgencia();
            });
        } else {
            sort(citas.begin(), citas.end(), [](const Cita& a, const Cita& b) {
                return a.getFecha() < b.getFecha();
            });
        }

        for (const auto& cita : citas) {
            cout << "Cita - Fecha: " << cita.getFecha() << ", Paciente: " << cita.getPacienteID()
                 << ", Medico: " << cita.getMedicoID() << ", Urgente: " << (cita.getUrgencia() ? "Si" : "No") << endl;
        }
    }

    void guardarDatos() {
        string directorio = "datos_hospital";
        crearDirectorio(directorio);

        vector<string> pacientesCSV;
        for (const auto& p : pacientes) {
            pacientesCSV.push_back(p.toCSV());
        }
        guardarCSV(directorio + "/pacientes.csv", pacientesCSV);

        vector<string> medicosCSV;
        for (const auto& m : medicos) {
            medicosCSV.push_back(m.toCSV());
        }
        guardarCSV(directorio + "/medicos.csv", medicosCSV);

        vector<string> citasCSV;
        for (const auto& c : citas) {
            citasCSV.push_back(c.toCSV());
        }
        guardarCSV(directorio + "/citas.csv", citasCSV);

        cout << "Datos guardados exitosamente." << endl;
    }

    void menu() {
        int opcion;
        do {
            cout << "\nMenu de Gestion del Hospital" << endl;
            cout << "1. Alta Paciente" << endl;
            cout << "2. Baja Paciente" << endl;
            cout << "3. Alta Medico" << endl;
            cout << "4. Baja Medico" << endl;
            cout << "5. Asignar Cita" << endl;
            cout << "6. Listar Citas" << endl;
            cout << "7. Guardar Datos" << endl;
            cout << "8. Salir" << endl;
            cout << "Seleccione una opcion: ";
            
            if (!(cin >> opcion)) {
                cout << "Entrada inválida. Por favor, ingrese un número." << endl;
                limpiarBuffer();
                continue;
            }

            switch (opcion) {
                case 1: {
                    string nombre, id, fecha;
                    cout << "Nombre: "; nombre = leerLinea();
                    cout << "ID: "; id = leerLinea();
                    cout << "Fecha de Ingreso: "; fecha = leerLinea();
                    altaPaciente(nombre, id, fecha);
                    break;
                }
                case 2: {
                    string id;
                    cout << "ID del Paciente: "; id = leerLinea();
                    bajaPaciente(id);
                    break;
                }
                case 3: {
                    string nombre, id, fecha, especialidad;
                    bool disponible;
                    cout << "Nombre: "; nombre = leerLinea();
                    cout << "ID: "; id = leerLinea();
                    cout << "Fecha de Ingreso: "; fecha = leerLinea();
                    cout << "Especialidad: "; especialidad = leerLinea();
                    cout << "Disponible (1: Si, 0: No): "; cin >> disponible;
                    altaMedico(nombre, id, fecha, especialidad, disponible);
                    break;
                }
                case 4: {
                    string id;
                    cout << "ID del Medico: "; id = leerLinea();
                    bajaMedico(id);
                    break;
                }
                case 5: {
                    string fecha, pacienteID, medicoID;
                    bool urgencia;
                    cout << "Fecha: "; fecha = leerLinea();
                    cout << "ID del Paciente: "; pacienteID = leerLinea();
                    cout << "ID del Medico: "; medicoID = leerLinea();
                    cout << "Es urgente (1: Si, 0: No): "; cin >> urgencia;
                    asignarCita(fecha, pacienteID, medicoID, urgencia);
                    break;
                }
                case 6: {
                    char ordenUrgencia;
                    cout << "¿Ordenar por urgencia? (S/N): "; cin >> ordenUrgencia;
                    listarCitas(ordenUrgencia == 'S' || ordenUrgencia == 's');
                    break;
                }
                case 7: {
                    guardarDatos();
                    break;
                }
                case 8: {
                    cout << "Saliendo del programa..." << endl;
                    break;
                }
                default: {
                    cout << "Opción no válida. Por favor, intente nuevamente." << endl;
                    break;
                }
            }
        } while (opcion != 8);
    }
};

int main() {
    GestionHospital hospital;
    hospital.menu();
    return 0;
}

