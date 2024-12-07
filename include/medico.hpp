#ifndef MEDICO_HPP
#define MEDICO_HPP

#include "persona.hpp"
#include "fecha.hpp"
#include "especialidad.hpp"
#include <map>
#include <vector>
#include <string>

class Medico : public Persona {
private:
    Especialidad especialidad;
    bool disponible;
    std::map<Fecha, std::vector<std::string>> citasAsignadas;

public:
    Medico(std::string n, std::string i, Especialidad esp);

    Especialidad getEspecialidad() const;
    bool isDisponible() const;
    void setDisponibilidad(bool d);

    void agregarCita(const Fecha& fecha, const std::string& idCita);
    void eliminarCita(const Fecha& fecha, const std::string& idCita);
    bool tieneDisponibilidad(const Fecha& fecha) const;

    std::string toCSV() const override;
    void fromCSV(const std::string& csv) override;

    void mostrarInfo() const override;
};

#endif // MEDICO_HPP

