#include "comm.h"

int main()
{
    /* 1�������ܵ��ļ������ж��Ƿ�ɹ� */
    bool r = createFifo(NAMED_PIPE);
    assert(r);
    (void)r;
    
    /* 2���Զ���ʽ�򿪹ܵ��ļ� */
    std::cout << "server begin" << std::endl;
    int FIFOfd = open(NAMED_PIPE, O_RDONLY);
    std::cout << "server end" << std::endl;
    if(FIFOfd < 0) exit(1);

    //read
    char buffer[1024];
    while(true)
    {
        ssize_t s = read(FIFOfd, buffer, sizeof(buffer)-1);
        if(s > 0)
        {
            buffer[s] = 0;
            std::cout << "client->server# " << buffer << std::endl;
        }
        else if(s == 0)
        {
            std::cout << "client quit, me too!" << std::endl;
            break;
        }
        else
        {
            std::cout << "err string: " << strerror(errno) << std::endl;
            break;
        }
    }

    close(FIFOfd);

    // sleep(10);
    /* 3��ɾ���ܵ��ļ� */
    removeFifo(NAMED_PIPE);
    return 0;
}
