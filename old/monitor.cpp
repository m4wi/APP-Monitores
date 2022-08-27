/*
  Tema: "Monitores"
  Curso: "Algoritmos y Programacion Paralela"
  Fecha: 23/08/22
*/
#include "old.h"

//-------------------------------------------
Semaphore::Semaphore(){
}
void Semaphore::initSemaphore (int value) {
  if( sem_init(&sem, 0, value) != 0)
    throw "sem_init: failed";
}
Semaphore::~Semaphore() {
  sem_destroy(&sem);
}
void Semaphore::acquire () { // -> wait
  if (sem_wait(&sem) != 0)
    throw "sem_wait: failed";
}
void Semaphore::realease () { // -> signal
  if (sem_post(&sem) != 0)
    throw "sem_post: failed";
}
//---------------------------------------------

Condition::Condition() {
  m_x_sem.initSemaphore(0);
  m_x_count = 0;
}
Condition::~Condition(){
  m_x_sem.~Semaphore();
}

//---------------------------------------------
Monitor::Monitor(){
  m_mutex.initSemaphore(1);
  m_next.initSemaphore(0);
  m_next_count = 0;
}
Monitor::~Monitor(){
  m_mutex.~Semaphore();
  m_next.~Semaphore();
}
bool Monitor::empty(Condition* m_x){
  if(m_x->m_x_count > 0) return true;
  else return false;
}
void Monitor::resume(Condition* m_x){
  if (m_x->m_x_count > 0) {
    m_next_count = m_next_count + 1; // 
    m_x->m_x_sem.realease(); // libera al sgt proceso en la cola de la
    m_next.acquire(); // variable condicion y se bloquea en la cola de cortesia
    m_next_count = m_next_count - 1;
  }
}
void Monitor::delay(Condition* m_x){
  m_x->m_x_count++;
  /*Alguien puede ocupar el monitor: puede ser de la cola de cortesía
    primero o de la entrada normal al monitor*/
  if (m_next_count > 0) {
    m_next.realease();
    /*Libera al siguiente proceso: tienen
      preferencia los de la cola de cortesía*/
  }
  else {
    m_mutex.realease();
  }
  m_x->m_x_sem.acquire(); // Se bloquea en la variable de condición m_x
  m_x->m_x_count--;
}

void Monitor::signal_m_mutex(){
  m_mutex.realease();
}
void Monitor::wait_m_mutex(){
  m_mutex.acquire();
}
void Monitor::signal_m_next(){
  m_next.realease();
}
void Monitor::wait_m_next(){
  m_next.acquire();
}
int Monitor::get_m_next_count(){
  return m_next_count;
}
//---------------------------------------------
