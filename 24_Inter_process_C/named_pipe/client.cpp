#include "comm.h"

// ��ɲ����԰Ѹո�д�ĸĳ������ܵ��أ�
int main()
{
    std::cout << "client begin" << std::endl;
    // ��ʵ������ֻд�ķ�ʽȥ���˹ܵ��ļ�
    int FIFOfd = open(NAMED_PIPE, O_WRONLY); //ͬʱ���򿪲Ż��������
    std::cout << "client end" << std::endl;
    if(FIFOfd < 0) exit(1); 

    //write
    char buffer[1024];
    while(true)
    {
        std::cout << "Please Say# ";
        fgets(buffer, sizeof(buffer), stdin); // abcd\n
        if(strlen(buffer) > 0) buffer[strlen(buffer)-1] = 0;
        // ��ܵ��ļ�֮��д��
        ssize_t n = write(FIFOfd, buffer, strlen(buffer));
        assert(n == strlen(buffer));
        (void)n;
    }

    close(FIFOfd);
    return 0;
}