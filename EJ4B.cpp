/*
  Tema: "Monitores - Lector/Escritor caso2"
  Curso: "Algoritmos y Programacion Paralela"
  Fecha: 23/08/22
*/
#include <iostream>
#include <unistd.h>
#include <thread>
#include "monitor.h"
#define N 5

using namespace std;

Monitor PLEC;
int lec = 0;
bool writing = false;
Condition* writer = NULL; //escritor
Condition* reader = NULL; //lector



void openReading(){
	if (writing)
		reader->delay();
	++lec;
	reader->resume();
}

void closeReading(){
	--lec;
	if (lec == 0)
		writer->resume();
}
void openWriting(){
    if (lec != 0 || writing)
      	writer->delay();
    writing = true;
}
void closeWriting(){
  	writing = false;
  	if (!(reader->empty()))
  		reader->resume();
  	else
		writer->resume();
}

//-------[HILOS]----------------

void lector (int i) {
	//srand((unsigned)time(NULL));
	while (1) {
		PLEC.run([&](){
			openReading();
		});
		cout << "Leer Recurso : "<<i<<endl;
		//this_thread::sleep_for(chrono::milliseconds(rand()%10));
		PLEC.run([&](){
			closeReading();
		});
	}
}
void escritor (int i) {
	//srand((unsigned)time(NULL));
	while (1) {
		PLEC.run([&](){
			openWriting();
		});
		cout << "Escribir Recurso : "<<i<<endl;
		//this_thread::sleep_for(chrono::milliseconds(rand()%50));
		PLEC.run([&](){
			closeWriting();
		});
	}
}

//-----------------------------
int main () {
	ios::sync_with_stdio(false), cout.tie(nullptr);
	writer = new Condition(&PLEC);
	reader = new Condition(&PLEC);

	thread Lector[2];
	thread Escritor[N];
    
	short i = 0;

	for (i = 0; i < 2 ; i++)
		Lector[i] = thread(lector,i);

	for (i = 0; i < N ; i++)
		Escritor[i] = thread(escritor,i);

    for (i = 0; i < N ; i++)
		if (Escritor[i].joinable()) Escritor[i].join();

	for (i = 0; i < 2 ; i++)
		if (Lector[i].joinable()) Lector[i].join();
  return 0;
}
