#ifndef AYED_18_TP_3_BARRERA_HPP
#define AYED_18_TP_3_BARRERA_HPP

#include "Coordenadas.hpp"
#include "Linea.hpp"

class Barrera : public Linea {
  public:
    // funciones
    Barrera(Coordenadas coordenadasInicio, Coordenadas coordenadasFin) : Linea(coordenadasInicio, coordenadasFin) {};
};

#endif
