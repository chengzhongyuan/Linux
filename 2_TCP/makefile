# 习惯上，C++ 编译器变量名用 CXX，C 语言用 CC
CXX = g++
# 将编译选项提取出来，方便统一管理
CXXFLAGS = -std=c++11

.PHONY:all
all:tcpClient tcpServer
tcpClient:tcpClient.cc
	$(CXX) -o $@ $^ $(CXXFLAGS)
tcpServer:tcpServer.cc
	$(CXX) -o $@ $^ $(CXXFLAGS) -std=c++11 -lpthread

.PHONY:clean
clean:
	rm -f tcpClient tcpServer