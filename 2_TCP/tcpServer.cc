#include "tcpServer.hpp"
#include "log.hpp"
#include <sys/types.h>          
#include <sys/socket.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <thread>

using namespace std;
// 构造函数，初始时传入端口号和业务逻辑
server::TcpServer::TcpServer(uint16_t port, ServiceCallback cb)
    : _port(port), 
    _callback(cb),
    _listen_sockfd(-1),
    _is_running(false)
{
    // 完成目前所有初始工作
}

// 1. 初始化服务器
// 核心职责：完成 socket() -> bind() -> listen() 三步曲
void server::TcpServer::InitServer()
{
    // 1、创建套接字对应空间
    _listen_sockfd = socket(AF_INET,SOCK_STREAM,0);
    if (_listen_sockfd < 0)
    {
        // 使用 FATAL 级别，像 printf 一样传入 %d 和 %s
        LogMessage(FATAL, "创建套接字失败! 错误码: %d, 描述: %s", errno, strerror(errno));
        // std::cout << "socket error" << errno << ":" << strerror(errno) << std::endl;
        // perror("socket");
        exit(1);
    }

    // 2、进行绑定
    struct sockaddr_in addr{}; // 这就是一个初始化的操作
    addr.sin_family = AF_INET; // 协议域
    addr.sin_port = htons(_port); // 端口号要转换成大端字节序的方式进行网络端的传输
    addr.sin_addr.s_addr = INADDR_ANY;// 任意IP地址

    if (bind(_listen_sockfd,(struct sockaddr *)&addr, sizeof(addr)) < 0 )
    {
        perror("bind");
        exit(1);
    }

    // 3、监听 (Listen) —— 这是 TCP 特有的
    // 5 表示全连接队列的长度，通常设置 5~128 都可以
    if (listen(_listen_sockfd, 5) < 0) 
    {
        perror("listen");
        exit(1);
    }

    LogMessage(NORMAL,"TCP 服务器启动成功，正在监听端口 %d", _port);
    // std::cout << "TCP Server started on port " << _port << std::endl;
    
}

// 2. 启动服务器
// 核心职责：进入死循环，调用 accept() 不断接收新客户的连接
void server::TcpServer::Start()
{
    _is_running = true;
    while (_is_running)
    {
        struct sockaddr_in clientaddr{};
        socklen_t len = sizeof(clientaddr);

        // 1、从等待队列之中拉去一个用户
        // RETURN VALUE
        // On success, these system calls return a nonnegative integer that is a descriptor for the accepted socket.  
        // On error, -1 is returned, and errno is set appropriately.
        int client_sockfd = accept(_listen_sockfd,(struct sockaddr*)&clientaddr,&len);
        if (client_sockfd < 0)
        {
            perror("accept error");
            continue; // 拉客失败没关系，继续拉下一个，不要让整个服务器崩溃
        }

        // 2、提取客户端的IP地址和端口号
        char ip_buf[32];
        // 把底层网络二进制转换成可读的字符串提取
        inet_ntop(AF_INET, &clientaddr.sin_addr, ip_buf, sizeof(ip_buf));
        // 转换IP和端口号
        std::string client_ip = ip_buf;
        uint16_t client_port = ntohs(clientaddr.sin_port);// 网络字节序转换成可读


        LogMessage(NORMAL, "新客户接入! IP: %s, 端口: %d, 分配FD: %d", client_ip.c_str(), client_port, client_sockfd);
        // std::cout << "\n[新连接建立] Client IP: " << client_ip 
        //           << ", Port: " << client_port 
        //           << ", 分配的单间 fd: " << client_sockfd << std::endl;
        
        // 3、链接建立完成开始提供服务端服务,这是一个只能一对一的版本，现在引入多线程概念去实现
        // Service(client_sockfd, client_ip, client_port);

        // 修改成多人在线聊天室
        // 登记：将新连接加入在线映射表
        {
            std::lock_guard<std::mutex> lock(_mtx); // 自动加锁/解锁
            _online_clients[client_sockfd] = client_ip + ":" + std::to_string(client_port);
        }

        // =========================================================
        // （开辟新线程）
        // 参数1：要执行的函数指针 (&TcpServer::Service)
        // 参数2：类成员函数需要传入的 this 指针
        // 参数3-5：传给 Service 函数的具体参数
        // =========================================================
        // 这里的fd只能传值，因为这是循环接收的假如我们传引用这个数值会一直改变
        std::thread t(&TcpServer::Service, this, client_sockfd, client_ip, client_port);
        // 执行完任务之后去析构掉
        t.detach();
    }
    
}

