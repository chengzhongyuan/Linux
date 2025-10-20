#include<stdio.h>
#include<stdlib.h>
#include<assert.h>
#include<string.h>
#include<unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define PWD "PWD"
#define LINE_NUM 1024
#define OPT_NUM 64
char lineCommand[LINE_NUM]; // 这里所存放的是已经提取完成的
char* myargv[OPT_NUM];

void do_exec(char** myargv_)
{
    // 这是CD命令，不需要创建子进程，让shell自己执行对应的命令，本质上执行系统接口
    // 不需要创建子进程，而是直接让shell自己来执行的命令，称之为内建命令
    if(myargv[0] != NULL && strcmp(myargv[0],"cd") == 0)
    {
        if(myargv[1] != NULL) chdir(myargv[1]);
        return;
    }
    pid_t id = fork();
    assert(id >= 0);
    if(id == 0)
    {
        // 执行进程程序替换使用execvp
       execvp(myargv_[0], myargv_);
       perror("execvp failed");
       exit(1);
    }
    int status;
    pid_t ret = waitpid(id, &status, 0);// 这里我就直接选用阻塞式等待
	printf("child id:%d, sig code:%d, exit code:%d\n",ret,status&0x7F,(status>>8)&0xFF);	
    
}

char** do_prase(char* buff)
{
    // 完成字符串切割,这里调用strtok
    int i = 0;
    char* token = strtok(buff, " ");
    while(token != NULL)
    {
        myargv[i++] = token;
        token = strtok(NULL, " ");// 这表示从上一次分割的位置开始继续进行分割
    }
    myargv[i] = NULL;

    return myargv;

}

int do_face()
{
    // 这里就会像bash一样输出行信息还有当前路径
    printf("zy@Cent %s:",getenv(PWD));
    fflush(stdout);
    // 获取用户输入
    char *s = fgets(lineCommand,sizeof(lineCommand)-1,stdin);
    assert(s != NULL);
    (void)s;
    // 清除最后一个\n
    lineCommand[strlen(lineCommand)-1] = '\0';
    // 这里加入了断言，再输入不为空的情况去去进行字符串的剪切和存储
    // 剪切完成后得到的就是一个指针数组，再返回这个数组的地址，所以我这里的返回值就必须是二级指针
    do_prase(lineCommand);
    return 0;
}
int main()
{
    // 这里写一个死循环可以去持续读取执行指令
    while(1)
    {
        // 1、对指令进行分割提取
        if(do_face()<0)
            continue;
        // 2、调用进程替换进行执行，然后父进程等待
        do_exec(myargv);
    }
    return 0;
}
