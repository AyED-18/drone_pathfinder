#ifndef AYED_18_TP_3_GRAFONODIRIGIDO_HPP
#define AYED_18_TP_3_GRAFONODIRIGIDO_HPP

#include "Cola.hpp"
#include "Tramo.hpp"

#define INF 999

class GrafoNoDirigido {
  public:
    // variables
    Cola<Tramo*>* tramos;
    int cantNodos;
    float **matrizDeAdyacencia;
    // las proximas dos variables obtienen valor luego de llamar a #generarCicloHamiltoneanoEficiente()
    Cola<int>* cicloHamiltoneanoEficiente = nullptr;
    float distanciaMasEficiente = INF;
    // funciones
    GrafoNoDirigido(Cola<Tramo*>* tramos, int cantNodos);
    void generarCicloHamiltoneanoEficiente();
  private:
    // funciones
    bool nodosVinculados(int nodo1, int nodo2);
    float calcularDistancia(Cola<int>* camino);
};

#endif
