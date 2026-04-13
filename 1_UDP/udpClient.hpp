#ifndef UDP_CLIENT_HPP
#define UDP_CLIENT_HPP

#include <string>

class UdpClient {
public:
    UdpClient(const std::string& serverIp, int port);
    void run();

private:
    int sockfd;
    std::string serverIp;
    int port;
};

#endif