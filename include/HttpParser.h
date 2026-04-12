#ifndef HTTP_PARSER_H
#define HTTP_PARSER_H

#include <string>

struct HttpRequest {
    std::string method;
    std::string path;
    bool isValid = false;
};

class HttpParser {
public:
    // handles parsing the raw HTTP text and it also returns a structured http request for the browser
    static HttpRequest Parse(const std::string& rawRequest);
};

#endif