#ifndef SOCKET_H
#define SOCKET_H

#include <netinet/in.h>

class Socket {
private:
    int sock;
    int connection;
    int backlog;
    int listening;
    struct sockaddr_in address;

public:
    Socket(int portno);
    void test_connection(int con);
    bool bind_socket();
    bool connect_socket();
    bool start_listening(int bklg);

    struct sockaddr_in get_address();
    int get_sock();
    int get_connection();
};

#endif // SOCKET_H
