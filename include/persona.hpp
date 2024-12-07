#ifndef PERSONA_HPP
#define PERSONA_HPP

#include <string>
#include <sstream>
#include <vector>

class Persona {
protected:
    std::string nombre;
    std::string id;
    std::string telefono;
    std::string email;

public:
    Persona(std::string n, std::string i, std::string t = "", std::string e = "");
    virtual ~Persona() = default;

    std::string getNombre() const;
    std::string getID() const;
    std::string getTelefono() const;
    std::string getEmail() const;

    void setNombre(std::string n);
    void setID(std::string i);
    void setTelefono(std::string t);
    void setEmail(std::string e);

    virtual std::string toCSV() const;
    virtual void fromCSV(const std::string& csv);

    virtual void mostrarInfo() const = 0;  // Método virtual puro
};

#endif // PERSONA_HPP

