#include <iostream>
#include <string>
#include <cstring>
#include <unistd.h>
#include <cassert>
#include <sys/types.h>
#include <sys/wait.h>
#include <cstdio>
using namespace std;

int main()
{
    /* 1�������ܵ��ļ��򿪶�д�� */
    int pipefd[2];
    int n = pipe(pipefd);
    assert(n != -1);

    /* 2�������ӽ��� */
    pid_t id = fork();
    assert(id >=0 );
    if(id == 0)
    {
        // �ӽ��̵�ͨ�Ŵ��루д�룩
        close(pipefd[0]);
        // �ӽ��̵ķ�������
        const char*s = "I am child process";
        int count = 0;
        while(true)
        {
            char buffer[1024]; // ֻ���ӽ��̿��Կ���
            snprintf(buffer, sizeof buffer, "child->parent %s[%d]\n",s,count++);

            // д��ܵ����ڴ漶�ļ����ܵ�������Ҳ�������Ƶ�
            // �����ȡ�رգ�osҲ��ر�д
            write(pipefd[1],buffer, strlen(buffer));
            sleep(1); // ÿ��һ�룬д��һ��
        }
        // �ӽ����˳�
        exit(0);
    } 
    // �����̽��ж�ȡ
    while(true)
    {
        char buffer[1024];
        // ����ܵ�֮��û�������ݣ���ô��һֱ�������������ָ����С���ж�ȡ
        ssize_t s = read(pipefd[0], buffer, sizeof(buffer) - 1);
        if(s > 0) buffer[s] = 0; // �����һ��λ�ü����ֹ
        cout<<"# "<<buffer << endl; 
        
        // ������û�н���sleep
        close(pipefd[0]);
        break;
    }
    // �����̵�ͨ�Ŵ��루��ȡ��
   
    int status;
    // �ȴ��ӽ����˳�,����ʽ
    n = waitpid(id,&status,0);
    assert(n == id);
    int sig = (status)&0x7F;
    cout<<"pid-> "<<n<<"kill by SIG"<<sig<<endl;
    // pipefd[0] = 3  refers to the read end of the pipe
    // pipefd[1] = 4  refers to the write end of the pipe
    cout<<"pipefd[0] = "<<pipefd[0]<<endl<<"pipefd[1] = "<<pipefd[1]<<endl;
    return 0;
}