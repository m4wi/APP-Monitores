/*
  Tema: "Monitores"
  Curso: "Algoritmos y Programacion Paralela"
  Fecha: 23/08/22
*/

#include <semaphore.h>


class Semaphore {
  private:
    sem_t sem;
  public:
    Semaphore();
    ~Semaphore();
    void initSemaphore(int);
    void acquire(); //wait
    void realease(); //signal
};

class Condition {
  friend class Monitor;
  private:
    Semaphore m_x_sem; //para la cola de la varable de condicion (init: 0)
    int m_x_count; //numero de procesos bloqueados en la variable condicion (init:0)
  public:
    Condition(const Condition &) = delete; //desabilitamos el constructor copia
    Condition();
    ~Condition();
};

//Estructura de los procedimientos del monitor
/*
  1   wait(m_mutex)
  2   # cuerpo_procedimiento
  3   if m_next_count > 0:
  4     # Al salir del monitor se debe desbloquear en primer lugar a los
  5     # procesos de la cola de cortesía si los hay.
  6     signal(m_next)
  7     # Fijate que no hay signal(m_mutex) aquí porque hay cesión de la
  8     # exclusión mutua del proceso que sale al proceso que entra desde la
  9     # cola de cortesía. Dicho de otro modo, las llamadas a wait y signal
  10    # han de estar equilibradas.
  11  else:
  12    signal(m_mutex)
*/

class Monitor {
  private:
    Semaphore m_mutex; //inicializado en 1 para la entrada al monitor
    Semaphore m_next; // para la cola de cortesia inicializado en 0
    int m_next_count; // numero de proceso bloqueados en la cola de cortesia (init 0)

  public:
    /*
      procedimientos...
    */
    Monitor();
    ~Monitor();
    bool empty(Condition*);
    void delay(Condition*);
    void resume(Condition*);
    void signal_m_mutex();
    void wait_m_mutex();
    void signal_m_next();
    void wait_m_next();
    int get_m_next_count();
};
