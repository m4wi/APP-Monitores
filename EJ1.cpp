#include <iostream>
#include <unistd.h>
#include <thread>
#include "monitor.h"

using namespace std;
// #-----------------------------------#
Monitor Incremento;
int counter = 0;

void inc() {
    counter += 2;
}
void viewCount() {
    cout << "C : " << counter <<endl;
}

void incrementar() {
    while (true) {
        Incremento.run([&](){
            inc();
        });
        sleep(1);
    }
}
void imprimir() {
    while (true) {
        Incremento.run([&](){
            viewCount();
        });
        sleep(1);
    }
}
int main () {
  thread Prcs[2];
  Prcs[0] = thread(incrementar);
  Prcs[1] = thread(imprimir);
  Prcs[0].join();
  Prcs[1].join();
  return 0;
}