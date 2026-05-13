#include "tcpClient.hpp"
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>

using namespace client;

TcpClient::TcpClient(const std::string& server_ip, uint16_t server_port)
    : _server_ip(server_ip), _server_port(server_port), _sockfd(-1), _is_running(false)
{
}

TcpClient::~TcpClient()
{
    _is_running = false;
    if (_sockfd >= 0)
    {
        close(_sockfd);
    }
}

void TcpClient::InitAndConnect()
{
    _sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (_sockfd < 0)
    {
        perror("socket creation failed");
        exit(1);
    }

    struct sockaddr_in server_addr{};
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(_server_port);
    
    if (inet_pton(AF_INET, _server_ip.c_str(), &server_addr.sin_addr) <= 0)
    {
        std::cerr << "无效的 IP 地址格式！" << std::endl;
        exit(1);
    }

    std::cout << "正在连接到服务器 " << _server_ip << ":" << _server_port << " ..." << std::endl;
    if (connect(_sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0)
    {
        perror("连接服务器失败 (connect failed)");
        exit(1);
    }

    std::cout << "🎉 成功连接到服务器！可以开始聊天了。\n" << std::endl;
}

// 🌟 新增的后台接收线程逻辑
void TcpClient::ReceiveLoop()
{
    char buffer[1024];
    while (_is_running)
    {
        // 这里的 recv 是独立的，完全不会被主线程的 getline 影响！
        ssize_t recv_bytes = recv(_sockfd, buffer, sizeof(buffer) - 1, 0);
        if (recv_bytes > 0)
        {
            buffer[recv_bytes] = '\0';
            // 收到广播消息，直接打印到屏幕上
            // 加上 \n 是为了尽量不破坏用户正在输入的命令行视图
            std::cout << "\n" << buffer << std::endl; 
        }
        else if (recv_bytes == 0)
        {
            std::cout << "\n⚠️ 服务器已主动关闭聊天室！按回车键退出。" << std::endl;
            _is_running = false;
            break;
        }
        else
        {
            // 只有在运行状态下出错才打印，防止正常 close 时误报
            if (_is_running) perror("\n与服务器连接异常断开");
            _is_running = false;
            break;
        }
    }
}

// 🌟 改造后的主线程：只负责发送
void TcpClient::StartChat()
{
    _is_running = true;

    // 🚀 核心改造：召唤一个后台专员（开辟子线程），让它去死循环监听服务器回信
    std::thread recv_thread(&TcpClient::ReceiveLoop, this);
    recv_thread.detach(); // 分离线程，让它在后台默默工作

    std::string message;
    // 主线程死循环：专注处理键盘输入和发送
    while (_is_running)
    {
        std::getline(std::cin, message);

        // 如果后台接收线程发现服务器断开了，主线程也应该及时退出
        if (!_is_running) break;

        if (message == "quit" || message == "exit")
        {
            std::cout << "主动退出聊天室..." << std::endl;
            _is_running = false;
            break;
        }
        if (message.empty()) continue;

        ssize_t send_bytes = send(_sockfd, message.c_str(), message.size(), 0);
        if (send_bytes < 0)
        {
            perror("发送失败");
            _is_running = false;
            break;
        }
    }

    // 退出前关掉套接字，这会同时触发 ReceiveLoop 里的 recv 退出阻塞
    if (_sockfd >= 0)
    {
        close(_sockfd);
        _sockfd = -1;
    }
}

// main 函数保持完全不变即可
int main()
{
    std::string server_ip = "127.0.0.1"; 
    uint16_t server_port = 8888;

    client::TcpClient cli(server_ip, server_port);
    cli.InitAndConnect();
    cli.StartChat();

    return 0;
}