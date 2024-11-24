#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <map>
#include <set>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <limits>
#include <memory>
#include <cstring>
#include <filesystem>

using namespace std;

class Fecha {
    int dia, mes, anio;

public:
    Fecha() : dia(1), mes(1), anio(2024) {}
    Fecha(int d, int m, int a) : dia(d), mes(m), anio(a) {}
    
    static Fecha hoy() {
        auto now = chrono::system_clock::now();
        time_t tt = chrono::system_clock::to_time_t(now);
        tm local_tm;
        #ifdef _WIN32
            localtime_s(&local_tm, &tt);
        #else
            localtime_r(&tt, &local_tm);
        #endif
        
        return Fecha(local_tm.tm_mday, local_tm.tm_mon + 1, local_tm.tm_year + 1900);
    }

    static Fecha fromString(const string& fecha) {
        istringstream ss(fecha);
        string token;
        vector<int> componentes;
        
        while (getline(ss, token, '-')) {
            componentes.push_back(stoi(token));
        }
        
        if (componentes.size() != 3) return Fecha();
        return Fecha(componentes[2], componentes[1], componentes[0]);
    }

    string toString() const {
        stringstream ss;
        ss << setfill('0') << setw(4) << anio << "-"
           << setfill('0') << setw(2) << mes << "-"
           << setfill('0') << setw(2) << dia;
        return ss.str();
    }

    bool operator<(const Fecha& otra) const {
        if (anio != otra.anio) return anio < otra.anio;
        if (mes != otra.mes) return mes < otra.mes;
        return dia < otra.dia;
    }

    bool operator==(const Fecha& otra) const {
        return dia == otra.dia && mes == otra.mes && anio == otra.anio;
    }

    bool estaEnRango(const Fecha& inicio, const Fecha& fin) const {
        return !(*this < inicio) && !(*this > fin);
    }

    bool operator>(const Fecha& otra) const {
        return otra < *this;
    }
};

enum class Especialidad {
    CARDIOLOGIA,
    DERMATOLOGIA,
    PEDIATRIA,
    TRAUMATOLOGIA,
    NEUROLOGIA,
    GENERAL
};

string especialidadToString(Especialidad esp) {
    switch(esp) {
        case Especialidad::CARDIOLOGIA: return "Cardiologia";
        case Especialidad::DERMATOLOGIA: return "Dermatologia";
        case Especialidad::PEDIATRIA: return "Pediatria";
        case Especialidad::TRAUMATOLOGIA: return "Traumatologia";
        case Especialidad::NEUROLOGIA: return "Neurologia";
        case Especialidad::GENERAL: return "Medicina General";
        default: return "Desconocida";
    }
}

Especialidad stringToEspecialidad(const string& esp) {
    if (esp == "Cardiologia") return Especialidad::CARDIOLOGIA;
    if (esp == "Dermatologia") return Especialidad::DERMATOLOGIA;
    if (esp == "Pediatria") return Especialidad::PEDIATRIA;
    if (esp == "Traumatologia") return Especialidad::TRAUMATOLOGIA;
    if (esp == "Neurologia") return Especialidad::NEUROLOGIA;
    return Especialidad::GENERAL;
}

struct HistorialClinico {
    Fecha fecha;
    string descripcion;
    string diagnostico;
    string tratamiento;
    bool esCronico;

    string toString() const {
        return fecha.toString() + " - " + descripcion + " - " + diagnostico + 
               " - " + tratamiento + " - " + (esCronico ? "Cronico" : "No cronico");
    }
};

class Persona {
protected:
    string nombre;
    string id;
    string telefono;
    string email;

public:
    Persona(string n, string i, string t = "", string e = "")
        : nombre(n), id(i), telefono(t), email(e) {}
    virtual ~Persona() = default;

    string getNombre() const { return nombre; }
    string getID() const { return id; }
    string getTelefono() const { return telefono; }
    string getEmail() const { return email; }

    void setNombre(string n) { nombre = n; }
    void setID(string i) { id = i; }
    void setTelefono(string t) { telefono = t; }
    void setEmail(string e) { email = e; }

    virtual string toCSV() const {
        return nombre + "," + id + "," + telefono + "," + email;
    }

    virtual void fromCSV(const string& csv) {
        stringstream ss(csv);
        string token;
        vector<string> tokens;
        while (getline(ss, token, ',')) {
            tokens.push_back(token);
        }
        if (tokens.size() >= 4) {
            nombre = tokens[0];
            id = tokens[1];
            telefono = tokens[2];
            email = tokens[3];
        }
    }

