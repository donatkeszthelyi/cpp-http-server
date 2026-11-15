#ifndef SERVER_H
#define SERVER_H

#include "server/Socket.h"
#include <netinet/in.h>
#include <string>
#include <vector>

class Socket;
class HttpRequest;
class HttpResponse;

class Server {
private:
    Socket socket;
    int new_socket;
    int accept_connection();
    int handle();
    HttpResponse handle_get(HttpRequest &request);
    int respond(HttpResponse response);
    std::vector<std::string> targets;

public:
    Server(int portno, int bklg);

    void start();
    void add_target(std::string target);
    void remove_target(std::string target);
};

#endif // SERVER_H
