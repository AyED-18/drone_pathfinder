#include <iostream>
#include <string>
#include <fstream>
#include <bitset>
#include "main.hpp"
#include "Coordenadas.hpp"
#include "Cola.hpp"
#include "Barrera.hpp"
#include "Maleza.hpp"
#include "Tramo.hpp"
#include "GrafoNoDirigido.hpp"

int main(){

  ejecutarBienvenida();

  // ejecutar el pedido de ruta al archivo "mapa"
  std::string ruta = obtenerRuta();

  // extraemos el texto del archivo
  std::string texto = obtenerTexto(ruta);
  if(texto.empty()) {
    std::cout << "La ruta ingresada es invalida o el archivo no contiene dato alguno." << std::endl;
    return 0;
  }

  // obtener imagen matricial desde texto aportado
  char imagen[TAMANO][TAMANO];
  generarImagenMatricial(texto, imagen);

  Coordenadas origen(0, 0);
  Cola<Maleza*>* malezas = encontrarMalezas(imagen);
  Cola<Barrera*>* barreras = encontrarBarreras(imagen);
  // generar todos los tramos, ignorando barreras
  Cola<Tramo*>* tramos = generarTramos(origen, malezas);
  // reducir los tramos generados a solamente aquellos no interceptados por barreras
  reducirTramos(tramos, barreras);

  // generamos el grafo no dirigido y ejecutamos la generacion del ciclo hamiltoneano eficiente
  GrafoNoDirigido grafoNoDirigido(tramos, malezas->getTamano() + 1); // + 1 es por el origen, que tambien es un nodo
  grafoNoDirigido.generarCicloHamiltoneanoEficiente();

  ejecutarOutputFinal(grafoNoDirigido, malezas);

  return 0;
}


// Pasaje de texto lineal a matriz util.
void generarImagenMatricial(std::string &texto, char imagen[TAMANO][TAMANO]) {
  for(int i = 0; i < TAMANO * TAMANO; i++){
    imagen[i / TAMANO][i % TAMANO] = obtenerImagenParcial(texto, i / TAMANO, i % TAMANO);
  }
}


void ejecutarBienvenida() {
  std::cout << std::endl << "PLANIFICACION DE VUELO DE UN DRONE" << std::endl;
  std::cout << "======================================" << std::endl << std::endl;
  std::cout << "Bienvenido. ";
}


std::string obtenerRuta() {
  std::string s;
  std::cout << "Ingrese la ruta de su archivo mapa: ";
  std::getline(std::cin >> std::ws, s);
  std::cout << std::endl;
  return s;
}


// Obtiene el texto a procesar, a partir del contenido de cierto archivo ubicado en *ruta*.
std::string obtenerTexto(std::string ruta) {
  std::ifstream archivo;
  std::string texto;

  // abre el archivo
  archivo.open(ruta);
  if(archivo.fail()) {
    // el archivo no existe, devuelvo string vacia
    return texto;
  }

  // extraer string
  std::string linea;
  while(!archivo.eof()){
    std::getline(archivo, linea);
    texto += linea;
  }

  archivo.close();
  return texto;
}


// Helper para obtencion de pixel data.
char obtenerImagenParcial(std::string texto, int i, int j) {
  return texto[(i * TAMANO) + j];
}