    virtual void mostrarInfo() const = 0;
};

class Paciente : public Persona {
    Fecha fechaIngreso;
    vector<HistorialClinico> historial;
    string grupoSanguineo;
    set<string> alergias;
    bool activo;

public:
    Paciente(string n, string i, const Fecha& f, string gs = "")
        : Persona(n, i), fechaIngreso(f), grupoSanguineo(gs), activo(true) {}

    void agregarHistorial(const HistorialClinico& entrada) {
        historial.push_back(entrada);
    }

    void agregarAlergia(const string& alergia) {
        alergias.insert(alergia);
    }

    bool tieneEnfermedadCronica() const {
        return any_of(historial.begin(), historial.end(),
                     [](const HistorialClinico& h) { return h.esCronico; });
    }

    const vector<HistorialClinico>& getHistorial() const { return historial; }
    Fecha getFechaIngreso() const { return fechaIngreso; }
    bool isActivo() const { return activo; }
    void setActivo(bool a) { activo = a; }

    string toCSV() const override {
        string csv = Persona::toCSV() + "," + fechaIngreso.toString() + "," + 
                    grupoSanguineo + "," + to_string(activo);
        
        csv += ",";
        for (const auto& alergia : alergias) {
            csv += alergia + ";";
        }
        
        csv += ",";
        for (const auto& h : historial) {
            csv += h.toString() + ";";
        }
        
        return csv;
    }

    void fromCSV(const string& csv) override {
        stringstream ss(csv);
        string token;
        vector<string> tokens;
        
        while (getline(ss, token, ',')) {
            tokens.push_back(token);
        }

        if (tokens.size() >= 7) {
            Persona::fromCSV(tokens[0] + "," + tokens[1] + "," + tokens[2] + "," + tokens[3]);
            fechaIngreso = Fecha::fromString(tokens[4]);
            grupoSanguineo = tokens[5];
            activo = (tokens[6] == "1");

            stringstream ss_alergias(tokens[7]);
            string alergia;
            while (getline(ss_alergias, alergia, ';')) {
                if (!alergia.empty()) {
                    alergias.insert(alergia);
                }
            }

            if (tokens.size() > 8) {
                stringstream ss_historial(tokens[8]);
                string entrada;
                while (getline(ss_historial, entrada, ';')) {
                    if (!entrada.empty()) {

                    }
                }
            }
        }
    }

    void mostrarInfo() const override {
        cout << "Paciente: " << nombre << " (ID: " << id << ")\n"
             << "Contacto: " << telefono << " / " << email << "\n"
             << "Fecha de ingreso: " << fechaIngreso.toString() << "\n"
             << "Grupo sanguíneo: " << grupoSanguineo << "\n"
             << "Estado: " << (activo ? "Activo" : "Inactivo") << "\n"
             << "Alergias: ";
        
        for (const auto& alergia : alergias) {
            cout << alergia << ", ";
        }
        cout << "\nHistorial Clinico:\n";
        for (const auto& h : historial) {
            cout << "- " << h.toString() << "\n";
        }
    }
};

class Medico : public Persona {
    Especialidad especialidad;
    bool disponible;
    map<Fecha, vector<string>> citasAsignadas;

public:
    Medico(string n, string i, Especialidad esp)
        : Persona(n, i), especialidad(esp), disponible(true) {}

    Especialidad getEspecialidad() const { return especialidad; }
    bool isDisponible() const { return disponible; }
    void setDisponibilidad(bool d) { disponible = d; }

    void agregarCita(const Fecha& fecha, const string& idCita) {
        citasAsignadas[fecha].push_back(idCita);
    }

    void eliminarCita(const Fecha& fecha, const string& idCita) {
        auto it = citasAsignadas.find(fecha);
        if (it != citasAsignadas.end()) {
            auto& citas = it->second;
            citas.erase(remove(citas.begin(), citas.end(), idCita), citas.end());
        }
    }

    bool tieneDisponibilidad(const Fecha& fecha) const {
        auto it = citasAsignadas.find(fecha);
        return it == citasAsignadas.end() || it->second.size() < 8;
    }

    string toCSV() const override {
        return Persona::toCSV() + "," + especialidadToString(especialidad) + "," + 
               to_string(disponible);
    }

    void fromCSV(const string& csv) override {
        stringstream ss(csv);
        string token;
        vector<string> tokens;
        
        while (getline(ss, token, ',')) {
            tokens.push_back(token);
        }

        if (tokens.size() >= 6) {
            Persona::fromCSV(tokens[0] + "," + tokens[1] + "," + tokens[2] + "," + tokens[3]);
            especialidad = stringToEspecialidad(tokens[4]);
            disponible = (tokens[5] == "1");
        }
    }

