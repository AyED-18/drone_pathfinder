#ifndef AYED_18_TP_3_MAIN_HPP
#define AYED_18_TP_3_MAIN_HPP

#include <string>
#include <bitset>
#include "Cola.hpp"
#include "Barrera.hpp"
#include "Maleza.hpp"
#include "Tramo.hpp"
#include "GrafoNoDirigido.hpp"

#define TAMANO 100

//prototipado de funciones
void ejecutarBienvenida();
std::string obtenerRuta();
std::string obtenerTexto(std::string ruta);
char obtenerImagenParcial(std::string texto,int i, int j);
void generarImagenMatricial(std::string &texto, char imagen[TAMANO][TAMANO]);
Cola<Barrera*>* encontrarBarreras(char imagen[TAMANO][TAMANO]);
Cola<Maleza*>* encontrarMalezas(char imagen[TAMANO][TAMANO]);
int revisarSiEsMaleza(int param, int direccionx, int direcciony, std::bitset<TAMANO> m[TAMANO]);
Cola<Tramo*>* generarTramos(Coordenadas origen, Cola<Maleza*>* malezas);
void reducirTramos(Cola<Tramo*>* tramos, Cola<Barrera*>* barreras);
void ejecutarOutputFinal(const GrafoNoDirigido& grafoNoDirigido, Cola<Maleza*>* malezas);

#endif