// Encuentra cada una de las barreras (terreno impasable) del mapa, y las devuelve.
Cola<Barrera*>* encontrarBarreras(char imagen[TAMANO][TAMANO]) {
  Cola<Barrera*>* barreras = new Cola<Barrera*>;
  // analisis horizontal
  int i, j, tamanoActualPosibleBarrera = 0;
  for(i = 0; i < TAMANO; ++i) {
    for(j = 0; j < TAMANO; ++j) {
      if(tamanoActualPosibleBarrera == 0) {
        if(imagen[i][j] == 'X') {
          // posible barrera horizontal
          int coordXActual = j + 1;
          tamanoActualPosibleBarrera++;
          while((coordXActual < TAMANO) && (imagen[i][coordXActual] == 'X')) {
            coordXActual++;
            tamanoActualPosibleBarrera++;
          }
          if(tamanoActualPosibleBarrera >= 4) {
            // barrera horizontal encontrada
            Barrera* nuevaBarrera = new Barrera(Coordenadas(j, i), Coordenadas(j + tamanoActualPosibleBarrera - 1, i));
            barreras->encolar(nuevaBarrera);
          }
          tamanoActualPosibleBarrera--;
        }
      } else {
        tamanoActualPosibleBarrera--;
      }
    }
  }
  // analisis vertical
  tamanoActualPosibleBarrera = 0;
  for(j = 0; j < TAMANO; ++j) {
    for(i = 0; i < TAMANO; ++i) {
      if(tamanoActualPosibleBarrera == 0) {
        if(imagen[i][j] == 'X') {
          // posible barrera vertical
          int coordYActual = i + 1;
          tamanoActualPosibleBarrera = 1;
          while((coordYActual < TAMANO) && (imagen[coordYActual][j] == 'X')) {
            coordYActual++;
            tamanoActualPosibleBarrera++;
          }
          if(tamanoActualPosibleBarrera >= 4) {
            // barrera vertical encontrada
            Barrera *nuevaBarrera = new Barrera(Coordenadas(j, i), Coordenadas(j, i + tamanoActualPosibleBarrera - 1));
            barreras->encolar(nuevaBarrera);
          }
          tamanoActualPosibleBarrera--;
        }
      } else {
        tamanoActualPosibleBarrera--;
      }
    }
  }

  return barreras;
}


// Filtra el mapa en busqueda de malezas y devuelve una cola con todas las descubiertas.
Cola<Maleza*>* encontrarMalezas(char imagen[TAMANO][TAMANO]) {
  Cola<Maleza*>* malezas = new Cola<Maleza*>;
  // 1. crear bitset-matriz: bitset<TAMANO> m[TAMANO] y asignar valores de matriz imagen
  std::bitset<TAMANO> m[TAMANO];
  for(int n = 0; n < TAMANO; n++) {
    for(int o = 0; o < TAMANO; o++) {
      m[n][o] = imagen[n][o] == '1' ? 1 : 0;
    }
  }
  int centrox = 0, centroy = 0, longitud = 0;
  // 2. buscar primer 1 de cada maleza
  for(int fila = 0; fila < TAMANO; fila++) {
    // 3. la fila entera no es nula?
    if(!m[fila].none()){
      // 4. buscar cada bit
      for(int columna = 0; columna < TAMANO; columna++) {
        // 5. al encontar un 1, revisar alrededor y obtener longitud

        if(m[fila].test(columna)) {
          longitud++;
          continue;
        } else {
          if(longitud != 0) {
            if(longitud >= 3) {
              int potencialLongitud = revisarSiEsMaleza(longitud, fila, columna - longitud, m);
              if(potencialLongitud >= 0) {
                centroy = (fila + (potencialLongitud / 2));
                centrox = ((columna - longitud) + (potencialLongitud / 2));
                Maleza* maleza = new Maleza(centrox, centroy);
                malezas->encolar(maleza);
              }
              longitud = 0;
              centrox = 0;
              centroy = 0;
            } else {
              // resetear longitud
              longitud = 0;
            }
          }
        }
      }
      // chequear longitud, puede haber maleza "pegada" al borde derecho del mapa
      if(longitud != 0) {
        if(longitud >= 3) {
          int potencialLongitud = revisarSiEsMaleza(longitud, fila, TAMANO - longitud, m);
          if(potencialLongitud >= 0) {
            centrox = (fila + (potencialLongitud / 2));
            centroy = ((TAMANO - longitud) + (potencialLongitud / 2));
            Maleza* maleza = new Maleza(centrox, centroy);
            malezas->encolar(maleza);
          }
          longitud = 0;
          centrox = 0;
          centroy = 0;
        } else {
          // resetear longitud
          longitud = 0;
        }
      }
    }
  }
  return malezas;
}


