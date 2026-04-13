#include "udpServer.hpp"
#include <iostream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <cstdio>    // C++风格
#include <cstdlib>
// 可以直接初始化创建端口号
UdpServer::UdpServer(int port) : port(port) {
    /* * **DOMAIN** ? 协议域

        * `AF_INET` → IPv4
        * `AF_INET6` → IPv6

        **TYPE** ? 套接字类型

        * `SOCK_STREAM` → TCP
        * `SOCK_DGRAM` → UDP

        **PROTOCOL** ? 具体协议

        * 一般填 `0`（系统自动选） */
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    // 创建失败的情况下返回报错
    if (sockfd < 0) {
        perror("socket");
        exit(1);
    }

    /* Structure describing an Internet socket address.  */
    // struct sockaddr_in
    // {
    //     __SOCKADDR_COMMON (sin_);
    //     in_port_t sin_port;			/* 端口号  */
    //     struct in_addr sin_addr;		/* IP地址  */

    //     /* Pad to size of `struct sockaddr'.  */
    //     unsigned char sin_zero[sizeof (struct sockaddr) -
    //             __SOCKADDR_COMMON_SIZE -
    //             sizeof (in_port_t) -
    //             sizeof (struct in_addr)];
    // };
    
    /* 定义一个 sockaddr_in 结构体变量，并把所有成员初始化为 0 */
    sockaddr_in addr{};
    addr.sin_family = AF_INET;         // 设置地址类型 
    addr.sin_addr.s_addr = INADDR_ANY; // 设置IP地址，这个意思是监听本机的所有网卡的IP
    addr.sin_port = htons(port);       // 设置端口号 

    /* 把 socket 和“IP地址 + 端口号”绑定起来 */
    if (bind(sockfd, (sockaddr*)&addr, sizeof(addr)) < 0) {
        perror("bind");
        exit(1);
    }

    std::cout << "UDP Server started on port " << port << std::endl;
}

// 服务器上线之后进行阻塞式等待
void UdpServer::run() {
    char buffer[1024];

    while (true) {
        sockaddr_in clientAddr{};
        socklen_t len = sizeof(clientAddr);

        ssize_t n = recvfrom(sockfd, buffer, sizeof(buffer) - 1, 0,
                             (sockaddr*)&clientAddr, &len);

        if (n < 0) {
            perror("recvfrom");
            continue;
        }

        buffer[n] = '\0';
        std::cout << "Client: " << buffer << std::endl;

        // echo 回去
        sendto(sockfd, buffer, n, 0,
               (sockaddr*)&clientAddr, len);
    }
}

int main() {
    UdpServer server(8888);
    server.run();
    return 0;
}