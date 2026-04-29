#include "tcpClient.hpp"
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
using namespace client;

// 构造函数初始化
TcpClient::TcpClient(const std::string& server_ip, uint16_t server_port)
    : _server_ip(server_ip), _server_port(server_port), _sockfd(-1)
{
}

// 析构函数安全退出
TcpClient::~TcpClient()
{
    if (_sockfd >= 0)
    {
        close(_sockfd);
    }
}

void TcpClient::InitAndConnect()
{
    // 1. 创建流式套接字 (SOCK_STREAM)
    _sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (_sockfd < 0)
    {
        perror("socket creation failed");
        exit(1);
    }

    // 2. 准备服务器的地址面单
    struct sockaddr_in server_addr{};
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(_server_port);
    
    // 使用 inet_pton 将字符串 IP 转换为底层二进制数字
    if (inet_pton(AF_INET, _server_ip.c_str(), &server_addr.sin_addr) <= 0)
    {
        std::cerr << "无效的 IP 地址格式！" << std::endl;
        exit(1);
    }

    // 3. 发起连接 (TCP 客户端特有的底层调用！)
    // 这行代码底层会触发著名的“TCP 三次握手”
    std::cout << "正在连接到服务器 " << _server_ip << ":" << _server_port << " ..." << std::endl;
    if (connect(_sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0)
    {
        perror("连接服务器失败 (connect failed)");
        exit(1);
    }

    std::cout << "🎉 成功连接到服务器！可以开始聊天了。" << std::endl;
}

void TcpClient::StartChat()
{
    std::string message;
    char buffer[1024];

    while (true)
    {
        std::cout << "(客户端) 请输入消息 > ";
        std::getline(std::cin, message);

        // 如果输入 quit 或 exit，主动退出聊天
        if (message == "quit" || message == "exit")
        {
            std::cout << "正在断开连接..." << std::endl;
            break;
        }
        if (message.empty()) continue;

        // 🌟 核心 1：发送数据给服务器 (TCP 用 send)
        ssize_t send_bytes = send(_sockfd, message.c_str(), message.size(), 0);
        if (send_bytes < 0)
        {
            perror("发送失败");
            break;
        }

        // 🌟 核心 2：阻塞等待服务器的回信 (TCP 用 recv)
        ssize_t recv_bytes = recv(_sockfd, buffer, sizeof(buffer) - 1, 0);
        if (recv_bytes > 0)
        {
            buffer[recv_bytes] = '\0'; // 加上字符串结尾
            std::cout << "[服务器回信]: " << buffer << std::endl;
        }
        else if (recv_bytes == 0)
        {
            // 如果服务端按 Ctrl+C 退出，这里会收到 0
            std::cout << "⚠️ 服务器已主动断开连接！" << std::endl;
            break;
        }
        else
        {
            perror("接收失败");
            break;
        }
    }
}

int main()
{
    // 假设你目前先在本地进行自测，填 127.0.0.1
    // 如果要跨网测试，换成你服务器的公网 IP 即可
    std::string server_ip = "127.0.0.1"; 
    uint16_t server_port = 8888;

    client::TcpClient cli(server_ip, server_port);
    
    cli.InitAndConnect();
    cli.StartChat();

    return 0;
}