    void mostrarInfo() const override {
        cout << "Medico: " << nombre << " (ID: " << id << ")\n"
             << "Contacto: " << telefono << " / " << email << "\n"
             << "Especialidad: " << especialidadToString(especialidad) << "\n"
             << "Estado: " << (disponible ? "Disponible" : "No disponible") << "\n"
             << "Citas asignadas:\n";
        
        for (const auto& [fecha, citas] : citasAsignadas) {
            cout << fecha.toString() << ": " << citas.size() << " citas\n";
        }
    }
};

class Cita {
    string id;
    string idPaciente;
    string idMedico;
    Fecha fecha;
    string hora;
    string motivo;
    bool urgente;
    string estado;

public:
    Cita(string i, string ip, string im, const Fecha& f, string h, string m, bool u = false)
        : id(i), idPaciente(ip), idMedico(im), fecha(f), hora(h), motivo(m),
          urgente(u), estado("Pendiente") {}

    string getID() const { return id; }
    string getIdPaciente() const { return idPaciente; }
    string getIdMedico() const { return idMedico; }
    Fecha getFecha() const { return fecha; }
    string getHora() const { return hora; }
    string getMotivo() const { return motivo; }
    bool isUrgente() const { return urgente; }
    string getEstado() const { return estado; }

    void setFecha(const Fecha& f) { fecha = f; }
    void setHora(string h) { hora = h; }
    void setMotivo(string m) { motivo = m; }
    void setUrgente(bool u) { urgente = u; }
    void setEstado(string e) { estado = e; }

    string toCSV() const {
        return id + "," + idPaciente + "," + idMedico + "," + fecha.toString() + "," +
               hora + "," + motivo + "," + to_string(urgente) + "," + estado;
    }

    void fromCSV(const string& csv) {
        stringstream ss(csv);
        string token;
        vector<string> tokens;
        
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

    void mostrarInfo() const {
        cout << "Cita ID: " << id << "\n"
             << "Paciente ID: " << idPaciente << "\n"
             << "Medico ID: " << idMedico << "\n"
             << "Fecha: " << fecha.toString() << " " << hora << "\n"
             << "Motivo: " << motivo << "\n"
             << "Urgente: " << (urgente ? "Sí" : "No") << "\n"
             << "Estado: " << estado << "\n";
    }
};

class SistemaGestionClinica {
    map<string, unique_ptr<Paciente>> pacientes;
    map<string, unique_ptr<Medico>> medicos;
    map<string, unique_ptr<Cita>> citas;
    string rutaArchivos;

public:
    SistemaGestionClinica(const string& ruta = "./datos/") : rutaArchivos(ruta) {
        cargarDatos();
    }

    ~SistemaGestionClinica() {
        guardarDatos();
    }

    bool agregarPaciente(unique_ptr<Paciente> paciente) {
        string id = paciente->getID();
        if (pacientes.find(id) != pacientes.end()) return false;
        pacientes[id] = move(paciente);
        return true;
    }

    Paciente* buscarPaciente(const string& id) {
        auto it = pacientes.find(id);
        return it != pacientes.end() ? it->second.get() : nullptr;
    }


    bool agregarMedico(unique_ptr<Medico> medico) {
        string id = medico->getID();
        if (medicos.find(id) != medicos.end()) return false;
        medicos[id] = move(medico);
        return true;
    }

    Medico* buscarMedico(const string& id) {
        auto it = medicos.find(id);
        return it != medicos.end() ? it->second.get() : nullptr;
    }

    vector<Medico*> buscarMedicosPorEspecialidad(Especialidad esp) {
        vector<Medico*> resultado;
        for (const auto& [_, medico] : medicos) {
            if (medico->getEspecialidad() == esp && medico->isDisponible()) {
                resultado.push_back(medico.get());
            }
        }
        return resultado;
    }


    string generarIdCita() {
        return "CIT" + to_string(citas.size() + 1);
    }

    bool agendarCita(string idPaciente, string idMedico, const Fecha& fecha, 
                     const string& hora, const string& motivo, bool urgente = false) {

        auto paciente = buscarPaciente(idPaciente);
        auto medico = buscarMedico(idMedico);
        if (!paciente || !medico) return false;


        if (!medico->tieneDisponibilidad(fecha)) return false;


        string idCita = generarIdCita();
        auto cita = make_unique<Cita>(idCita, idPaciente, idMedico, fecha, hora, motivo, urgente);
        medico->agregarCita(fecha, idCita);
        citas[idCita] = move(cita);
        return true;
    }

