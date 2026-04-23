#ifndef UDP_SERVER_HPP
#define UDP_SERVER_HPP

#include <string>
#include <functional>
#include <unordered_map>
#include <memory>
#include <array>

// 必须在类声明的上方，先定义好这个类型是什么！
using ServiceCallback = std::function<std::string(const std::string&)>;
class UdpServer {
public:
    // 构造函数：强迫在创建服务器时，就必须塞给它一个处理逻辑
    UdpServer(int port, ServiceCallback cb);
    void run();

private:
    int sockfd;
    int port;
    ServiceCallback callback_; // 保存传进来的业务逻辑
};

#endif