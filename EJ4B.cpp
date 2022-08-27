/*
  Tema: "Monitores - Productor-Consumidor"
  Curso: "Algoritmos y Programacion Paralela"
  Fecha: 23/08/22
*/
#include <iostream>
#include <unistd.h>
#include <thread>
#include "ink.h"
#define N 5

using namespace std;

class PEsc : public Monitor {
    private:
    int lec;
    int writing;
    Condition* writer;
    Condition* reader;
  public:
    PEsc();
    ~PEsc();
    void openReading();
    void closeReading();
    void openWriting();
    void closeWriting();
};
PEsc::PEsc() {
  lec = 0;
  writing = 0;
  writer = new Condition();
  reader = new Condition();  
}
PEsc::~PEsc() {
  delete writer;
  delete reader;
}
void PEsc::openReading() {
  wait_m_mutex();
  /*   */
  if (get_m_next_count() > 0) {
    signal_m_next();
  }
  else signal_m_mutex();
}
void PEsc::closeReading() {
  wait_m_mutex();
  /*   */
  if (get_m_next_count() > 0) {
    signal_m_next();
  }
  else signal_m_mutex();
}
void PEsc::openWriting() {
  wait_m_mutex();
  /*   */
  if (get_m_next_count() > 0) {
    signal_m_next();
  }
  else signal_m_mutex();
}
void PEsc::closeWriting(){
  wait_m_mutex();
  /*   */
  if (get_m_next_count() > 0) {
    signal_m_next();
  }
  else signal_m_mutex();
}
//-------[HILOS]----------------

void lector (int i) {

}
void escritor (int i) {

}
//-----------------------------
int main () {
  thread Lector[N];
  thread Escritor[N];
  short i = 0;
  for (i = 0; i < N ; i++) {
    Lector[i] = thread(lector,i);
    Escritor[i] = thread(escritor,i);
  }
  return 0;
}
