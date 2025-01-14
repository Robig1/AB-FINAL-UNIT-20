#ifndef CITA_HPP
#define CITA_HPP

#include <string>

class Cita {
private:
    std::string fecha;
    std::string pacienteID;
    std::string medicoID;
    bool esUrgente;
public:
    Cita(std::string f, std::string pID, std::string mID, bool urgente);
    std::string getFecha() const;
    std::string getPacienteID() const;
    std::string getMedicoID() const;
    bool getUrgencia() const;
    std::string toCSV() const;
};

#endif // CITA_HPP