// Devuelve longitud real de la maleza; -1 si la anomalia no califica como maleza.
int revisarSiEsMaleza(int longLado, int direccionY, int direccionX, std::bitset<TAMANO> matriz[TAMANO]){
  // inicia longLado helper
  int helperX = 0;
  // revisa las (lado) filas siguientes
  for(int s = direccionY + 1; s < direccionY + longLado; s++) {
    // si contiene solo 0s es falso
    if(!matriz[s].none()) {
      // revisa los (lado) bits desde el indice indicado
      int t;
      for(t = direccionX; t < direccionX + longLado; t++){
        // cuenta mientras den 1s
        while(((t + helperX) < TAMANO) && (matriz[s].test(t + helperX))) {
          helperX++;
        }
      }

      // mientras no sea demasiado chico, ajusta el tamaño de la maleza
      if((helperX < longLado) && (helperX > 2)) {
        longLado = helperX;
        helperX = 0;
        // si se llega al maximo predeterminado para lado, es maleza
        if(t == direccionX + longLado){
          for(int h=0; h<longLado; h++){
            for(int i=0; i<longLado; i++){
              matriz[direccionY+h][direccionX+i]=0;
            }
          }
          return longLado;
        }
      } else if((longLado == helperX) && (helperX > 2)) {
        helperX=0;
        // si se llega al maximo predeterminado para lado, es maleza
        if(t == direccionX + longLado) {
          for(int j=0; j<longLado; j++){
            for(int k=0; k<longLado; k++){
              matriz[direccionY+j][direccionX+k]=0;
            }
          }
          return longLado;
        }
      } else return -1;
    }
    else return-1;
  }
  return -1;
}


// Genera todos los tramos del sistema. Cada tramo es una arista entre nodos (maleza y/u origen).
Cola<Tramo*>* generarTramos(Coordenadas origen, Cola<Maleza*>* malezas) {
  // fase obtencion de todos los tramos posibles
  Cola<Tramo*>* tramos = new Cola<Tramo*>;
  // tramos origen vs malezas
  Nodo<Maleza*>* nodoMalezaActual = malezas->getInicio();
  while(nodoMalezaActual) {
    Tramo* nuevoTramo = new Tramo(origen, nodoMalezaActual->dato->coordenadas);
    tramos->encolar(nuevoTramo);
    nodoMalezaActual = nodoMalezaActual->next;
  }
  // tramos malezas vs malezas
  for(int i = 0; i < malezas->getTamano(); i++) {
    for(int j = 1; j < malezas->getTamano(); j++) {
      if(j > i) {
        Tramo* nuevoTramo = new Tramo(malezas->enIndex(i)->dato->coordenadas, malezas->enIndex(j)->dato->coordenadas);
        tramos->encolar(nuevoTramo);
      }
    }
  }
  return tramos;
}


/*
 * Se entiende que los *tramos* pasados son aquellos totales del sistema, ignorando barreras. Este metodo busca
 * la interseccion entre cada tramo vs cada barrera y quita de *tramos* aquellos que suceden.
*/
void reducirTramos(Cola<Tramo*>* tramos, Cola<Barrera*>* barreras) {
  // fase de reduccion de tramos dado colisiones con barreras
  Nodo<Tramo*>* nodoTramoActual = tramos->getInicio();
  while(nodoTramoActual) {
    for(int i = 0; i < barreras->getTamano(); ++i) {
      if(nodoTramoActual->dato->intercepta(barreras->enIndex(i)->dato)) {
        // este tramo intercepta una barrera, lo quito
        Nodo<Tramo*>* nodoAEliminar = nodoTramoActual;
        nodoTramoActual = nodoTramoActual->next;
        tramos->eliminar(nodoAEliminar->dato);
        continue;
      }
    }
    nodoTramoActual = nodoTramoActual->next;
  }
}


// Ejecuta mensajes finales de salida del programa.
void ejecutarOutputFinal(const GrafoNoDirigido& grafoNoDirigido, Cola<Maleza*>* malezas) {
  std::cout << "Ciclo Hamiltinoeano Eficiente: ";
  Nodo<int>* nodoActual = grafoNoDirigido.cicloHamiltoneanoEficiente->getInicio();
  while(nodoActual) {
    if(nodoActual->dato == 0) {
      // origen
      std::cout << "[0] (x: 0, y: 0)";
    } else {
      // malezas
      std::cout << "[" <<
      nodoActual->dato << "] (x: " << malezas->enIndex(nodoActual->dato - 1)->dato->coordenadas.x + 1 << ", y: " <<
      malezas->enIndex(nodoActual->dato - 1)->dato->coordenadas.y + 1 << ")";
    }

    nodoActual = nodoActual->next;
    if(nodoActual) {
      std::cout << " => ";
    } else {
      std::cout << "." << std::endl;
    }
  }
  std::cout << std::endl << "Distancia Hamiltoneana mas eficiente (corta): " << grafoNoDirigido.distanciaMasEficiente
  << std::endl;
}
