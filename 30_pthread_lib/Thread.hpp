#pragma once

#include<iostream>
#include<string>
#include<functional>
#include<pthread.h>
#include<assert.h>

// 创建一个线程的类要考虑到私有部分含有什么
class Thread
{
public:
    // 创建了一个函数类型供私有函数进行使用
    typedef std::function<void*(void*)> func_t;
    const int mun = 1024;
public:
    // 构造函数,先把所需要用到的线程名称和要执行的操作准备好
    Thread(func_t func,void* args,int number) : _func(func),_args(args)
    {
        char buffer[1024];
        snprintf(buffer,sizeof buffer,"thread->%d",number);
        _name = buffer;
    }

    // 调用一个静态包装
    // 这是一个类内成员函数，还有一个this指针，要把这里设置成static，这样就没有了
    static void* run(void* arg) // 这里要传入的是this指针，这里的 this 指针，指向的就是当前正在调用 start() 方法的那个 Thread 类对象
    // 这一行设置的就已经完全满足条件了，后面的只是在做一件事情，
    // 把 pthread_create 的 C 风格接口和 C++ 类对象的成员绑定起来
    {
        /*
        static_cast 是 C++ 里的一种类型转换运算符，用来在编译期进行明确的类型转换。
        它比传统的 C 风格强制转换 (type)expr 更安全、更清晰
        */
       
       // 静态方法不能调用成员变量或者成员方法
        Thread* self = static_cast<Thread*>(arg);
        return self->_func(self->_args); 
    }


    // 线程启动函数
    void start()
    {
        /* 
        这里pthread_creat所需要入口函数签名必须是这样的子的void* (*)(void*)
        这代表了需要的是一个参数和返回值都是void*的函数指针

        但是我在这里使用的_func，本质上是一个类对象，
        所以我们现在就想包装一个静态的这样的函数指针
        */
        int n = pthread_create(&_tid,nullptr,run,this);
        /*
        意料之中用断言
        意料之外使用异常
        */
        assert(n == 0);
        // 在调试模式之下断言是存在的，但是在正式发布版本中断言会被全部取消，这个时候
        // 这个n被定义了但是没有被使用，所以要置空一下。
        (void)n;
    }

    // 线程等待函数
    void join()
    {
        int n = pthread_join(_tid,nullptr);
        assert(n == 0);
        (void)n;
    }

    // 析构函数
    ~Thread()
    {

    }
/* 
pthread_create(&tid, NULL, thread_run, (void*)"thread1 run..."
查看线程创建过程，还差需要的参数
*/
private:
    pthread_t _tid;
    void* _args;
    func_t _func;
    std::string _name;
};