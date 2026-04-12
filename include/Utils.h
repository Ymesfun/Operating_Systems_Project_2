#ifndef UTILS_H
#define UTILS_H

#include <vector>
#include <string>
#include <fstream>
#include <chrono>
#include <iomanip>
#include <sstream>

class Utils {
public:
    //Helper function for time logging
    static std::string GetCurrentTime() {
        auto now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
        std::stringstream ss;
        ss << std::put_time(std::localtime(&now), "%Y-%m-%d %H:%M:%S");
        return ss.str();
    }
    //Helper function for loading a file - Empty vector means file does not exist
    static std::vector<char> LoadFile(const std::string& filepath) {
        // goes to the end of a filestream to find the size, need to know how large for our buffer vector
        std::ifstream file(filepath, std::ios::binary | std::ios::ate);
        if (!file.is_open()) {
            return {};
        }
        std::streamsize size = file.tellg();
        file.seekg(0, std::ios::beg);
        std::vector<char> buffer(size);
        if (file.read(buffer.data(), size)) {
            return buffer;
        }
        
        return {};
    }
};

#endif