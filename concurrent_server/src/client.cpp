#include <iostream>
#include <cstring>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 8888

int main() {
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) { perror("socket"); return 1; }

    sockaddr_in server_addr{};
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (connect(sockfd, (sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("connect"); return 1;
    }

    std::cout << "连接服务器成功！输入消息并回车发送。" << std::endl;

    char buffer[1024];
    while (fgets(buffer, sizeof(buffer), stdin)) {
        write(sockfd, buffer, strlen(buffer));
        int n = read(sockfd, buffer, sizeof(buffer)-1);
        buffer[n] = '\0';
        std::cout << "服务器回显: " << buffer;
    }

    close(sockfd);
    return 0;
}
