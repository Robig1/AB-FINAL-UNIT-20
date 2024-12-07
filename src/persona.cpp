#include "persona.hpp"
#include <sstream>

Persona::Persona(std::string n, std::string i, std::string t, std::string e)
    : nombre(n), id(i), telefono(t), email(e) {}

std::string Persona::getNombre() const { return nombre; }
std::string Persona::getID() const { return id; }
std::string Persona::getTelefono() const { return telefono; }
std::string Persona::getEmail() const { return email; }

void Persona::setNombre(std::string n) { nombre = n; }
void Persona::setID(std::string i) { id = i; }
void Persona::setTelefono(std::string t) { telefono = t; }
void Persona::setEmail(std::string e) { email = e; }

std::string Persona::toCSV() const {
    return nombre + "," + id + "," + telefono + "," + email;
}

void Persona::fromCSV(const std::string& csv) {
    std::stringstream ss(csv);
    std::string token;
    std::vector<std::string> tokens;
    while (std::getline(ss, token, ',')) {
        tokens.push_back(token);
    }
    if (tokens.size() >= 4) {
        nombre = tokens[0];
        id = tokens[1];
        telefono = tokens[2];
        email = tokens[3];
    }
}

