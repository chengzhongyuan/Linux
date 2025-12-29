#ifndef __M_TP_H__
#define __M_TP_H__

#include <iostream>
#include <queue>
#include <pthread.h>
#include <unistd.h> // pthread_self, usleep
#include <cassert>

// 可配置的线程池大小
#define MAX_THREAD 5

// 定义了一个函数指针类型
// 任务处理函数类型：输入一个整型数据，返回执行是否成功
typedef bool (*handler_t)(int);

// 任务对象：封装任务数据与执行函数
class ThreadTask
{
private:
    int _data;          // 任务数据（示例用 int，可扩展为结构体/对象）
    handler_t _handler; // 任务处理回调

public:
    ThreadTask()
        : _data(-1), _handler(NULL) {}

    ThreadTask(int data, handler_t handler)
        : _data(data), _handler(handler) {}

    void SetTask(int data, handler_t handler)
    {
        _data = data;
        _handler = handler;
    }

    // 执行任务：直接调用回调
    void Run()
    {
        assert(_handler != NULL);
        _handler(_data);
    }
};

// 线程池：维护固定数量的工作线程，从任务队列中提取任务执行
class ThreadPool
{
private:
    int _thread_max; // 最大工作线程数
    int _thread_cur; // 当前存活的工作线程数
    bool _tp_quit;   // 线程池退出标志（被置位后唤醒线程让其退出）

    std::queue<ThreadTask *> _task_queue; // 任务队列（生产者：PushTask；消费者：工作线程）
    pthread_mutex_t _lock;                // 队列互斥锁
    pthread_cond_t _cond;                 // 条件变量：队列为空时，线程在此等待

private:
    // 加锁/解锁队列操作，保护队列与退出标志
    void LockQueue()
    {
        pthread_mutex_lock(&_lock);
    }
    void UnlockQueue()
    {
        pthread_mutex_unlock(&_lock);
    }

    // 唤醒一个/全部等待中的工作线程
    void WakeUpOne()
    {
        pthread_cond_signal(&_cond);
    }
    void WakeUpAll()
    {
        pthread_cond_broadcast(&_cond);
    }

    // 当前线程安全退出：减少线程计数并退出
    void ThreadQuit()
    {
        _thread_cur--;
        UnlockQueue();      // 退出前确保释放锁
        pthread_exit(NULL); // 结束当前工作线程
    }

    // 等待任务：队列为空时挂起；若池处于退出状态则直接退出
    void ThreadWait()
    {
        if (_tp_quit)
        {
            ThreadQuit();
        }
        // 原子地释放锁并阻塞等待；被唤醒后重新加锁返回
        pthread_cond_wait(&_cond, &_lock);
    }

    // 队列是否为空
    bool IsEmpty() const { return _task_queue.empty(); }

    // 工作线程入口函数（静态成员，pthread_create 需要 C 接口）
    static void *thr_start(void *arg)
    {
        ThreadPool *tp = (ThreadPool *)arg;
        while (true)
        {
            tp->LockQueue();

            // 当队列为空时，进入等待；被唤醒后检查退出标志或继续取任务
            while (tp->IsEmpty())
            {
                // 若线程池退出：让工作线程有序退出
                if (tp->_tp_quit)
                {
                    tp->ThreadQuit(); // 内部会解锁并 pthread_exit
                    // 不会执行到这里
                }
                tp->ThreadWait();
            }

            // 队列非空：弹出任务
            ThreadTask *tt = nullptr;
            tp->PopTask(&tt);
            tp->UnlockQueue();

            // 执行任务（不在锁内执行，避免阻塞其他线程取任务）
            if (tt)
            {
                tt->Run();
                delete tt; // 任务对象由线程池负责释放
            }
        }
        return NULL;
    }

public:
    ThreadPool(int max = MAX_THREAD)
        : _thread_max(max),
          _thread_cur(0), // 由 PoolInit 创建线程后计数
          _tp_quit(false)
    {
        pthread_mutex_init(&_lock, NULL);
        pthread_cond_init(&_cond, NULL);
    }

    ~ThreadPool()
    {
        pthread_mutex_destroy(&_lock);
        pthread_cond_destroy(&_cond);
    }

    // 初始化线程池：创建固定数量工作线程
    bool PoolInit()
    {
        pthread_t tid;
        for (int i = 0; i < _thread_max; i++)
        {
            int ret = pthread_create(&tid, NULL, thr_start, this);
            if (ret != 0)
            {
                std::cerr << "create pool thread error, ret=" << ret << "\n";
                return false;
            }
            _thread_cur++; // 创建成功，计数+1
            // 可选择 pthread_detach(tid)，让线程结束后资源自动回收
            // 此处不 detach，退出流程由 PoolQuit 管理
        }
        return true;
    }

    // 投递任务：生产者接口
    bool PushTask(ThreadTask *tt)
    {
        LockQueue();
        if (_tp_quit)
        {
            UnlockQueue();
            return false; // 已退出状态不再接受新任务
        }
        _task_queue.push(tt);
        WakeUpOne(); // 唤醒一个等待中的工作线程
        UnlockQueue();
        return true;
    }

    // 弹出任务：仅供工作线程调用
    bool PopTask(ThreadTask **tt)
    {
        if (_task_queue.empty())
            return false;
        *tt = _task_queue.front();
        _task_queue.pop();
        return true;
    }

    // 线程池退出：设置退出标志，唤醒所有线程，使其有序退出
    bool PoolQuit()
    {
        LockQueue();
        _tp_quit = true;
        UnlockQueue();

        // 广播唤醒全部线程（包括因队列为空而等待的线程）
        while (_thread_cur > 0)
        {
            WakeUpAll();
            // 睡一会儿，让被唤醒的线程有机会执行 ThreadQuit 退出
            usleep(1000);
        }
        return true;
    }
};

#endif // __M_TP_H__
