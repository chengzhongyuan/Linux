#ifndef UDP_SERVER_HPP
#define UDP_SERVER_HPP

#include <string>

class UdpServer {
public:
    UdpServer(int port);
    void run();

private:
    int sockfd;
    int port;
};

#endif