#include <iostream>
#include <thread>
#include <vector>
#include <cstring>
#include <unistd.h>
#include <netinet/in.h>

#define PORT 8888

void handle_client(int client_fd) {
    char buffer[1024];
    int n;

    while ((n = read(client_fd, buffer, sizeof(buffer)-1)) > 0) {
        buffer[n] = '\0';
        std::cout << "收到客户端消息: " << buffer << std::endl;
        write(client_fd, buffer, strlen(buffer)); // 回显
    }

    close(client_fd);
    std::cout << "客户端断开连接" << std::endl;
}

int main() {
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) { perror("socket"); return 1; }

    sockaddr_in server_addr{};
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    if (bind(server_fd, (sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("bind"); return 1;
    }

    listen(server_fd, 10);
    std::cout << "服务器启动，监听端口 " << PORT << "..." << std::endl;

    while (true) {
        sockaddr_in client_addr{};
        socklen_t client_len = sizeof(client_addr);
        int client_fd = accept(server_fd, (sockaddr*)&client_addr, &client_len);
        if (client_fd < 0) { perror("accept"); continue; }

        std::thread(handle_client, client_fd).detach(); // 创建线程处理客户端
    }

    close(server_fd);
    return 0;
}
