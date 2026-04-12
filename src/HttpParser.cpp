#include "HttpParser.h"
#include <sstream>

HttpRequest HttpParser::Parse(const std::string& rawRequest) {
    HttpRequest req;
    std::stringstream ss(rawRequest);
    
    // extracts the first two words: METHOD and PATH (e.g., "GET /index.html")
    if (!(ss >> req.method >> req.path)) {
        return req; // isValid will be false
    }

    //prevents ".." to prevent directory traversal
    if (req.path.find("..") != std::string::npos) {
        return req; 
    }

    // routes the root directory to index.html
    if (req.path == "/") {
        req.path = "/index.html";
    }

    req.isValid = true;
    return req;
}