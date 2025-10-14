#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <assert.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/wait.h>
/* 接下来如果我们想调用自己的命令该怎么办呢 */

int main()
{
    pid_t id = fork();
    assert(id != -1);
    if(id == 0)// 子进程创建成功
    {
        sleep(2);
        execlp("./mybin",
                "./mybin",
                NULL);
        exit(1); // 我们在这里看到的显示一定是0，因为这一段代码已经被覆盖了
    }
    // 从这里开始就是进入父进程
    // 让我们使用waitpid进行等待
    int status;
    pid_t ret = waitpid(id,&status,0);// 直接选用进行阻塞时等待

    if(ret>0) printf("child id:%d, sig:%d, exit id:%d\n",ret,status&0x7F,(status>>8)&0xFF);

    

    return 0;
}
/* 但是在实际的使用过程中我们并不会这样使用，接下来我们创建一个子进程让他去完成下面的任务 */
//int main()
//{
//    pid_t id = fork();
//    assert(id != -1);
//    if(id == 0)// 子进程创建成功
//    {
//        sleep(2);
//        execlp("ls",
//                "ls","-a","-l",
//                NULL);
//        exit(1); // 我们在这里看到的显示一定是0，因为这一段代码已经被覆盖了
//    }
//    // 从这里开始就是进入父进程
//    // 让我们使用waitpid进行等待
//    int status;
//    pid_t ret = waitpid(id,&status,0);// 直接选用进行阻塞时等待
//
//    if(ret>0) printf("child id:%d, sig:%d, exit id:%d\n",ret,status&0x7F,(status>>8)&0xFF);
//
//    
//
//    return 0;
//}


//int main()
//{
//    printf("process is runing\n");
//    
//   // execl("/usr/bin/ls",    /* 我们要去哪里找到这个文件呢 */
//   //         "ls","-a","-l", /* 想如何去执行这样一个程序 */
//   //         NULL);
//
//    execlp("ls",/* 这里就会自动查找路径 */
//            "ls","-a","-l",
//            NULL);
//    perror("excel");// 打印错误的原因
//    
//    // 为什么到这里就没有进行输出了呢
//    // 那是因为为excel执行完毕的时候就已经将代码全部副高，开始执行新的代码了
//    printf("process done");
//    return 0;
//}
