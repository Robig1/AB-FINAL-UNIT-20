#include "Persona.hpp"

Persona::Persona(std::string n, std::string id, std::string fecha)
    : nombre(n), identificacion(id), fechaIngreso(fecha) {}

Persona::~Persona() {}

std::string Persona::getNombre() const {
    return nombre;
}

std::string Persona::getIdentificacion() const {
    return identificacion;
}

std::string Persona::getFechaIngreso() const {
    return fechaIngreso;
}

void Persona::modificarDatos(std::string n, std::string id, std::string fecha) {
    nombre = n;
    identificacion = id;
    fechaIngreso = fecha;
}

