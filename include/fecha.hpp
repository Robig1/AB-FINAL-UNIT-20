#ifndef FECHA_HPP
#define FECHA_HPP

#include <string>
#include <sstream>
#include <vector>
#include <iomanip>
#include <chrono>

class Fecha {
private:
    int dia, mes, anio;

public:
    Fecha();
    Fecha(int d, int m, int a);

    static Fecha hoy();
    static Fecha fromString(const std::string& fecha);
    std::string toString() const;

    bool operator<(const Fecha& otra) const;
    bool operator==(const Fecha& otra) const;
    bool estaEnRango(const Fecha& inicio, const Fecha& fin) const;
    bool operator>(const Fecha& otra) const;
};

#endif // FECHA_HPP

