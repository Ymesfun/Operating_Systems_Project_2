#ifndef LRU_CACHE_H
#define LRU_CACHE_H

#include <list>
#include <unordered_map>
#include <string>
#include <vector>
#include <pthread.h>

struct CacheEntry {
    std::string path;
    std::vector<char> content;
};

class LRUCache {
private:
    std::list<CacheEntry> dll;
    

    std::unordered_map<std::string, std::list<CacheEntry>::iterator> map;
    
    int capacity;
    pthread_mutex_t cache_mutex;

public:
    explicit LRUCache(int c);
    ~LRUCache();

    std::vector<char> get(const std::string& path);
    void put(const std::string& path, const std::vector<char>& content);
};

#endif 