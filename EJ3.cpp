/*
  Tema: "Monitores - Productor-Consumidor"
  Curso: "Algoritmos y Programacion Paralela"
  Fecha: 23/08/22
*/

#include <iostream>
#include <unistd.h>
#include <thread>
#include "ink.h"

using namespace std;

class Buffer : public Monitor {
    private:
        Condition* clleno;
        Condition* cvacio;
        int queue[10];
        int frente,final,slots;
    public:
        Buffer();
        ~Buffer();
        void push(int);
        void pop(int *);
};

Buffer::Buffer() : Monitor() {
    frente = 0;
    final = -1;
    slots = 0;
    clleno = new Condition();
    cvacio = new Condition();
}
Buffer::~Buffer() {
    delete clleno;
    delete cvacio;
}

void Buffer::push(int x) {
    wait_m_mutex();
    if (slots == 10) {
        delay(clleno);
    }
    final = (final + 1) % 10;
    queue[final] = x;
    slots++;
    cout << "# Producer push : "<<x<<endl;
    resume(cvacio);
    if (get_m_next_count() > 0) {
        signal_m_next();
    }
    else signal_m_mutex();
}
void Buffer::pop(int* x){
    wait_m_mutex();
    if (slots == 0){
        delay(cvacio);
    }
    *x = queue[frente];
    frente = (frente + 1) % 10;
    slots--;
    resume(clleno);
    if (get_m_next_count() > 0) {
        signal_m_next();
    }
    else signal_m_mutex();    
}

void producerx(Buffer* y){
    int value = 0;
    srand((unsigned)time(0));
    while (1) {
        value = rand()%210;
        y->push(value);
        sleep(1);
    }
}
void consumerx(Buffer* y){
    int value1 = 0;
    while (1) {
        y->pop(&value1);
        cout <<"$ Consumer pop : "<<value1<<endl;
        sleep(1);
    }
}

int main () {
    Buffer* y = new Buffer();
    thread Prcs[2];
    Prcs[0] = thread(producerx,y);
    Prcs[1] = thread(consumerx,y);

    Prcs[0].join();
    Prcs[1].join();

    delete y;
    return 0;
}