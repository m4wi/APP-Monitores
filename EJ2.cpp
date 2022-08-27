/*
  Tema: "Monitores - Semaforo Binario"
  Curso: "Algoritmos y Programacion Paralela"
  Fecha: 23/08/22
*/
#include <iostream>
#include <unistd.h>
#include <thread>
#include "monitor.h"

using namespace std;

Monitor xs;
bool sem_b = false;
Condition* c = NULL;

void wait() {
  if (sem_b == true) {
    c->delay();
    sem_b = true;
  }
}
void signal() {
  if (c->empty() == false)
    c->resume();
  else sem_b = false;
}

//---------------------------------------------->
int i = 0;

void P1() {
  while (true) {
    sleep(1);
    cout << "a\n";
    xs.run([&](){
        signal();
    });
    sleep(1);
    cout << "b\n";
  }
}
void P2() {
  while (true) {
    sleep(1);
    cout << "c\n";
    xs.run([&](){
        wait();
    });
    sleep(1);
    cout << "d\n";
  }
}
//---------------------------------------------->

int main () {
    c = new Condition(&xs);
    thread P[2];
    P[0] = thread(P1);
    P[1] = thread(P2);

    while(1) NULL;
    delete c;
  return 0;
}
