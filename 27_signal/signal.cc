// 信号产生的方式
// 1、通过键盘去发送信号（前台进程就是能够影响shell）
// 2、调用系统函数去发送信号
// 3、硬件异常产生OS怎么知道我除0呢？CPU会异常,野指针就是段错误，操作系统怎么会知道我野指针了呢？
// 4、软件条件产生信号比如管道，由软件条件触发，可以使用alarm
#include <iostream>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <stdlib.h>

void ctachsig(int a)
{
    std::cout << "获取到一个信号，信号编号是" << a << std::endl;
}
int main()
{
    // 如何证明我们收到的是八号信号
    signal(SIGFPE, ctachsig);
    alarm(1);
    int cnt = 0;
    while (true)
    {
        std::cout << "cnt: " << cnt++ << std::endl;
    }
    // while (true)
    // {
    //     sleep(1);
    //     int a = 10;
    //     a /= 0; // 当前进程会收到来自操作系统内部的信号
    // }

    /* 2、调用系统函数去发送信号,也可以自己去实现一个kill命令，入口传入参数 */
    // 1、kill()可以向任意进程发送任意信号
    // 2、raise()给自己发送任意信号
    // 3、abort()给自己发送指定的信号SIGABRT
    // 信号处理行为，进程收到大部分的信号默认处理动作都是终止进程，那那么多信号种类有什么意义呢
    // 信号的意义在于信号的不同代表不同的事件，但是对事件发生之后的处理动作可以一样
    // int cnt = 5;
    // while (cnt)
    // {
    //     std::cout << "hello" << std::endl;
    //     cnt--;
    //     if(cnt == 2) raise(3);
    //     // abort();
    //     sleep(1);
    // }

    // int val = kill(getpid(), 3);
    // if (val == 0)
    //     std::cout << "kill sucess" << std::endl;
    return 0;
}