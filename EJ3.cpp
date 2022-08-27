/*
  Tema: "Monitores - Productor-Consumidor"
  Curso: "Algoritmos y Programacion Paralela"
  Fecha: 23/08/22
*/

#include <iostream>
#include <unistd.h>
#include <thread>
#include "monitor.h"
#define MAXSIZE 10
using namespace std;

Monitor Buffer;
Condition* clleno = NULL;
Condition* cvacio = NULL;
int queue[MAXSIZE];
int frente = 0,final = -1,slots = 0;


void push(int x) {
    if (slots == 10)
        clleno->delay();
    final = (final + 1) % 10;
    queue[final] = x;
    slots++;
    cout << "# Producer push : "<<x<<endl;
    cvacio->resume();
}
void pop(int* x){
    if (slots == 0)
        cvacio->delay();
    *x = queue[frente];
    frente = (frente + 1) % 10;
    slots--;
    clleno->resume();
}

void producerx(){
    int value = 0;
    srand((unsigned)time(0));
    while (1) {
        value = rand()%210;
        Buffer.run([&](){
            push(value);
        });
        sleep(1);
    }
}
void consumerx(){
    int value1 = 0;
    while (1) {
        Buffer.run([&](){
            pop(&value1);
        });
        cout <<"$ Consumer pop : "<<value1<<endl;
        sleep(1);
    }
}

int main () {
    clleno = new Condition(&Buffer);
    cvacio = new Condition(&Buffer);
    thread Prcs[2];
    Prcs[0] = thread(producerx);
    Prcs[1] = thread(consumerx);

    Prcs[0].join();
    Prcs[1].join();

    return 0;
}