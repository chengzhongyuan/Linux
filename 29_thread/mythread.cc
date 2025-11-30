#include<iostream>
#include<pthread.h>
#include<unistd.h>
using namespace std;

// 这个函数资源就是被划分给了这个线程
void* thread_handler(void* args)
{
    // 这样我们就接收到了这个参数
    const char* str = (char*)args;
    while(true)
    {
        cout<<str<<endl;
        cout<<"I am new thread"<<endl;
        sleep(1);
    }
}

int main()
{
    // 创建线程id，这个是一个地址，现在还无法解释
    pthread_t tid;
    // 最后这个参数是会被喂给函数指针的
    int n = pthread_create(&tid,NULL,thread_handler,(void*)"thread one");
    
    // 这里是主线程
    while(true)
    {
        cout<<"I am master thread tid create by me: "<<tid<<endl;
        sleep(1);
    }
    return 0;
}

// 在使用ps xaj | grep mythread 的过程当中我们会发现只有一个进程显示

/*
[zhongyuan@VM-8-11-centos project]$ ps xaj | grep mythread
10280 12228 12228 10280 pts/3    12228 Sl+   1001   0:00 ./mythread
11969 12313 12312 11969 pts/4    12312 S+    1001   0:00 grep --color=auto mythread
并且在我们杀死这样一个进程之后，所有的线程也会随之停止

ps -aL 就表示查看轻量级进程
[zhongyuan@VM-8-11-centos project]$ ps -aL
  PID   LWP TTY          TIME CMD
13925 13925 pts/3    00:00:00 mythread  PID和LWP相同就表明这个是主进程
13925 13926 pts/3    00:00:00 mythread
14018 14018 pts/4    00:00:00 ps

LWP :light weight process 这就是轻量级进程ID
*/

// 线程一旦被创建几乎所有资源都是被共享的，线程之间互相通信是非常容易的
/*
不过呢也有一些是线程私有的资源
1、独立的线程ID，独立的优先级，独立的线程状态
下面这两点是最重要的
2、一定要有自己私有的上下文结构
3、局部变量，每一个线程都会有自己独立的栈结构
*/ 