#pragma once

#include <iostream>
#include <string>
#include <cstdint>

namespace client
{
    class TcpClient
    {
    private:
        std::string _server_ip;  // 服务器 IP
        uint16_t _server_port;   // 服务器端口
        int _sockfd;             // 客户端专属的文件描述符

    public:
        // 构造函数
        TcpClient(const std::string& server_ip, uint16_t server_port);
        
        // 析构函数：负责关闭套接字
        ~TcpClient();

        // 1. 初始化套接字并向服务器发起 TCP 连接
        void InitAndConnect();

        // 2. 开启死循环，与服务器进行持续聊天
        void StartChat();
    };
}