#pragma once

#include <iostream>
#include <string>
#include <cstdint>
#include <thread> // 引入多线程支持

namespace client
{
    class TcpClient
    {
    private:
        std::string _server_ip;
        uint16_t _server_port;
        int _sockfd;
        bool _is_running; // 控制读写线程退出的开关

    public:
        TcpClient(const std::string& server_ip, uint16_t server_port);
        ~TcpClient();

        void InitAndConnect();
        
        // 启动全双工聊天（内部会派生接收线程）
        void StartChat();

    private:
        // 独立运行在后台的子线程函数：专门负责接收服务器广播
        void ReceiveLoop();
    };
}