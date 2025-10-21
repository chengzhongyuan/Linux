#include<stdio.h>
#include<unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main()
{
    close(1);
    int fd = open("log.txt",O_WRONLY | O_CREAT | O_TRUNC,0666);
    if(fd < 0)
    {
        perror("open");
        return 1;
    }
    printf("open fd:%d\n",fd);
    printf("open fd:%d\n",fd);
    fflush(stdout);
    close(fd);
    return 0;
}
