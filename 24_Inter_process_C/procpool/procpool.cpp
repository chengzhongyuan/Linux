#include <iostream>
#include <unistd.h>
#include <cassert>
#define PROCESS_NUM 5
int main()
{
    /* 1������һ���ӽ����Լ���Ӧ�ܵ��Ĺ�ϵ */
    for(int i = 0;i < PROCESS_NUM;i++)
    {
        int pipefd[2];
        int n = pipe(pipefd);
        assert(n == 0);

        pid_t id = fork();
        if(id == 0)
        {
            /* 2���ӽ��̶�ȡ���������� */
            close(pipefd[1]);
        }
    }
    return 0;
}