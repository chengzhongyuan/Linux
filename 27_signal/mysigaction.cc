#include <iostream>
#include <signal.h>
#include <unistd.h>

// 可视化倒计时
void count(int cnt)
{
    while(cnt--)
    {
        printf("cnt:%d \r",cnt);
        fflush(stdout);
        sleep(1);
    }
}

void handler(int signo)
{
    std::cout<<"get a signal "<<signo<<std::endl;
    count(10);
    // sleep(20);

    /* 
    当我们正在递达某一个信号期间，同类信号无法被递达
    系统会把当前信号加入到进程的信号屏蔽字，在完成改动作之后。系统又会解除
    */
}

int main()
{
    struct sigaction act, oact;
    act.sa_handler = handler;
    act.sa_flags = 0;
    sigemptyset(&act.sa_mask);

    sigaction(SIGINT,&act,&oact);
    // 这个oact信息是会被保存的，可以方便我们去复原

    while(true) sleep(1);
    return 0;
}