# 习惯上，C++ 编译器变量名用 CXX，C 语言用 CC
CXX = g++
# 将编译选项提取出来，方便统一管理
CXXFLAGS = -std=c++11

.PHONY:all
all:udpClient udpServer
udpClient:udpClient.cc
	$(CXX) -o $@ $^ $(CXXFLAGS)
udpServer:udpServer.cc
	$(CXX) -o $@ $^ $(CXXFLAGS)

.PHONY:clean
clean:
	rm -f udpClient udpServer