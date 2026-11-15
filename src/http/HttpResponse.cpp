#include "http/HttpResponse.h"

HttpResponse::HttpResponse(std::string version, std::string status_code, std::string status_message,
                           std::vector<std::string> headers, std::string body)
    : version(version), status_code(status_code), status_message(status_message), headers(headers),
      body(body) {};

std::string HttpResponse::get_version() { return version; };

std::string HttpResponse::get_status_code() { return status_code; };

std::string HttpResponse::get_status_message() { return status_message; };

std::vector<std::string> HttpResponse::get_headers() { return headers; };

std::string HttpResponse::get_body() { return body; };

std::string HttpResponse::serialize() {
    std::string response_string = "";
    response_string += version;
    response_string += " ";
    response_string += status_code;
    response_string += " ";
    response_string += status_message;
    response_string += "\r\n";
    for (std::string line : headers) {
        response_string += line;
        response_string += "\r\n";
    }
    response_string += "\r\n";
    response_string += body;

    return response_string;
}
