/*
  Tema: "Monitores - Productor-Consumidor"
  Curso: "Algoritmos y Programacion Paralela"
  Fecha: 23/08/22
*/
#include <iostream>
#include <unistd.h>
#include <thread>
#include "monitor.h"
#define N 5
using namespace std;

/* */
Monitor cena;
int state[5];
Condition* dormido[N];

void eval(int k) {
    if (((state[k] == 0) && state[(k+N-1) % N]) != 1 && (state[(k+1) % N] != 1)) {
        state[k] = 1;
        dormido[k]->resume();
    }
}
void take(int i) {
    state[i] = 0;
    eval(i);
    if (state[i] != 1)
        dormido[i]->delay();
}
void drop(int i){
    state[i] = -1;
    eval((i-1 + N) % N);
    eval((i+1) % N);
}


void filosofo (int i) {
    while(true) {
        cout << "Piensa : " << i <<endl;
        cena.run([&](){
            take(i);
        });
        cout << "Come : " << i <<endl;
        cena.run([&](){
            drop(i);
        });
    }
}

int main () {
    ios::sync_with_stdio(false), cout.tie(nullptr);
    short i = 0;

    for (i = 0; i < N ; i++) {
        dormido[i] = new Condition(&cena);
    }

    thread Fil[N];
    for (i = 0; i < N; i++)
        Fil[i] = thread(filosofo,i);

    while(1) NULL;
    return 0;
}