#ifndef AYED_18_TP_3_MALEZA_HPP
#define AYED_18_TP_3_MALEZA_HPP

#include "Coordenadas.hpp"

class Maleza {
  public:
    // variables
    Coordenadas coordenadas;
    // funciones
    Maleza(int coordenadaX, int coordenadaY) : coordenadas(Coordenadas(coordenadaX, coordenadaY)) {};
};

#endif
