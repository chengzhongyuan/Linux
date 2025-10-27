#include <iostream>
#include <string>
#include <cstring>
#include <unistd.h>
#include <cassert>
#include <sys/types.h>
#include <sys/wait.h>
#include <cstdio>
using namespace std;

int main()
{
    /* 1、创建管道文件打开读写端 */
    int pipefd[2];
    int n = pipe(pipefd);
    assert(n != -1);

    /* 2、创建子进程 */
    pid_t id = fork();
    assert(id >=0 );
    if(id == 0)
    {
        // 子进程的通信代码（写入）
        close(pipefd[0]);
        // 子进程的发送数据
        const char*s = "I am child process";
        int count = 0;
        while(true)
        {
            char buffer[1024]; // 只有子进程可以看见
            snprintf(buffer, sizeof buffer, "child->parent %s[%d]\n",s,count++);

            // 写入管道，内存级文件
            write(pipefd[1],buffer, strlen(buffer));
            sleep(1); // 每隔一秒，写入一次
        }
        // 子进程退出
        exit(0);
    } 
    // 父进程进行读取
    while(true)
    {
        char buffer[1024];
        ssize_t s = read(pipefd[0], buffer, sizeof(buffer) - 1);
        if(s > 0) buffer[s] = 0; // 再最后一个位置加入截止
        cout<<"# "<<buffer << endl; 
        // 父进程没有进行sleep
    }
    // 父进程的通信代码（读取）
    close(pipefd[1]);

    // 等待子进程退出,阻塞式
    n = waitpid(id,nullptr,0);
    assert(n == id);
    // pipefd[0] = 3  refers to the read end of the pipe
    // pipefd[1] = 4  refers to the write end of the pipe
    cout<<"pipefd[0] = "<<pipefd[0]<<endl<<"pipefd[1] = "<<pipefd[1]<<endl;
    return 0;
}