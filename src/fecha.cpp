#include "fecha.hpp"
#include <ctime>
#include <sstream>

Fecha::Fecha() : dia(1), mes(1), anio(2024) {}

Fecha::Fecha(int d, int m, int a) : dia(d), mes(m), anio(a) {}

Fecha Fecha::hoy() {
    auto now = std::chrono::system_clock::now();
    time_t tt = std::chrono::system_clock::to_time_t(now);
    tm local_tm;
    #ifdef _WIN32
        localtime_s(&local_tm, &tt);
    #else
        localtime_r(&tt, &local_tm);
    #endif
    
    return Fecha(local_tm.tm_mday, local_tm.tm_mon + 1, local_tm.tm_year + 1900);
}

Fecha Fecha::fromString(const std::string& fecha) {
    std::istringstream ss(fecha);
    std::string token;
    std::vector<int> componentes;
    
    while (std::getline(ss, token, '-')) {
        componentes.push_back(std::stoi(token));
    }
    
    if (componentes.size() != 3) return Fecha();
    return Fecha(componentes[2], componentes[1], componentes[0]);
}

std::string Fecha::toString() const {
    std::stringstream ss;
    ss << std::setfill('0') << std::setw(4) << anio << "-"
       << std::setfill('0') << std::setw(2) << mes << "-"
       << std::setfill('0') << std::setw(2) << dia;
    return ss.str();
}

bool Fecha::operator<(const Fecha& otra) const {
    if (anio != otra.anio) return anio < otra.anio;
    if (mes != otra.mes) return mes < otra.mes;
    return dia < otra.dia;
}

bool Fecha::operator==(const Fecha& otra) const {
    return dia == otra.dia && mes == otra.mes && anio == otra.anio;
}

bool Fecha::estaEnRango(const Fecha& inicio, const Fecha& fin) const {
    return !(*this < inicio) && !(*this > fin);
}

bool Fecha::operator>(const Fecha& otra) const {
    return otra < *this;
}

