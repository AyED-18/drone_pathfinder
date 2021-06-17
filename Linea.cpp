#include <iostream>
#include "Linea.hpp"

bool Linea::segmento(Coordenadas p, Coordenadas q, Coordenadas r)
{
  if (q.x <= std::max(p.x, r.x) && q.x >= std::min(p.x, r.x) &&
      q.y <= std::max(p.y, r.y) && q.y >= std::min(p.y, r.y))
    return true;

  return false;
}


// Encontrar la orientación del triplete ordenado (p, q, r).
// La función devuelve los siguientes valores
// 0 -> p, qyr son colineales
// 1 -> Sentido horario
// 2 -> en sentido antihorario
int Linea::orientacion(Coordenadas p, Coordenadas q, Coordenadas r)
{
  int val = (q.y - p.y) * (r.x - q.x) -
            (q.x - p.x) * (r.y - q.y);

  if(val == 0) return 0;  // colineal

  return (val > 0)? 1: 2; // en sentido horario o antihorario
}


bool Linea::intercepta(const Linea* linea){
  //definimos variables;
  Coordenadas p1 (coordenadasInicio.x , coordenadasInicio.y);
  Coordenadas q1 (coordenadasFin.x , coordenadasFin.y);
  Coordenadas p2 (linea->coordenadasInicio.x , linea->coordenadasInicio.y);
  Coordenadas q2 (linea->coordenadasFin.x , linea->coordenadasFin.y);

  // Encuentra las cuatro orientaciones necesarias para general y casos especiales
  int o1 = orientacion(p1, q1, p2);
  int o2 = orientacion(p1, q1, q2);
  int o3 = orientacion(p2, q2, p1);
  int o4 = orientacion(p2, q2, q1);

  // caso general
  if(o1 != o2 && o3 != o4)
    return true;

  // casos especiales
  // p1, q1 y p2 son colineal y p2 se encuentran en el segmento p1q1
  if(o1 == 0 && segmento(p1, p2, q1)) return true;

  // p1, q1 y q2 son colineal y q2 se encuentran en el segmento p1q1
  if(o2 == 0 && segmento(p1, q2, q1)) return true;

  // p2, q2 y p1 son colineal y p1 se encuentran en el segmento p2q2
  if(o3 == 0 && segmento(p2, p1, q2)) return true;

  // p2, q2 y q1 son colineal y q1 se encuentran en el segmento p2q2
  if(o4 == 0 && segmento(p2, q1, q2)) return true;

  return false; // si fallan los demas casos
}
