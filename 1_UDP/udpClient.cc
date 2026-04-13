#include "udpClient.hpp"
#include <iostream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <cstdio>    // C++·ç¸ñ
#include <cstdlib>
UdpClient::UdpClient(const std::string& serverIp, int port)
    : serverIp(serverIp), port(port) {

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("socket");
        exit(1);
    }
}

void UdpClient::run() {
    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    inet_pton(AF_INET, serverIp.c_str(), &serverAddr.sin_addr);

    char buffer[1024];

    while (true) {
        std::cout << "Input: ";
        std::cin.getline(buffer, sizeof(buffer));

        sendto(sockfd, buffer, strlen(buffer), 0,
               (sockaddr*)&serverAddr, sizeof(serverAddr));

        ssize_t n = recvfrom(sockfd, buffer, sizeof(buffer) - 1, 0, nullptr, nullptr);
        if (n > 0) {
            buffer[n] = '\0';
            std::cout << "Echo: " << buffer << std::endl;
        }
    }
}

int main() {
    UdpClient client("127.0.0.1", 8888);
    client.run();
    return 0;
}