    bool cancelarCita(const string& idCita) {
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

    vector<Cita*> buscarCitasPaciente(const string& idPaciente) {
        vector<Cita*> citasPaciente;
        for (const auto& [_, cita] : citas) {
            if (cita->getIdPaciente() == idPaciente) {
                citasPaciente.push_back(cita.get());
            }
        }
        return citasPaciente;
    }


    void guardarDatos() {

        ofstream archivoPacientes(rutaArchivos + "pacientes.csv");
        for (const auto& [_, paciente] : pacientes) {
            archivoPacientes << paciente->toCSV() << "\n";
        }

        ofstream archivoMedicos(rutaArchivos + "medicos.csv");
        for (const auto& [_, medico] : medicos) {
            archivoMedicos << medico->toCSV() << "\n";
        }

        ofstream archivoCitas(rutaArchivos + "citas.csv");
        for (const auto& [_, cita] : citas) {
            archivoCitas << cita->toCSV() << "\n";
        }
    }

	void cargarDatos() {

    ifstream archivoPacientes(rutaArchivos + "pacientes.csv");
    string linea;
    while (getline(archivoPacientes, linea)) {

        auto paciente = make_unique<Paciente>("", "", Fecha()); 
        paciente->fromCSV(linea);
        pacientes[paciente->getID()] = move(paciente);
    }


    ifstream archivoMedicos(rutaArchivos + "medicos.csv");
    while (getline(archivoMedicos, linea)) {

        auto medico = make_unique<Medico>("", "", Especialidad::GENERAL);
        medico->fromCSV(linea);
        medicos[medico->getID()] = move(medico);
    }


    ifstream archivoCitas(rutaArchivos + "citas.csv");
    while (getline(archivoCitas, linea)) {

        auto cita = make_unique<Cita>("", "", "", Fecha(), "", "");
        cita->fromCSV(linea);
        citas[cita->getID()] = move(cita);
    }
}


    map<Especialidad, int> obtenerEstadisticasEspecialidades() {
        map<Especialidad, int> estadisticas;
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

    vector<pair<string, int>> obtenerMedicosMasOcupados(const Fecha& inicio, const Fecha& fin) {
        map<string, int> conteo;
        for (const auto& [_, cita] : citas) {
            if (cita->getFecha().estaEnRango(inicio, fin)) {
                conteo[cita->getIdMedico()]++;
            }
        }

        vector<pair<string, int>> resultado(conteo.begin(), conteo.end());
        sort(resultado.begin(), resultado.end(),
             [](const auto& a, const auto& b) { return a.second > b.second; });
        
        return resultado;
    }

    void mostrarEstadisticas() const {
        cout << "\n=== Estadisticas del Sistema ===\n"
             << "Total de pacientes: " << pacientes.size() << "\n"
             << "Total de medicos: " << medicos.size() << "\n"
             << "Total de citas: " << citas.size() << "\n\n";


        map<Especialidad, int> distEsp;
        for (const auto& [_, medico] : medicos) {
            distEsp[medico->getEspecialidad()]++;
        }

        cout << "Distribucion de especialidades:\n";
        for (const auto& [esp, cant] : distEsp) {
            cout << especialidadToString(esp) << ": " << cant << " medicos\n";
        }
    }
};

int main() {


    std::filesystem::create_directories("datos");

    system("mkdir datos");

	SistemaGestionClinica sistema;


    auto medico1 = make_unique<Medico>("Dr. Juan Pérez", "M001", Especialidad::CARDIOLOGIA);
    auto medico2 = make_unique<Medico>("Dra. Ana García", "M002", Especialidad::PEDIATRIA);


    auto paciente1 = make_unique<Paciente>("Carlos López", "P001", Fecha(1, 1, 2024));
    auto paciente2 = make_unique<Paciente>("María Rodríguez", "P002", Fecha(2, 1, 2024));

    sistema.agregarMedico(move(medico1));
    sistema.agregarMedico(move(medico2));
    sistema.agregarPaciente(move(paciente1));
    sistema.agregarPaciente(move(paciente2));

    sistema.agendarCita("P001", "M001", Fecha(25, 11, 2024), "10:00", "Consulta rutinaria");
    sistema.agendarCita("P002", "M002", Fecha(26, 11, 2024), "11:00", "Control pediátrico");

    sistema.mostrarEstadisticas();

    return 0;
}

