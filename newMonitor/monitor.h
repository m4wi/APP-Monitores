#ifndef MONITOR_H
#define MONITOR_H

#include <semaphore.h>
#include <functional>
class Semaphore {
    private:
        sem_t sem;
    public:
        Semaphore(int value) {
            sem_init(&sem,0,value);
        }
        ~Semaphore() {
            sem_destroy(&sem);
        }
        void realease() {
            sem_post(&sem);
        }
        void acquire() {
            sem_wait(&sem);
        }
};
class Monitor {
    friend class Condition;
    private:
        Semaphore monitorMutex{1};
        Semaphore monitorNext{0};
        int monitorCount{0};
    public:
        void run(std::function<void()> func) {
            monitorMutex.acquire();
            func();
            if (monitorCount > 0)
                monitorNext.realease();
            else
                monitorMutex.realease();
        }
};

class Condition {
    private:
        Semaphore conditionLock{0};
        int conditionCount{0};
        Monitor* father = NULL;
    public:
        Condition(const Condition &) = delete;
        Condition(Monitor* _father){
            father = _father;
        };
        void resume() {
            if (conditionCount > 0) {
                ++father->monitorCount;
                conditionLock.realease();
                father->monitorNext.acquire();
                --father->monitorCount;
            }
        }
        void delay() {
            ++conditionCount;
            if (father->monitorCount > 0)
                father->monitorNext.realease();
            else
                father->monitorMutex.realease();
            conditionLock.acquire();
            --conditionCount;
        }
        bool empty() {
            return (conditionCount != 0);
        }
};
#endif