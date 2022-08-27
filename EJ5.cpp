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

class Cena : public Monitor {
  private:
    int state[N];
    Condition* dormido[N];
    int i;
  public:
    Cena();
    ~Cena();
    void take(int);
    void drop(int);
    void eval(int);
};
Cena::Cena(){
    for(i = 0; i < N; i++){
    dormido[i] = new Condition();
        state[i] = -1;
    }
}
Cena::~Cena(){
    for (i = 0; i < N; i++)
        delete dormido[i];
}
void Cena::eval(int k) {
    wait_m_mutex();
    if (((state[k] == 0) && state[(k+N-1) % N]) != 1 && (state[(k+1) % N] != 1)) {
        state[k] = 1;
        resume(dormido[k]);
    }
    if (get_m_next_count() > 0)
        signal_m_next();
    else 
        signal_m_mutex();
}
void Cena::take(int i) {
    wait_m_mutex();
    state[i] = 0;
    eval(i);
    if (state[i] != 1)
        delay(dormido[i]);
    if (get_m_next_count() > 0)
        signal_m_next();
    else 
        signal_m_mutex();
}
void Cena::drop(int i){
    wait_m_mutex();
    state[i] = -1;
    eval((i-1 + N) % N);
    eval((i+1) % N);
    if (get_m_next_count() > 0)
        signal_m_next();
    else 
        signal_m_mutex();
}

Cena* rd = new Cena();

void filosofo (int i) {
    while(1) {
        cout << "Piensa : " << i <<endl;
        rd->take(i);
        cout << "Come : " << i <<endl;
        rd->drop(i);
    }
}

int main () {
    ios::sync_with_stdio(false), cout.tie(nullptr);
    short i = 0;
    thread Fil[N];
    for (i = 0; i < N; i++)
        Fil[i] = thread(filosofo,i);
        
    for (i = 0; i < N; i++)
        Fil[i].join();

    while(1) NULL;
    delete rd;
    return 0;
}