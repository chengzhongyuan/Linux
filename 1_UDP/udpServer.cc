#include "udpServer.hpp"
#include <iostream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <cstdio> // C++���?
#include <cstdlib>


// ����ֱ�ӳ�ʼ�������˿ں�
UdpServer::UdpServer(int port, ServiceCallback cb) : port(port), callback_(cb)
{
    /* * **DOMAIN** ? Э����

        * `AF_INET` �� IPv4
        * `AF_INET6` �� IPv6

        **TYPE** ? �׽�������

        * `SOCK_STREAM` �� TCP
        * `SOCK_DGRAM` �� UDP

        **PROTOCOL** ? ����Э��

        * һ���� `0`��ϵͳ�Զ�ѡ�� */
       // ��ֻ�Ǵ�����һ���ļ�����������IP��ַ�Ͷ˿ںŲ�û��
    sockfd = socket(AF_INET, SOCK_DGRAM, 0); // Э���Զ���ȷ����

    // ����ʧ�ܵ�����·��ر���?
    if (sockfd < 0)
    {
        // ��������������?
        std::cout << "socket error" << errno << ":" << strerror(errno) << std::endl;
        perror("socket");
        exit(1);
    }

    /* Structure describing an Internet socket address.  */
    // struct sockaddr_in
    // {
    //     __SOCKADDR_COMMON (sin_);
    //     in_port_t sin_port;			/* �˿ں�  */
    //     struct in_addr sin_addr;		/* IP��ַ  */

    //     /* Pad to size of `struct sockaddr'.  */
    //     unsigned char sin_zero[sizeof (struct sockaddr) -
    //             __SOCKADDR_COMMON_SIZE -
    //             sizeof (in_port_t) -
    //             sizeof (struct in_addr)];
    // };

    /* ����һ�� sockaddr_in �ṹ��������������г�Ա��ʼ���? 0 */
    struct sockaddr_in addr{};
    addr.sin_family = AF_INET;         // ���õ�ַ����
    addr.sin_addr.s_addr = INADDR_ANY; // ����IP��ַ�������˼�Ǽ�������������������IP
    addr.sin_port = htons(port);       // ���ö˿ں�,δ���˿ں�һ��Ҫ�������磬һ��Ҫת���ɴ�����ݣ���Ϊ����Ҫ���������

    /* �� socket �͡�IP��ַ + �˿ںš������� */   
    if (bind(sockfd, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
        perror("bind");
        exit(1);
    }

    std::cout << "UDP Server started on port " << port << std::endl;
}

// ����������֮���������ʽ�ȴ�?
// ����������ı��ʾ���һ����ѭ��������һ����פ�ڴ�Ľ���
void UdpServer::run()
{
    char buffer[1024];

    while (true)
    {
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

        // echo ��ȥ
        sendto(sockfd, response.c_str(), response.size(), 0,
               (sockaddr *)&clientAddr, len);
    }

}

    // ���? A��Ӣ���ֵ䷭�����?
    std::string dictService(const std::string& request) {
        static std::unordered_map<std::string, std::string> dict = {
            {"hello", "���?"},
            {"world", "����"},
            {"apple", "ƻ��"},
            {"linux", "һ��ΰ��Ĳ���ϵ�?"}
        };

        auto it = dict.find(request);
        if (it != dict.end()) {
            return it->second;
        } else {
            return "Unknown word (δ֪����)";
        }
    }
    // ��� B��Զ������ִ�з���
std::string execCommandService(const std::string& cmd) {
    // ��ȫ���أ�ֻ�����ض����޺�����
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

int main()
{
    UdpServer server(8888,dictService);
    server.run();
    return 0;
}

// inet 127.0.0.1  ���ػ��ص�ַ�����������㣬���ǻ���һ��Э