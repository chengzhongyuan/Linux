/* 
1、可重入函数的概念 
假如我们现在要对链表进行头节点头插，那么势必就会发生一种情况：
如果这个时候我进行插入，然后发现这时候触发了硬件中断信号去执行了我们自定义的函数
并且也是一个头插的过程
结果是,main函数和sighandler先后 向链表中插入两个节点,而最后只
有一个节点真正插入链表中了

a. 不可重入函数概念:
像上例这样,insert函数被不同的控制流程调用,有可能在第一次调用还没返回时就再次进入该函数,这称
为重入,insert函数访问一个全局链表,有可能因为重入而造成错乱,像这样的函数称为 不可重入函数,反之,

b. 可重入函数概念：
如果一个函数只访问自己的局部变量或参数,则称为可重入(Reentrant)函数。想一下,为什么两个不同的
控制流程调用同一个函数,访问它的同一个局部变量或参数就不会造成错乱?这不就是因为这个局部变量是自己临时创建的吗

*/
#include <iostream>
#include <signal.h>
#include <unistd.h>
using namespace std;

/* 
volatile作用：保持内存的可见性，告知编译器，被该关键字修饰的变量，
不允许被优化，对该变量的任何操作，都必须在真实的内存中进行操作 
*/
volatile int flag = 0;

void handler(int signo)
{
    printf("change flag 0 to 1\n");
    flag = 1;
}

int main()
{
    signal(2,handler);
    while(!flag);
    cout<<" PROCESS QUIT NORMAL "<<endl;
    return 0;
}