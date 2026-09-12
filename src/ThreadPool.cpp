#include "ThreadPool.h"
#include "HttpParser.h"
#include <iostream>
#include <unistd.h>
#include <sys/socket.h>
#include <fstream>
#include "Utils.h"
#include <chrono>

ThreadPool::ThreadPool(int threadCount, std::shared_ptr<LRUCache> cache) 
    : m_workers(threadCount), m_cache(cache), m_stop(false) {
    pthread_mutex_init(&m_queue_mutex, nullptr);
    sem_init(&m_job_count_sem, 0, 0); 
}

void ThreadPool::Initialize() {
    for (size_t i = 0; i < m_workers.size(); ++i) {
        pthread_create(&m_workers[i], nullptr, WorkerRoutine, this);
    }
}

ThreadPool::~ThreadPool() {
    m_stop = true;
    for (size_t i = 0; i < m_workers.size(); ++i) {
        sem_post(&m_job_count_sem);
    }
    for (pthread_t& thread : m_workers) {
        pthread_join(thread, nullptr);
    }
    pthread_mutex_destroy(&m_queue_mutex);
    sem_destroy(&m_job_count_sem);
}

void ThreadPool::Enqueue(std::unique_ptr<Socket> client) {
    pthread_mutex_lock(&m_queue_mutex);
    m_work_queue.push(std::move(client));
    pthread_mutex_unlock(&m_queue_mutex);
    sem_post(&m_job_count_sem); //lets any waiting threads wake up to execute the next job
}

void* ThreadPool::WorkerRoutine(void* arg) {
    ThreadPool* pool = static_cast<ThreadPool*>(arg);
    while (true) {
        sem_wait(&pool->m_job_count_sem);
        if (pool->m_stop) break;

        std::unique_ptr<Socket> client;
        pthread_mutex_lock(&pool->m_queue_mutex);
        if (!pool->m_work_queue.empty()) {
            client = std::move(pool->m_work_queue.front());
            pool->m_work_queue.pop();
        }
        pthread_mutex_unlock(&pool->m_queue_mutex);

        if (client) {
            pool->HandleClient(std::move(client));
        }
    }
    return nullptr;
}

void ThreadPool::HandleClient(std::unique_ptr<Socket> client) {
    char buffer[4096] = {0};
    int fd = client->get_fd();
    ssize_t bytes_received = recv(fd, buffer, sizeof(buffer) - 1, 0);
    if (bytes_received <= 0) return;

    HttpRequest req = HttpParser::Parse(std::string(buffer));
    if (!req.isValid || req.method != "GET") {
        std::string error = "HTTP/1.1 400 Bad Request\r\nConnection: close\r\n\r\n";
        send(fd, error.c_str(), error.size(), 0);
        return;
    }
    auto start = std::chrono::high_resolution_clock::now();
    std::vector<char> content = m_cache->get(req.path);
    bool isCacheHit = !content.empty(); //checks if its a cache miss or hit
    if (!isCacheHit) {
        std::string fullPath = "./public" + req.path;
        content = Utils::LoadFile(fullPath);
        if (!content.empty()) {
            m_cache->put(req.path, content);
        }
    }
    auto finish = std::chrono::high_resolution_clock::now();
    auto time_taken = std::chrono::duration_cast<std::chrono::microseconds>(finish-start).count();
    std::cout<<"Request at: ["<<Utils::GetCurrentTime()<<"] "<< (isCacheHit ? "Cache hit" : "Cache miss") << " Time taken: "<<time_taken<<std::endl;
    //just trying to decode the type of the file for the browser
    std::string contentType = "text/plain";
    if (req.path.find(".html") != std::string::npos) contentType = "text/html";
    else if (req.path.find(".png") != std::string::npos) contentType = "image/png";
    else if (req.path.find(".jpg") != std::string::npos || req.path.find(".jpeg") != std::string::npos) contentType = "image/jpeg";
    else if (req.path.find(".css") != std::string::npos) contentType = "text/css";
    else if (req.path.find(".mp4") != std::string::npos) contentType = "video/mp4"; // Forgot to push this from my local machine.
    std::string header;
    if (!content.empty()) {
        header = "HTTP/1.1 200 OK\r\n"
                 "Content-Type: " + contentType + "\r\n" 
                 "Content-Length: " + std::to_string(content.size()) + "\r\n"
                 "Connection: close\r\n\r\n";
    } else {
        std::string not_found = "<h1>404 Not Found</h1>";
        content.assign(not_found.begin(), not_found.end());
        header = "HTTP/1.1 404 Not Found\r\n"
                 "Content-Type: text/html\r\n"
                 "Content-Length: " + std::to_string(content.size()) + "\r\n"
                 "Connection: close\r\n\r\n";
    }

    send(fd, header.c_str(), header.size(), 0);
    send(fd, content.data(), content.size(), 0);
}
