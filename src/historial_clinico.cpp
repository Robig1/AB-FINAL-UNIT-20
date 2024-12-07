#include "historial_clinico.hpp"

std::string HistorialClinico::toString() const {
    return fecha.toString() + " - " + descripcion + " - " + diagnostico + 
           " - " + tratamiento + " - " + (esCronico ? "Cronico" : "No cronico");
}

