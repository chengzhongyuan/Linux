#pragma once

#include <iostream>
#include <string>
#include <functional>
#include <stdint.h>

// 将服务端进行改造实现多人聊天室的功能
#include <unordered_map>
#include <mutex>

namespace server
{
    class TcpServer; // 前置声明，因为回调定义里需要用到该指针
    // 升级后的回调函数签名：
    // 参数1：服务器对象指针 (this)
    // 参数2：发送消息的客户端专属文件描述符
    // 参数3：发送者的身份字符串 (例如 "127.0.0.1:47252")
    // 参数4：收到的消息体
    using ServiceCallback = std::function<void(TcpServer*, int, const std::string&, const std::string&)>;

    class TcpServer{
        private:
            uint16_t _port;             // 服务器绑定的端口号
            ServiceCallback _callback;  // 要使用的回调函数
            // 这里是与udp不同的地方
            int _listen_sockfd;         // 监听套接字 (TCP 特有：专门负责“迎宾”)
            bool _is_running;           // 控制服务器运行状态的开关

            // key: 客户端的描述符, value: 客户端的描述符（或者包含IP、Port的结构体）
            std::unordered_map<int, std::string> _online_clients; 
            std::mutex _mtx; // 保护通讯录的锁
        public:
            // 构造函数
            TcpServer(uint16_t _port,ServiceCallback _callback);
            ~TcpServer();

            // 1. 初始化服务器
            // 核心职责：完成 socket() -> bind() -> listen() 三步曲
            void InitServer();
            void Start();

            void Broadcast(int sender_fd, const std::string& sender_info, const std::string& message);
        private:
            // 3. 为单个连接的客户端提供服务 (内部辅助函数)
            // TCP 是面向连接的，拉到客之后，通常交由这个函数(或者新线程)去专心服务他
            void Service(int client_sockfd, const std::string& client_ip, uint16_t client_port);
    };
}