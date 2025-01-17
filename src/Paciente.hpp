#ifndef PACIENTE_HPP
#define PACIENTE_HPP

#include "Persona.hpp"
#include <vector>
#include <string>

class Paciente : public Persona {
private:
    std::vector<std::string> historialClinico;
    bool tieneEnfermedadCronica_;
public:
    Paciente(std::string n, std::string id, std::string fecha, bool cronic = false);
    void registrarHistorial(std::string historial);
    std::vector<std::string> getHistorialClinico() const;
    std::string toCSV() const;
    bool tieneEnfermedadCronica() const;
    void setEnfermedadCronica(bool cronic);
};

#endif // PACIENTE_HPP

