#ifndef AYED_18_TP_3_LINEA_HPP
#define AYED_18_TP_3_LINEA_HPP

#include "Coordenadas.hpp"

class Linea {
  public:
    // variables
    Coordenadas coordenadasInicio;
    Coordenadas coordenadasFin;
    // funciones
    Linea(Coordenadas coordenadasInicio, Coordenadas coordenadasFin) :
      coordenadasInicio(coordenadasInicio), coordenadasFin(coordenadasFin) {};
    bool intercepta(const Linea* linea);
  private:
    // variables
    bool segmento(Coordenadas p, Coordenadas q, Coordenadas r);
    int orientacion(Coordenadas p, Coordenadas q, Coordenadas r);
};

#endif
