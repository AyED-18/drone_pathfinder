#include "GrafoNoDirigido.hpp"

// Constructor.
GrafoNoDirigido::GrafoNoDirigido(Cola<Tramo*>* tramos, int cantNodos) {
  // inicializacion de variables de instancia
  this->tramos = tramos;
  this->cantNodos = cantNodos;
  // armo matriz de adyacencia (two dimensional dynamic array)
  matrizDeAdyacencia = new float *[cantNodos];
  for(int i = 0; i < cantNodos; ++i) {
    matrizDeAdyacencia[i] = new float [cantNodos];
  }
  // la inicializo a valores INF por defecto, lo que significa: no-conexion entre respectivos nodos
  for(int i = 0; i < cantNodos; ++i) {
    for(int j = 0; j < cantNodos; ++j) {
      matrizDeAdyacencia[i][j] = INF;
    }
  }
  // itero por cada tramo, e impacto dicha informacion dentro de la matriz
  Nodo<Tramo*>* tramoActual = tramos->getInicio();
  while(tramoActual) {
    matrizDeAdyacencia[tramoActual->dato->coordenadasInicio.id][tramoActual->dato->coordenadasFin.id] =
      tramoActual->dato->distancia;
    matrizDeAdyacencia[tramoActual->dato->coordenadasFin.id][tramoActual->dato->coordenadasInicio.id] =
      tramoActual->dato->distancia;
    tramoActual = tramoActual->next;
  }
}


// Este algoritmo utiliza una "busqueda en amplitud", y se sirve de una cola de datos para dicho proposito.
void GrafoNoDirigido::generarCicloHamiltoneanoEficiente() {
  Cola<Cola<int>*> descubridora; // i.e.: [[1, 2], [1, 3], [1, 4], [1, 4, 5], [1, 4, 3], ...]
  // la coordenada con id 0 es el inicio del camino
  descubridora.encolar(new Cola<int>(0)); // [] << [0]
  while(!descubridora.vacia()) {
    Cola<int>* potencialCamino = descubridora.desencolar();
    // si se han recorrido todos los nodos...
    if(potencialCamino->getTamano() == cantNodos) {
      // chequear si desde el ultimo nodo del camino se puede volver al origen (0)
      if(nodosVinculados(potencialCamino->getFin()->dato, 0)) {
        // si se puede
        potencialCamino->encolar(0);
        float distanciaDePotencialCamino = calcularDistancia(potencialCamino);
        if(distanciaMasEficiente > distanciaDePotencialCamino) {
          distanciaMasEficiente = distanciaDePotencialCamino;
          if(cicloHamiltoneanoEficiente) delete(cicloHamiltoneanoEficiente); // limpieza
          cicloHamiltoneanoEficiente = potencialCamino;
        } else {
          delete(potencialCamino); // limpieza
        }
      }
    } else {
      // desde el ultimo nodo del camino voy a nodos no descubiertos, si es que se puede
      for(int i = 0; i < cantNodos; ++i) {
        // se chequea si cierto nodo esta vinculado y no se encuentra impactado en el camino
        if(!potencialCamino->contiene(i) && nodosVinculados(potencialCamino->getFin()->dato, i)) {
          // se agrega a un potencial camino
          Cola<int>* nuevoPotencialCamino = new Cola<int>(potencialCamino);
          nuevoPotencialCamino->encolar(i);
          descubridora.encolar(nuevoPotencialCamino);
        }
      }
      delete(potencialCamino); // limpieza
    }
  }
}


// Devuelve true si el nodo1 esta vinculado al nodo2 y viceversa, observando la matriz de adyacencia.
bool GrafoNoDirigido::nodosVinculados(int nodo1, int nodo2) {
  return matrizDeAdyacencia[nodo1][nodo2] != INF ? true : false;
}


// Dado cierto *camino* sumo cada tramo que lo compone, arrojando una distancia total.
float GrafoNoDirigido::calcularDistancia(Cola<int> *camino) {
  // dada un camino "vacio", devolver 0
  if(camino->vacia()) return 0;
  // calculo distancia del camino
  float distancia = 0;
  Nodo<int>* nodoActual = camino->getInicio();
  while(nodoActual->next) {
    distancia += matrizDeAdyacencia[nodoActual->dato][nodoActual->next->dato];
    nodoActual = nodoActual->next;
  }
  return distancia;
}
