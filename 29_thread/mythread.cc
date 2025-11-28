#include<iostream>
#include<pthread.h>
#include<unistd.h>
using namespace std;

// 这个函数资源就是被划分给了这个线程
void* thread_handler(void* args)
{
    while(true)
    {
        cout<<"I am new thread"<<endl;
        sleep(1);
    }
}

int main()
{
    // 创建线程id
    pthread_t tid;
    int n = pthread_create(&tid,NULL,thread_handler,(void*)"thread one");
    
    // 这里是主线程
    while(true)
    {
        cout<<"I am master thread"<<endl;
        sleep(1);
    }
    return 0;
}
