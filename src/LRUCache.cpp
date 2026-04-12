#include <list>
#include <unordered_map>
#include <string>
#include <vector>
#include <pthread.h>
#include <iostream>
#include "LRUCache.h"

LRUCache::LRUCache(size_t c) : capacity(c) {
    pthread_mutex_init(&cache_mutex, nullptr);
}

LRUCache::~LRUCache() {
    pthread_mutex_destroy(&cache_mutex);
}

std::vector<char> LRUCache::get(const std::string& path) {
    pthread_mutex_lock(&cache_mutex);
    auto it = map.find(path);
    
    if (it == map.end()) {
	//Thought it'd be nice to help visualize a cache miss and hit in our demonstration. Comment out for serious usage
	std::cout<<"Cache miss on: "<<path<<" fetching from disk!"<<std::endl;
        pthread_mutex_unlock(&cache_mutex);
        return {}; // empty vector == cache miss
    }
    //Same as the cache miss comment, but on cache hits
    std::cout<<"Cache hit on: "<<path<<" fetching from cache!"<<std::endl;
    dll.splice(dll.begin(), dll, it->second);
    std::vector<char> data = it->second->content;
    
    pthread_mutex_unlock(&cache_mutex);
    return data;
}

void LRUCache::put(const std::string& path, const std::vector<char>& content) {
    pthread_mutex_lock(&cache_mutex);
    auto it = map.find(path);
    if (it != map.end()) {
        it->second->content = content;
        dll.splice(dll.begin(), dll, it->second);
    } 
    else {
	//evicts the LRU if our data doesnt exist
        if (map.size() >= capacity) { 
            std::string oldPath = dll.back().path;
            map.erase(oldPath);
            dll.pop_back(); 
        }
        
        dll.push_front({path, content});
        map[path] = dll.begin();
    }
    
    pthread_mutex_unlock(&cache_mutex);
}