#ifndef HTTPRESPONSE_H
#define HTTPRESPONSE_H

#include <string>
#include <vector>

class HttpResponse {
private:
    std::string version;
    std::string status_code;
    std::string status_message;
    std::vector<std::string> headers;
    std::string body;

public:
    HttpResponse(std::string version, std::string status_code, std::string status_message,
                 std::vector<std::string> headers, std::string body);

    std::string get_version();
    std::string get_status_code();
    std::string get_status_message();
    std::vector<std::string> get_headers();
    std::string get_body();
    std::string serialize();
};

#endif // HTTPRESPONSE_H
