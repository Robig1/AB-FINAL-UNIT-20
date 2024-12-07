#ifndef HISTORIAL_CLINICO_HPP
#define HISTORIAL_CLINICO_HPP

#include "fecha.hpp"
#include <string>

struct HistorialClinico {
    Fecha fecha;
    std::string descripcion;
    std::string diagnostico;
    std::string tratamiento;
    bool esCronico;

    std::string toString() const;
};

#endif // HISTORIAL_CLINICO_HPP

