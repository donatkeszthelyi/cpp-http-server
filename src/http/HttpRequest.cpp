#include "http/HttpRequest.h"
#include <sstream>
#include <stdexcept>

HttpRequest::HttpRequest(const std::string &message) {
    size_t header_end = message.find("\r\n\r\n");

    if (header_end == std::string::npos) {
        throw std::runtime_error("Invalid HTTP request");
    }

    std::string header_sections = message.substr(0, header_end);

    body = message.substr(header_end + 4);

    std::istringstream stream(header_sections);
    std::string line;

    if (!std::getline(stream, line)) {
        throw std::runtime_error("Missing request line");
    }

    if (!line.empty() && line.back() == '\r') {
        line.pop_back();
    }

    std::istringstream request_line(line);

    if (!(request_line >> method >> target >> version)) {
        throw std::runtime_error("Invalid request line");
    }

    while (std::getline(stream, line)) {
        if (!line.empty() && line.back() == '\r') {
            line.pop_back();
        }

        if (!line.empty()) {
            headers.push_back(line);
        }
    }
}

std::string HttpRequest::get_method() { return method; }

std::string HttpRequest::get_target() { return target; };

std::string HttpRequest::get_version() { return version; };

std::vector<std::string> HttpRequest::get_headers() { return headers; };

std::string HttpRequest::get_body() { return body; };
