#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
int main()
{
    pid_t id = fork();
    if(id==0)
    {
        // 子进程
        printf("我是子进程 pid: %d, ppid: %d\n", getpid(), getppid());
        sleep(5);
        exit(1);
    }
    else
    {
        // 父进程
        while(1)
        {
            printf("我是父进程 pid: %d, ppid: %d\n", getpid(), getppid());
        }
    }
    return 0;
}
