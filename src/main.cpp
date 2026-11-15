#include "server/Server.h"
#include <iostream>

int main() {
    std::cout << "Starting..." << std::endl;

    Server s = Server(80, 5);

    s.start();
}
