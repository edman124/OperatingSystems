#ifndef POOL_H_
#include <string>
#include <pthread.h>
#include <set>
#include <algorithm>
#include <deque>
#include <iostream>
#include <vector>
class Task {
public:
    std::string name;
    int stop;
    Task();
    virtual ~Task();

    virtual void Run() = 0;  // implemented by subclass
};

// struct ActionTask : Task {
//     void Run() override {}
// };

struct Info
{
    // int numThreads;
    std::deque<Task*> *buffer;
    pthread_mutex_t *lock;
    pthread_cond_t *data_ready;
    std::set<std::string> *finished;
    pthread_mutex_t *finishLock;
    std::vector<std::string> *action;
    pthread_barrier_t *pauseBarrier;
};

void *runTask(void *arg);

class ThreadPool {
public:
    // sem_t full_slots;
    // sem_t empty_slots;
    // sem_t mutex;
    pthread_mutex_t lock;
    pthread_cond_t data_ready;
    pthread_mutex_t finishLock;
    struct Info passInfo;
    pthread_t *taskThreads;
    pthread_barrier_t pauseBarrier;
    int threads;
    std::vector<std::string> action;
    std::set<std::string> finished;
    // std::vector<int> finished;
    std::deque<Task*> buffer;
    ThreadPool(int num_threads);

    // Submit a task with a particular name.
    void SubmitTask(const std::string &name, Task *task);
 
    // Wait for a task by name, if it hasn't been waited for yet. Only returns after the task is completed.
    void WaitForTask(const std::string &name);

    // Stop all threads. All tasks must have been waited for before calling this.
    // You may assume that SubmitTask() is not caled after this is called.
    void Stop();

    void Pause();

    void Resume();

    bool CancelTask(const std::string &name);
};
#endif
