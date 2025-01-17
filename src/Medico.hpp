#ifndef MEDICO_HPP
#define MEDICO_HPP

#include "Persona.hpp"
#include <string>

class Medico : public Persona {
private:
    std::string especialidad;
    bool disponible;
public:
    Medico(std::string n, std::string id, std::string fecha, std::string esp, bool disp = true);
    std::string getEspecialidad() const;
    bool isDisponible() const;
    void setDisponibilidad(bool disp);
    std::string toCSV() const;
};

#endif // MEDICO_HPP

