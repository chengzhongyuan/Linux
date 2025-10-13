#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>


#include<sys/types.h>
#include<sys/wait.h>
int main()
{
    pid_t id = fork();
    if(id == 0)
    {
        int cnt=10;
        while(cnt)
        {
            // 第一次前十秒没有现象是因为数据全部都在行缓冲区里面
            printf("我是子进程:%d,父进程:%d,计数:%d\n",getpid(),getppid(),cnt--);
            sleep(1);
        }
        exit(0);
    }
    // 父进程休眠
    sleep(15);
    int status = 0;
    pid_t ret = waitpid(id, &status, 0);
    //pid_t ret = wait(NULL);
    if(id > 0)
    {
        printf("success:%d, sig number:%d, child exit code:%d\n",ret,(status & 0x7F), (status>>8)&0xFF);
    }
    return 0;
    //for(int i=0;i<133;i++)
    //{
    //    printf("[%d]---> %s\n",i, strerror(i));
    //}
    printf("HHHHHHH!");// 观察这两个不同的现象，思考一下为什么会产生呢？
    sleep(2);
    exit(3); // 库函数（底层也是系统调用）他在中止进程之后会主动刷新缓冲区
    _exit(3);// 系统调用                  终止进程不会刷新缓冲区
    return 0;
}
