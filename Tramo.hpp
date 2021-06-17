#ifndef AYED_18_TP_3_TRAMO_HPP
#define AYED_18_TP_3_TRAMO_HPP

#include <math.h>
#include "Linea.hpp"

class Tramo : public Linea {
  public:
    // variables
    float distancia;
    // funciones
    Tramo (Coordenadas coordenadasInicio, Coordenadas coordenadasFin) : Linea(coordenadasInicio, coordenadasFin) {
      distancia = sqrt(pow((coordenadasFin.x - coordenadasInicio.x), 2) +
        pow((coordenadasFin.y - coordenadasInicio.y), 2));
    };
};

#endif
