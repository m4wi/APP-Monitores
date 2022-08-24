#include <iostream>
#include <unistd.h>
#include <thread>
#include "ink.h"

using namespace std;
// #-----------------------------------#
class Incremento : public Monitor {
  private:
    int count;
  public:
    Incremento();
    ~Incremento();
    void inc();
    void viewCount();
};
Incremento::Incremento() : Monitor() {
  count = 0;
}
Incremento::~Incremento(){}

void Incremento::inc() {
  wait_m_mutex();
  count += 2;
  if (get_m_next_count() > 0) {
    signal_m_next();
  }
  else signal_m_mutex();
}
void Incremento::viewCount(){
  wait_m_mutex();
  int value = count;
  cout << value <<endl;
  if (get_m_next_count() > 0) {
    signal_m_next();
  }
  else signal_m_mutex();
}

// #-----------------------------------#
void incrementar (Incremento* x) {
  while (1) {
    x->inc();
    sleep(1);
  }
}

void imprimir (Incremento* x) {
  while (1) {
    x->viewCount();
    sleep(1);
  }
}

int main () {
  Incremento* x = new Incremento();
  thread Prcs[2];
  Prcs[0] = thread(incrementar,x);
  Prcs[1] = thread(imprimir,x);

  Prcs[0].join();
  Prcs[1].join();
  delete x;
  return 0;
}