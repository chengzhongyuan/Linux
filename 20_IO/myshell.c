#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define PWD "PWD"
#define LINE_NUM 1024
#define OPT_NUM 64

// 关于重定向的一些宏定义
#define INPUT_REDIR  1
#define OUTPUT_REDIR 2
#define APPEND_REDIR  3
#define NONE_REDIR   0

int redirType = NONE_REDIR;
char* redirFile = NULL;

char lineCommand[LINE_NUM]; // 这里所存放的是已经提取完成的
char* myargv[OPT_NUM];

/* 为他添加重定向的一个功能，我同样还是需要分割字符串找到 
 * 1、<  输入重定向
 * 2、>  输出重定向
 * 2、>> append
 */

/* 去除字符串之中的空格 */
char* trimSpace(char* str) {
    while (*str && isspace((unsigned char)*str)) {
        str++;
    }
    return str;
}

/* 1、提取重定向模式 2、提却要写入的文件 */
void command_dir(char* str)
{
    // 首先这个字符串一定不能为空
    assert(str);
    char* start = str;
    char* end   = str + strlen(str);
    while(start < end)
    {
        if(*start == '>')
        {
            *start = '\0';
            start++;
            if(*start == '>')
            {
                // 这里面就是ERROR_REDIR
                *start = '\0';
                start++;
                // 去空格
                redirFile = trimSpace(start);
                redirType = APPEND_REDIR;
                break;
            }
            // 走到这里就是输出重定向了
            redirFile = trimSpace(start);
            redirType = OUTPUT_REDIR;
        }
        else if(*start == '<')
        {
            *start = '\0';
            start++;
            // 去空格
            redirFile = trimSpace(start);
            redirType = INPUT_REDIR;
            break;
        }
        else{
            start++;
        }
    }
}

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

    if(redirType != NONE_REDIR)
    {
        switch(redirType)
        {
            case INPUT_REDIR:
                {

                // 需要在这里完成输入重定向
                int fd = open(redirFile,O_RDONLY);
                if(fd<0)
                {
                    perror("open");
                    exit(1);
                }
                // 重定向文件已经打开了
                dup2(fd,0);
                close(fd);
                break;
                }
            case OUTPUT_REDIR:
            case APPEND_REDIR:
                {
                    
                int flags = O_WRONLY | O_CREAT;
                if(redirType == APPEND_REDIR) flags |= O_APPEND;
                else flags |= O_TRUNC;
                int fd = open(redirFile, flags,0666);
                if(fd<0)
                    {
                        perror("open");
                        exit(1);
                    }
                dup2(fd,1);
                close(fd);
                break;
                }

        }
    }
        // 执行进程程序替换使用execvp
       execvp(myargv_[0], myargv_);
       perror("execvp failed");
       exit(1);
    }
    int status;
    pid_t ret = waitpid(id, &status, 0);// 这里我就直接选用阻塞式等待
	printf("child id:%d, sig code:%d, exit code:%d\n",ret,status&0x7F,(status>>8)&0xFF);	
    
    redirType = NONE_REDIR;
    redirFile = NULL;
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
    
    // 先处理重定向
    command_dir(lineCommand);
    // 再分割命令
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
