#include<stdio.h>
#include <unistd.h>
pid_t fork(void);
//返回值：自进程中返回0，父进程返回子进程id，出错返回-1
int main( void )
 {
    pid_t pid;
 
    printf("Before: pid is %d\n", getpid());
    if ( (pid=fork()) == -1 )perror("fork()"),exit(1);
    printf("After:pid is %d, fork return %d\n", getpid(), pid);
    sleep(1);
    return 0;
 } 
