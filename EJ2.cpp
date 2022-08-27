/*
  Tema: "Monitores - Semaforo Binario"
  Curso: "Algoritmos y Programacion Paralela"
  Fecha: 23/08/22
*/

#include <iostream>
#include <unistd.h>
#include <thread>
#include "ink.h"

using namespace std;

class BinarySemaphore : public Monitor {
  private:
    bool sem_b;
    Condition* c;
  public:
    BinarySemaphore();
    ~BinarySemaphore();
    //procedimientos
    void wait();
    void signal();
};

BinarySemaphore::BinarySemaphore() : Monitor() {
  sem_b = false;
  c = new Condition();
}
BinarySemaphore::~BinarySemaphore(){
  delete c;
}
void BinarySemaphore::wait() {
  wait_m_mutex();
  if (sem_b == true) {
    delay(c);
    sem_b = true;
  }
  if (get_m_next_count() > 0) {
    signal_m_next();
  }
  else signal_m_mutex();
}
void BinarySemaphore::signal() {
  wait_m_mutex();
  if (empty(c) == false) resume(c);
  else sem_b = false;
  if (get_m_next_count() > 0) {
    signal_m_next();
  }
  else signal_m_mutex();
}

//---------------------------------------------->
int i = 0;

void P1(BinarySemaphore* s) {
  while(i < 25){
    sleep(1);
    cout << "a\n";
    s->signal();
    sleep(1);
    cout << "b\n";
    i++;
  }
}

void P2(BinarySemaphore *s) {
  while(i < 25){
    sleep(1);
    cout << "c\n";
    s->wait();
    sleep(1);
    cout << "d\n";
    i++;
  }
}
//---------------------------------------------->

int main () {
  BinarySemaphore* s;
  s = new BinarySemaphore();
  
  thread P[2];
  P[0] = thread(P1,s);
  P[1] = thread(P2,s);

  P[0].join();
  P[1].join();
  
  delete s;
  return 0;
}
