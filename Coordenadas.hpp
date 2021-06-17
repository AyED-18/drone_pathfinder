#ifndef AYED_18_TP_3_COORDENADAS_HPP
#define AYED_18_TP_3_COORDENADAS_HPP

// Un objeto #Coordenadas representa un punto en cierto plano cartesiano. Cada instancia tiene un id unico.
class Coordenadas {
  private:
    // variables de clase
    static int proximaIdAsignable;
  public:
    // variables
    int id;
    int x;
    int y;
    // funciones
    Coordenadas(int x, int y) : x(x), y(y), id(proximaIdAsignable++) {};
};

#endif
