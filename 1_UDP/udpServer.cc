#include "udpServer.hpp"
#include <iostream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <cstdio> 
#include <cstdlib>


// 对端口号和回调函数进行初始化
UdpServer::UdpServer(int port, ServiceCallback cb) : port(port), callback_(cb)
{
    /* * **DOMAIN** ? 协议域

        * `AF_INET`  IPv4
        * `AF_INET6` IPv6

        **TYPE** ? 通信类型udp还是tcp
        * `SOCK_STREAM`  TCP
        * `SOCK_DGRAM`  UDP

        **PROTOCOL** ? 使用什么样的协议，这个一般就是0就可以了因为协议域和通信类型已经足够定下通信协议了

    */
       
    sockfd = socket(AF_INET, SOCK_DGRAM, 0); // 其实就生成一个文件描述符

    // 正常生成就继续进行
    if (sockfd < 0)
    {
        std::cout << "socket error" << errno << ":" << strerror(errno) << std::endl;
        perror("socket");
        exit(1);
    }

// /* Structure describing an Internet socket address.  */
// struct sockaddr_in
//   {
//     __SOCKADDR_COMMON (sin_);
//     in_port_t sin_port;			/* Port number.  */
//     struct in_addr sin_addr;		/* Internet address.  */

//     /* Pad to size of `struct sockaddr'.  */
//     unsigned char sin_zero[sizeof (struct sockaddr) -
// 			   __SOCKADDR_COMMON_SIZE -
// 			   sizeof (in_port_t) -
// 			   sizeof (struct in_addr)];
//   };


    /* 锟斤拷锟斤拷一锟斤拷 sockaddr_in 锟结构锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷谐锟皆憋拷锟绞硷拷锟轿? 0 */
    struct sockaddr_in addr{};
    addr.sin_family = AF_INET;         // 指定IP地址地址版本人为IPV4
    addr.sin_addr.s_addr = INADDR_ANY; // IP地址
    addr.sin_port = htons(port);       // 端口号

    /* IP地址和端口号进行绑定 */   
    if (bind(sockfd, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
        perror("bind");
        exit(1);
    }

    std::cout << "UDP Server started on port " << port << std::endl;
}


void UdpServer::run()
{
    char buffer[1024];

    while (true)
    {
        // 客户端
        sockaddr_in clientAddr{};
        socklen_t len = sizeof(clientAddr);

        ssize_t n = recvfrom(sockfd, buffer, sizeof(buffer) - 1, 0,
                             (sockaddr *)&clientAddr, &len);

        if (n < 0)
        {
            perror("recvfrom");
            continue;
        }

        buffer[n] = '\0';
        std::cout << "Client Original Request:" << buffer << std::endl;

        std::string request = buffer;

        std::string response = callback_(request);

        // 
        sendto(sockfd, response.c_str(), response.size(), 0,
               (sockaddr *)&clientAddr, len);
    }

}

    // 插件 A:词典功能
    std::string dictService(const std::string& request) {
        static std::unordered_map<std::string, std::string> dict = {
            {"hello", "你好"},
            {"world", "世界"},
            {"apple", "苹果"},
            {"linux", "操作系统"}
        };

        auto it = dict.find(request);
        if (it != dict.end()) {
            return it->second;
        } else {
            return "Unknown word (未知)";
        }
    }
    // 插件 B：远程命令执行服务
    std::string execCommandService(const std::string& cmd) {
        // 安全拦截：只允许特定的无害命令
        if (cmd != "ls" && cmd != "pwd") {
            return "Command Not Allowed! Please try 'ls' or 'pwd'\n";
        }

        std::array<char, 128> buf;
        std::string result;
        std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd.c_str(), "r"), pclose);
        
        if (!pipe) {
            return "popen failed!";
        }
        
        while (fgets(buf.data(), buf.size(), pipe.get()) != nullptr) {
            result += buf.data();
        }
        
        return result.empty() ? "Success (No output)\n" : result;
    }

    // 插件C: 原路返回
    std::string repeatService(const std::string& request)
    {
        return request;
    }


int main()
{
    UdpServer server(8888,repeatService);
    server.run();
    return 0;
}

// inet 127.0.0.1  回环地址，只在当前服务器之中进行数据流转
// 49.232.244.54