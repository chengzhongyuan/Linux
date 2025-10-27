#include <iostream>
#include <unistd.h>
#include <cassert>
#define PROCESS_NUM 5
int main()
{
    /* 1、创建一批子进程以及对应管道的关系 */
    for(int i = 0;i < PROCESS_NUM;i++)
    {
        int pipefd[2];
        int n = pipe(pipefd);
        assert(n == 0);

        pid_t id = fork();
        if(id == 0)
        {
            /* 2、子进程读取任务处理任务 */
            close(pipefd[1]);
        }
    }
    return 0;
}