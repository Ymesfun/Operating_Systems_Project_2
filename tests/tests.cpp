#include <iostream>
#include <cassert>
#include <vector>
#include <string>
#include "HttpParser.h"
#include "LRUCache.h"
#include "Utils.h"
#include <sys/types.h>
#include <sys/socket.h>
#include "socket.h"
#include "ThreadPool.h"

#define RED_TEXT "\x1b[31m"
#define DEFAULT_TEXT "\x1b[0m"
#define GREEN_TEXT "\x1b[32m"

void resultPrint(bool b){
  if(b){
    std::cout<<GREEN_TEXT<<"[Test Passed!]"<<DEFAULT_TEXT<<std::endl;
  }else{
    std::cout<<RED_TEXT<<"[Test Failed.]"<<DEFAULT_TEXT<<std::endl;
  }
}

void testHttpParser() {
    std::cout << "Running HttpParser tests..." << std::endl;

    std::cout<<"Test set 1: Basic GET on /index.html"<<std::endl;
    
    HttpRequest req1 = HttpParser::Parse("GET /index.html HTTP/1.1");
    std::cout<<"Test: is filepath valid?"<<std::endl;
    resultPrint(req1.isValid == true);
    std::cout<<"Test: Is the path correct?"<<std::endl;
    resultPrint(req1.path == "/index.html");
    std::cout<<"Test: Is the type (GET) correct?"<<std::endl;
    resultPrint(req1.method == "GET");

    std::cout<<"Test set 2: Root routing"<<std::endl;
    HttpRequest req2 = HttpParser::Parse("GET / HTTP/1.1");
    resultPrint(req2.path == "/index.html");

    std::cout<<"Test: Can you use directory traversal to escape the server and access files on the VM?"<<std::endl;
    HttpRequest req3 = HttpParser::Parse("GET /../../etc/passwd HTTP/1.1");
    resultPrint(req3.isValid == false);
}

void testLRUCache() {
    std::cout << "Running LRUCache tests..." << std::endl;
    
    std::cout<<"Testing cache of 2 with 3 data elements."<<std::endl;
    LRUCache cache(2);
    std::vector<char> data1 = {'A'};
    std::vector<char> data2 = {'B'};
    std::vector<char> data3 = {'C'};

    cache.put("file1", data1);
    cache.put("file2", data2);
    std::cout<<"Test: cache hit on file 1:"<<std::endl;
    resultPrint(!cache.get("file1").empty());
    std::cout<<"Test: cache hit on file 2:"<<std::endl;
    resultPrint(!cache.get("file2").empty());

    // This should evict "file1" (since we just accessed file2)
    cache.put("file3", data3);
    std::cout<<"Test: Cache miss on file 1"<<std::endl;
    resultPrint(cache.get("file1").empty());
    std::cout<<"Test: Cache hit on file 3"<<std::endl;
    resultPrint(!cache.get("file3").empty());
}
void testThreadPoolIntegration() {
    std::cout << "Running ThreadPool Integration test..." << std::endl;

    auto cache = std::make_shared<LRUCache>(10);
    ThreadPool pool(2, cache);
    pool.Initialize();

    int sv[2]; // Pair of sockets
    if (socketpair(AF_UNIX, SOCK_STREAM, 0, sv) == -1) {
        perror("socketpair");
        return;
    }

    // sv[0] will be our "Client" (the test)
    // sv[1] will be our "Server" (handed to the pool)
    auto clientSocket = std::make_unique<Socket>(sv[1]);
    
    // Send a dummy HTTP request into the pipe
    std::string mockReq = "GET /index.html HTTP/1.1\r\n\r\n";
    send(sv[0], mockReq.c_str(), mockReq.size(), 0);

    // Enqueue the socket into the pool
    pool.Enqueue(std::move(clientSocket));

    // Wait a tiny bit for the worker thread to process
    usleep(100000); 

    // Try to read the response from the pool back at sv[0]
    char buffer[1024] = {0};
    ssize_t bytes = recv(sv[0], buffer, sizeof(buffer), 0);

    std::cout << "Test: Did the ThreadPool send a response?" << std::endl;
    resultPrint(bytes > 0);
    
    close(sv[0]);
}

int main() {
    testHttpParser();
    testLRUCache();
    testThreadPoolIntegration();
    std::cout << "\nTESTS FINISHED." << std::endl;
    return 0;
}
