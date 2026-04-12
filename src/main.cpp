#include <iostream>
#include <memory>
#include "socket.h"
#include "ThreadPool.h"
#include "LRUCache.h"
#include <csignal>
#include <atomic>

std::atomic<bool> server_running(true); //std::atomic prevents copy constructor use

void signal_handler(int signal) {
    if (signal == SIGINT) {
        server_running = false;
        std::cout << "\n[SHUTDOWN] Signal received. Cleaning up..." << std::endl;
    }
}

int main() {
    std::signal(SIGINT, signal_handler);
    const int PORT = 8080;
    const size_t THREAD_COUNT = 4;
    const int CACHE_CAPACITY = 10;
    auto cache = std::make_shared<LRUCache>(CACHE_CAPACITY);
    ServerSocket server;
    server.Bind(PORT);
    server.Listen();
    if (server.get_fd() == -1) {
        std::cerr << "Fatal: Could not initialize server socket. Exiting." << std::endl;
        return 1; 
    }
    ThreadPool pool(THREAD_COUNT, cache);
    pool.Initialize();
    std::cout << "Web server is ready to take connections and is running on http://localhost:" << PORT << "..." << std::endl;
    while (server_running) {
        std::unique_ptr<Socket> client = server.Accept();
        if (client) {
            pool.Enqueue(std::move(client));
        }
    }
    return 0;
}