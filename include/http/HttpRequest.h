#ifndef HTTPREQUEST_H
#define HTTPREQUEST_H

#include <string>
#include <vector>

class HttpRequest {
private:
    std::string method;
    std::string target;
    std::string version;
    std::vector<std::string> headers;
    std::string body;

public:
    HttpRequest(const std::string &message);

    std::string get_method();
    std::string get_target();
    std::string get_version();
    std::vector<std::string> get_headers();
    std::string get_body();
};

#endif // HTTPREQUEST_H
