#ifndef PERSONA_HPP
#define PERSONA_HPP

#include <string>

class Persona {
protected:
    std::string nombre;
    std::string identificacion;
    std::string fechaIngreso;
public:
    Persona(std::string n, std::string id, std::string fecha);
    virtual ~Persona();
    std::string getNombre() const;
    std::string getIdentificacion() const;
    std::string getFechaIngreso() const;
    void modificarDatos(std::string n, std::string id, std::string fecha);
};

#endif // PERSONA_HPP

