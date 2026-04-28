#pragma once

#include <iostream>
#include <string>
#include <functional>
#include <stdint.h>

namespace server
{
    // 调用回调函数进行解耦
    using ServiceCallback = std::function<std::string(const std::string&)>;

    class TcpServer{
        private:
            uint16_t _port;             // 服务器绑定的端口号
            ServiceCallback _callback;  // 要使用的回调函数
            // 这里是与udp不同的地方
            int _listen_sockfd;         // 监听套接字 (TCP 特有：专门负责“迎宾”)
            bool _is_running;           // 控制服务器运行状态的开关
        public:
            // 构造函数
            TcpServer(uint16_t _port,ServiceCallback _callback);
            // 析构函数
            ~TcpServer();

            // 1. 初始化服务器
            // 核心职责：完成 socket() -> bind() -> listen() 三步曲
            void InitServer();

            // 2. 启动服务器
            // 核心职责：进入死循环，调用 accept() 不断接收新客户的连接
            void Start();
        private:
            // 3. 为单个连接的客户端提供服务 (内部辅助函数)
            // TCP 是面向连接的，拉到客之后，通常交由这个函数(或者新线程)去专心服务他
            void Service(int client_sockfd, const std::string& client_ip, uint16_t client_port);
    }
}