// 信号产生的方式
// 1、通过键盘去发送信号（前台进程就是能够影响shell）
// 2、调用系统函数去发送信号 
#include <iostream>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
int main()
{
    int cnt = 5;
    while (cnt)
    {
        std::cout << "hello" << std::endl;
        cnt--;
        if(cnt == 2) raise(3);
        sleep(1);
    }
    /* 调用系统函数去发送信号,也可以自己去实现一个kill命令，入口传入参数 */
    // 1、kill()可以向任意进程发送任意信号
    // 2、raise()给自己发送任意信号
    
    
    // int val = kill(getpid(), 3);
    // if (val == 0)
    //     std::cout << "kill sucess" << std::endl;
    return 0;
}