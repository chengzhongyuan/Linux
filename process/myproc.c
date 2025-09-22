#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
int main()
{
    // 引入子进程的概念，观察进程现象
    pid_t id = fork();
    printf("我是一个进程，pid: %d, ppid：%d, id：%d\n",getpid(), getppid(), id);
    // 观察进程状态
    //while(1)
    //{
    //    printf("我是一个进程，pid: %d, ppid：%d\n",getpid(), getppid());
    //    sleep(1);
    //}
    return 0;
}
