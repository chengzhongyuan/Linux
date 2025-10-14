#include <stdio.h>
#include <string.h>
#include <stdlib.h>
// getpid getppid fork
#include <unistd.h>
// waitpid
#include <sys/types.h>
#include <sys/wait.h>
#define Num 10
// 函数指针
typedef void(*func_t)();

func_t handlerTask[Num];
// 样例任务
void task1()
{
    printf("1111");
}

void task2()
{
    printf("2222");
}
void loadTask()
{
    memset(handlerTask,0,sizeof(handlerTask));
    handlerTask[0] = task1;
    handlerTask[1] = task2;
}

int main()
{
    pid_t id = fork();
    if (id == 0)
    {
        int cnt = 5;
        while (cnt)
        {
            printf("我是子进程:%d,父进程:%d,计数:%d\n", getpid(), getppid(), cnt--);
            sleep(1);
        }
        exit(10);
    }

    // 父进程休眠
    sleep(7);
    int status = 0;
    pid_t ret = waitpid(id, &status, 0);
    // pid_t ret = wait(NULL);
    if (id > 0)
    {
        if(WIFEXITED(status))
        {
            printf("exit code:%d\n", WEXITSTATUS(status));
        }
        // 这种方法使用起来不是那么便捷
        printf("success:%d, sig number:%d, child exit code:%d\n",
               ret, (status & 0x7F), (status >> 8) & 0xFF);
    }
    return 0;
}


