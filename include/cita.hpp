#ifndef CITA_HPP
#define CITA_HPP

#include "fecha.hpp"
#include <string>
#include <sstream>
#include <vector>

class Cita {
private:
    std::string id;
    std::string idPaciente;
    std::string idMedico;
    Fecha fecha;
    std::string hora;
    std::string motivo;
    bool urgente;
    std::string estado;

public:
    Cita(std::string i, std::string ip, std::string im, const Fecha& f, std::string h, std::string m, bool u = false);

    std::string getID() const;
    std::string getIdPaciente() const;
    std::string getIdMedico() const;
    Fecha getFecha() const;
    std::string getHora() const;
    std::string getMotivo() const;
    bool isUrgente() const;
    std::string getEstado() const;

    void setFecha(const Fecha& f);
    void setHora(std::string h);
    void setMotivo(std::string m);
    void setUrgente(bool u);
    void setEstado(std::string e);

    std::string toCSV() const;
    void fromCSV(const std::string& csv);

    void mostrarInfo() const;
};

#endif // CITA_HPP

