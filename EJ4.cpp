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

class PLec : public Monitor {
	private:
		int lec;
		int writing;
		Condition* writer;
		Condition* reader;
	public:
		PLec();
		~PLec();
		void openReading();
		void closeReading();
		void openWriting();
		void closeWriting();
};
PLec::PLec() {
	lec = 0;
	writing = 0;
	writer = new Condition();
	reader = new Condition();
}
PLec::~PLec() {
  	delete writer;
  	delete reader;
}
PLec x;

void PLec::openReading(){
	wait_m_mutex();
	//------- procedure code -------
		if (writing = 1)
			delay(reader);
		++lec;
		resume(reader);
	//------------------------------
	if (get_m_next_count() > 0) {
		signal_m_next();
	}
	else signal_m_mutex();
}

void PLec::closeReading(){
	wait_m_mutex();
	//------- procedure code -------
		--lec;
		if (lec == 0)
			resume(writer);
	//------------------------------
	if (get_m_next_count() > 0) {
		signal_m_next();
	}
	else signal_m_mutex();
	}

void PLec::openWriting(){
  wait_m_mutex();
  //------- procedure code -------
    if (lec != 0 || writing == 1)
      delay(writer);
    writing = 1;
  //------------------------------
  if (get_m_next_count() > 0) {
    signal_m_next();
  }
  else signal_m_mutex();
}
void PLec::closeWriting(){
  	wait_m_mutex();
  	//------- procedure code -------
  	writing = 0;
  	if (empty(writer) == true)
  		resume(reader);
  	else
		resume(writer);  
  	//------------------------------
  	if (get_m_next_count() > 0) {
    	signal_m_next();
  	}
  	else signal_m_mutex();
}

//-------[HILOS]----------------

void lector (int i) {
  while (1) {
    x.openReading();
    cout << "Leer Recurso : "<<i<<endl;
    x.closeReading();
  }
}
void escritor (int i) {
  while (1) {
    x.openWriting();
    cout << "Escribir Recurso : "<<i<<endl;
    x.closeWriting();
  }
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
	while(1) NULL;

	return 0;
}