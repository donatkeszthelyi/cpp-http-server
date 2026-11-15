#include "server/Socket.h"
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>

Socket::Socket(int portno) {
    sock = socket(AF_INET, SOCK_STREAM, 0);

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(portno);
}

void Socket::test_connection(int con) {
    if (con < 0) {
        perror("Connection failed.");
        exit(EXIT_FAILURE);
    }
}

bool Socket::bind_socket() {
    connection = bind(sock, (struct sockaddr *)&address, sizeof(address));
    return connection >= 0;
}

bool Socket::connect_socket() {
    connection = connect(sock, (struct sockaddr *)&address, sizeof(address));
    return connection >= 0;
}

bool Socket::start_listening(int bklg) {
    backlog = bklg;
    listening = listen(sock, backlog);
    return listening >= 0;
}

struct sockaddr_in Socket::get_address() { return address; }

int Socket::get_sock() { return sock; }

int Socket::get_connection() { return connection; }
