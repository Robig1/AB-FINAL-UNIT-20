#ifndef PACIENTE_HPP
#define PACIENTE_HPP

#include "persona.hpp"
#include "fecha.hpp"
#include "historial_clinico.hpp"
#include <set>
#include <vector>
#include <string>

class Paciente : public Persona {
private:
    Fecha fechaIngreso;
    std::vector<HistorialClinico> historial;
    std::string grupoSanguineo;
    std::set<std::string> alergias;
    bool activo;

public:
    Paciente(std::string n, std::string i, const Fecha& f, std::string gs = "");

    void agregarHistorial(const HistorialClinico& entrada);
    void agregarAlergia(const std::string& alergia);
    bool tieneEnfermedadCronica() const;
    
    const std::vector<HistorialClinico>& getHistorial() const;
    Fecha getFechaIngreso() const;
    bool isActivo() const;
    void setActivo(bool a);

    std::string toCSV() const override;
    void fromCSV(const std::string& csv) override;

    void mostrarInfo() const override;
};

#endif // PACIENTE_HPP

