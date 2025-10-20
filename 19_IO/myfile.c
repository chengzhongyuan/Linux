#include<stdio.h>
#include<unistd.h>
#include<string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <assert.h>
#define FILE_NAME(number) "log.txt"#number// 这个可以解释一下：
int main()
{
    printf("stdin:%d \n", stdin->_fileno);
    printf("stdout:%d \n", stdout->_fileno);
    printf("stderr:%d \n", stderr->_fileno);
    // 这个fd为什么是从3开始的呢
    int fd = open(FILE_NAME(1),O_WRONLY | O_CREAT | O_APPEND,0666);
    assert(fd > 0);
    int count = 5;
    const char *msg = "hello bit!\n";
    int len = strlen(msg);
 // 这个len值如果加一会产生乱码，并且再进行文件写入的时候，系统调用没有自动清空
 // 再加一个选项 O_TRUNC 产生自动清空
    while(count--){
        write(fd, msg, len);
        //fd: 后面讲， 
        //msg：缓冲区首地址， 
        //len: 本次读取，期望写入多少个字节的数据。 
        //返回值：实际写了多少字节数据
    }

    char buffer[1024];
    ssize_t num = read(fd, buffer, sizeof(buffer)-1);
    if(num > 0) buffer[num] = 0;
    printf("%s",buffer);

    close(fd);
    // FILE *fp = fopen("log.txt", "w");
    //if(!fp){
    //    printf("fopen error!\n");
    //}
 
    //const char *msg = "hello bit!\n";
    //int count = 5;
    //while(count--){
    //    fwrite(msg, strlen(msg), 1, fp);
    //}
 
    //fclose(fp);




//    chdir("/home/zhongyuan");
//    while(1)
//    {
//        printf("我是一个进程：%d\n",getpid());
//        sleep(3);
//    }
    return 0;
}
