#include "server/Server.h"
#include "http/HttpRequest.h"
#include "http/HttpResponse.h"
#include "server/Socket.h"
#include <algorithm>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

Server::Server(int portno, int bklg) : socket(portno), targets({"/"}) {
    socket.bind_socket();
    socket.start_listening(bklg);
}

int Server::accept_connection() {
    struct sockaddr_in address = socket.get_address();
    socklen_t addrlen = sizeof(address);
    new_socket = accept(socket.get_sock(), (struct sockaddr *)&address, &addrlen);
    if (new_socket >= 0) {
        this->handle();
        return 1;
    } else {
        return 0;
    }
}

int Server::handle() {
    std::string message;
    char buffer[4096];

    size_t header_end = std::string::npos;
    size_t expected_size = 0;

    while (true) {
        ssize_t bytes = read(new_socket, buffer, sizeof(buffer));

        if (bytes > 0) {
            message.append(buffer, bytes);
        } else if (bytes == 0) {
            break;
        } else {
            if (errno == EINTR) {
                continue;
            }

            close(new_socket);
            new_socket = -1;
            return 0;
        }

        header_end = message.find("\r\n\r\n");

        if (header_end != std::string::npos) {
            size_t content_length_pos = message.find("Content-Length:");

            size_t body_length = 0;

            if (content_length_pos != std::string::npos && content_length_pos < header_end) {

                size_t start = content_length_pos + 15;
                size_t end = message.find("\r\n", start);

                body_length = std::stoul(message.substr(start, end - start));
            }

            expected_size = header_end + 4 + body_length;

            if (message.size() >= expected_size) {
                break;
            }
        }
    }

    if (message.empty()) {
        close(new_socket);
        new_socket = -1;
        return 0;
    }

    HttpRequest request(message);
    if (request.get_method() == "GET") {
        respond(handle_get(request));
    }

    return static_cast<int>(message.size());
}

int Server::respond(HttpResponse response) {
    std::string response_string = response.serialize();

    size_t total_written = 0;

    while (total_written < response_string.size()) {
        ssize_t bytes = write(new_socket, response_string.data() + total_written,
                              response_string.size() - total_written);

        if (bytes > 0) {
            total_written += bytes;
        } else if (bytes < 0 && errno == EINTR) {
            continue;
        } else {
            break;
        }
    }

    close(new_socket);
    new_socket = -1;

    return static_cast<int>(total_written);
}

void Server::start() {
    while (1) {
        this->accept_connection();
    }
}

void Server::add_target(std::string target) {
    std::vector<std::string>::iterator pos = find(targets.begin(), targets.end(), target);
    if (pos == targets.end()) {
        targets.push_back(target);
    }
}

void Server::remove_target(std::string target) {
    std::vector<std::string>::iterator pos = find(targets.begin(), targets.end(), target);
    if (pos != targets.end()) {
        targets.erase(pos);
    }
}

HttpResponse Server::handle_get(HttpRequest &request) {
    std::string version = "HTTP/1.1";
    std::string status_code;
    std::string status_message;
    std::vector<std::string> headers;
    std::string body;
    std::vector<std::string>::iterator pos =
        find(targets.begin(), targets.end(), request.get_target());

    if (pos != targets.end()) {
        status_code = "200";
        status_message = "OK";

        body = "<!DOCTYPE html><html><head></head><body><h1>HELLO</h1></body></html>";

        size_t length = body.size();
        std::string content_length = "Content-Length: ";
        content_length.append(std::to_string(length));

        headers = {"Content-Type: text/html; charset=utf-8", "Connection: close", content_length};
    } else {
        status_code = "404";
        status_message = "Not Found";

        body = "<!DOCTYPE html><html><head></head><body><h1>404</h1><p>Not "
               "Found</p></body></html>";

        size_t length = body.size();
        std::string content_length = "Content-Length: ";
        content_length.append(std::to_string(length));

        headers = {"Content-Type: text/html; charset=utf-8", "Connection: close", content_length};
    }

    HttpResponse response = HttpResponse(version, status_code, status_message, headers, body);

    return response;
}
