#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <vector>
#include <queue>
#include <memory>
#include <pthread.h>
#include <semaphore.h>
#include "socket.h"
#include "LRUCache.h"

class ThreadPool {
private:
    std::vector<pthread_t> m_workers;
    std::queue<std::unique_ptr<Socket>> m_work_queue;
    
    pthread_mutex_t m_queue_mutex;
    sem_t m_job_count_sem;
    std::shared_ptr<LRUCache> m_cache;
    bool m_stop;
    static void* WorkerRoutine(void* arg);
    void HandleClient(std::unique_ptr<Socket> client);

public:
    ThreadPool(int threadCount, std::shared_ptr<LRUCache> cache);
    ~ThreadPool();
    void Initialize();
    void Enqueue(std::unique_ptr<Socket> client);
};

#endif