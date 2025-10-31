/*
SYNOPSIS
       #include <sys/ipc.h>
       #include <sys/shm.h>

       int shmget(key_t key, size_t size, int shmflg);

DESCRIPTION
       shmget()  returns the identifier of the System V shared memory segment associated
       with the value of the argument key.  A new shared memory segment, with size equal
       to the value of size rounded up to a multiple of PAGE_SIZE, is created if key has
       the value IPC_PRIVATE or key isn't IPC_PRIVATE, no shared memory  segment  corre�\
       sponding to key exists, and IPC_CREAT is specified in shmflg.

       If  shmflg  specifies  both  IPC_CREAT  and  IPC_EXCL and a shared memory segment
       already exists for key, then shmget() fails with errno set to EEXIST.   (This  is
       analogous to the effect of the combination O_CREAT | O_EXCL for open(2).)

       The value shmflg is composed of:

       IPC_CREAT   ��������ھʹ���һ��������

       IPC_EXCL    ��IPC_CREATһ��ʹ�ã���������ڴ���ڣ��ͳ�����

       RETURN VALUE ���ļ�ϵͳ�ѽڣ�Ҳ��һ�������ڴ�ı�ʶ��
       On success, a valid shared memory  identifier  is  returned.   On  errir,  -1  is
       returned, and errno is set to indicate the error.

       key_t key ��ʲô����Ҫ����Ҫ���ǿ��Ա궨Ψһ�� ftok()�γ�һ��key
*/
#include "comm.hpp"
int main()
{
       /* 1����ȡΨһ��ֵ ȷ�������Ĺ����ڴ���Ψһ�� */
       key_t k = getkey();

       /* 2����ȡ�����ڴ�ID key��shmid ����Ϊ�˽����ں˻��ƺ��û�����*/
       int shmid = creatshm(k, 4096, 0);

       /* 3����ȡ�Ѿ������õĹ����ڴ��ַ */
       char *shmaddr = (char *)getshm(shmid, NULL, 0);

       /* 4�������ظ���ν������ݶ�ȡ�����Ƕ�һ�����ݾ�û���� */
       while (true)
       {
              std::cout << "Client receive: " << shmaddr << std::endl;
              sleep(2); // ÿ 2 ���ȡһ��
       }

       return 0;
}