void server::TcpServer::Service(int client_sockfd, const std::string& client_ip, uint16_t client_port)
{
    char buffer[1024];
    std::string client_info = client_ip + ":" + std::to_string(client_port);
    // 单间的死循环：只要客人不走，我们就一直为他服务
    while (true)
    {
        // 1：接收数据 (TCP 通常用 recv 或 read)
        // 注意这里用的 fd 是客人专属的 client_sockfd，绝对不能用 _listen_sockfd！
        ssize_t n = recv(client_sockfd, buffer, sizeof(buffer) - 1, 0);

        if (n > 0)
        {
            // 正常收到消息
            buffer[n] = '\0';
            std::cout << "[" << client_ip << ":" << client_port << "] 说: " << buffer << std::endl;

            // 核心传递：将处理权限转交给顶层的回调逻辑函数
            _callback(this, client_sockfd, client_info, buffer);
        }
        else if (n == 0)
        {
            // ? 极其重要：在 TCP 中，recv 返回 0 代表对端（客户端）主动断开了连接！
            std::cout << "[" << client_ip << ":" << client_port << "] 正常退出，断开连接。" << std::endl;
            break; // 客人走了，结束这个死循环
        }
        else
        {
            // n < 0，发生网络错误（比如网线拔了）
            std::cout << "[" << client_ip << ":" << client_port << "] 发生异常，断开连接。" << std::endl;
            perror("recv error");
            break; // 同样必须退出死循环
        }
    }

    // 退出服务逻辑时，必须同时在映射表里擦除记录并回收文件描述符
    {
        std::lock_guard<std::mutex> lock(_mtx);
        _online_clients.erase(client_sockfd);
    }

    // ??? 致命重点：关门送客！
    // 只要退出了上面的循环，说明服务结束，必须立刻把单间钥匙还给操作系统！
    // 如果不写这一句，服务器运行一段时间后，所有的系统 FD 资源会被耗尽，服务器直接崩溃！
    close(client_sockfd);
}

server::TcpServer::~TcpServer()
{
}

// ===================================================================
// 业务逻辑层实现 (解耦展示)
// ===================================================================

// 回调函数A: 原路返回业务 (Echo)
void echoService(server::TcpServer* svr, int fd, const std::string& info, const std::string& msg)
{
    std::string response = "[Echo 应答]: " + msg;
    send(fd, response.c_str(), response.size(), 0);
}

// 回调函数B: 多人聊天室消息广播插件
void chatRoomService(server::TcpServer* svr, int fd, const std::string& info, const std::string& msg)
{
    // 调用由服务端底层公开的 Broadcast 方法推送
    svr->Broadcast(fd, info, msg);
}

// 🌟 实现底层的广播分发机制
void server::TcpServer::Broadcast(int sender_fd, const std::string& sender_info, const std::string& message)
{
    // 组装群聊广播报文格式
    std::string broadcast_msg = "[" + sender_info + "] 对大家说: " + message;

    std::lock_guard<std::mutex> lock(_mtx);
    for (auto& client : _online_clients)
    {
        // 将消息分发给除了发送端以外的所有在线套接字
        if (client.first != sender_fd)
        {
            send(client.first, broadcast_msg.c_str(), broadcast_msg.size(), 0);
        }
    }
}
int main()
{
    // 假设你目前先在本地进行自测，填 127.0.0.1
    // 如果要跨网测试，换成你服务器的公网 IP 即可


    server::TcpServer tcpserver(8888,chatRoomService);
    tcpserver.InitServer();
    tcpserver.Start();

    return 0;
}