#ifndef CITA_HPP
#define CITA_HPP

#include <string>
#include <ctime>

class Cita {
private:
    std::string fecha;
    std::string pacienteID;
    std::string medicoID;
    bool esUrgente;
    bool esPasada;
public:
    Cita(std::string f, std::string pID, std::string mID, bool urgente);
    std::string getFecha() const;
    std::string getPacienteID() const;
    std::string getMedicoID() const;
    bool getUrgencia() const;
    bool isPasada() const;
    void setPasada(bool pasada);
    std::string toCSV() const;
};

#endif // CITA_HPP

