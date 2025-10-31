#include "comm.hpp"
int main()
{
    /* 1������Ψһkeyֵ */
    key_t k = getkey();

    /* 2�����������ڴ� */
    int shmid = creatshm(k, 4096, IPC_CREAT | 0666);

    /* 3����ȡ�����õĹ����ڴ�����ַ,Ҳ���ǰѵ�ǰ���̺͹���ռ���й����� */
    char* shmarr = (char*)getshm(shmid,NULL,0);

    /* 4��������֮��д������ 1.������Ϣ 2.����PID 3.������ */
    strcpy(shmarr,"hello world!");
    const char* msg = "hello I am server";
    pid_t id = getpid();
    int counter = 0;
    while(true)
    {
        snprintf(shmarr, 4096, "server[%d]: %s [%d]\n", id, msg, counter++);
        sleep(2);
    }



    /* 5��ȥ������,������������ڴ������Ǵ��ڵ�,ȥ������֮��ֻҪ�����ڴ治��ɾ�����ǿ��Զ�ȡ����Ϣ�� */
    shmdt(shmarr);

    sleep(5);
    
    /* 5��ɾ�������ڴ� */
    shmdel(shmid, IPC_RMID, NULL);
    return 0;
}

/*
�����ڴ���������������Ų���ϵͳ�ģ��������Ž��̵ģ���
------ Shared Memory Segments --------
key        shmid      owner      perms      bytes      nattch     status      
0x66010072 0          zhongyuan  666        1024       0       
*/

// �����ڴ治������ǽ���ͬ���ͻ���Ĳ�����û�ж��������κεı���
// ϵͳ���乲���ڴ�����4KBΪ��λ��