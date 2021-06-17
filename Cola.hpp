#ifndef AYED_18_TP_3_COLA_HPP
#define AYED_18_TP_3_COLA_HPP

#include "Nodo.hpp"

template <class T> class Cola {
  private:
    // variables de instancia
    Nodo<T>* inicio = nullptr;
    Nodo<T>* fin = nullptr;
    int tamano = 0;

  public:
    // funciones
    Cola() {};
    Cola(T dato) {encolar(dato);};
    Cola(Cola<T>* cola);
    Nodo<T>* getInicio() {return inicio;};
    Nodo<T>* getFin() {return fin;};
    int getTamano() {return tamano;};
    void encolar(T dato);
    T desencolar();
    void priorizar(T dato);
    bool vacia() {return inicio ? false : true;};
    void vaciar() {while(desencolar());};
    bool contiene(T dato);
    bool eliminar(T dato);
    Nodo<T>* enIndex(unsigned index);
};



/* definiciones de las funciones no-inline de la clase */
// Copy-initializer.
template<class T>
Cola<T>::Cola(Cola<T>* cola) {
  if(!cola->vacia()) {
    Nodo<T>* nodoActual = cola->getInicio();
    while(nodoActual) {
      encolar(nodoActual->dato);
      nodoActual = nodoActual->next;
    }
  }
}


template<typename T>
void Cola<T>::encolar(T dato) {
  if(!vacia()) {
    // si no esta vacia
    Nodo<T>* nuevoNodo = new Nodo<T>(dato);
    fin->next = nuevoNodo;
    fin = nuevoNodo;
  } else {
    // si esta vacia
    fin = inicio = new Nodo<T>(dato);
  }
  tamano++;
}


template<typename T>
T Cola<T>::desencolar() {
  // el primer de la fila debe salir
  if(!vacia()) {
    // copy initialization
    T datoADevolver(inicio->dato);
    Nodo<T>* nodoABorrar = inicio;
    inicio = inicio->next;
    if(!inicio) {
      fin = nullptr;
    }
    delete nodoABorrar;
    tamano--;
    return datoADevolver;
  } else {
    return nullptr;
  }
}


template<class T>
bool Cola<T>::contiene(T dato) {
  Nodo<T>* nodoIterado = inicio;
  while(nodoIterado) {
    if(nodoIterado->dato == dato) {
      return true;
    }
    nodoIterado = nodoIterado->next;
  }
  return false;
}


// Viola el principio de una cola e ingresa un nuevo dato como prioridad, primero en la cola.
template<class T>
void Cola<T>::priorizar(T dato) {
  if(!vacia()) {
    // si no esta vacia
    Nodo<T>* nuevoNodo = new Nodo<T>(dato);
    nuevoNodo->next = inicio;
    inicio = nuevoNodo;
  } else {
    // si esta vacia
    fin = inicio = new Nodo<T>(dato);
  }
  tamano++;
}


/*
* Elimina el primer elemento cuyo dato coincida con el dato pasado como argumento, devuelve true si lo logra, false
* si no encuentra match.
*/
template<class T>
bool Cola<T>::eliminar(T dato) {
  Nodo<T>* nodoAnterior = nullptr;
  Nodo<T>* nodoIterado = inicio;
  while(nodoIterado) {
    if(nodoIterado->dato == dato) {
      if(nodoAnterior) {
        nodoAnterior->next = nodoIterado->next;
      } else {
        // el *nodoIterado* es el inicial
        inicio = nodoIterado->next;
      }
      delete(nodoIterado);
      tamano--;
      return true;
    }
    nodoAnterior = nodoIterado;
    nodoIterado = nodoIterado->next;
  }
  return false;
}


template<class T>
Nodo<T>* Cola<T>::enIndex(unsigned index) {
  // si index out of bounds, return nullptr
  if(index > (tamano - 1)) return nullptr;
  Nodo<T>* nodoIterado = inicio;
  for(int i = 0; i < tamano; ++i) {
    if(i == index) {
      return nodoIterado;
    }
    nodoIterado = nodoIterado->next;
  }
  return nullptr;
}

#endif
