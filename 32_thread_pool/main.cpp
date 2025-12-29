#include "threadpool.hpp"
#include <cstdio>
#include <cstdlib>
#include <ctime>

// 示例任务处理函数：模拟耗时任务
bool handler(int data)
{
    // 为避免多线程下频繁重置相同种子，这里仅第一次调用时设置随机种子
    static bool seeded = false;
    if (!seeded) {
        seeded = true;
        srand((unsigned)time(NULL));
    }

    // 随机 sleep 一段时间，模拟任务处理耗时
    int n = rand() % 5;
    printf("Thread %lu run task: %d -- sleep %d sec\n",
           (unsigned long)pthread_self(), data, n);
    sleep(n);
    return true;
}

int main()
{
    // 1. 创建并初始化线程池（固定线程数）
    ThreadPool pool;
    if (!pool.PoolInit()) {
        fprintf(stderr, "ThreadPool init failed!\n");
        return 1;
    }

    // 2. 投递若干任务到线程池（生产者）
    const int TASKS = 10;
    for (int i = 0; i < TASKS; i++) {
        ThreadTask *tt = new ThreadTask(i, handler);
        if (!pool.PushTask(tt)) {
            fprintf(stderr, "PushTask failed for task %d\n", i);
            delete tt; // 若投递失败，释放任务
        }
    }

    // 3. 优雅退出线程池：
    //    将退出标志置位并唤醒所有线程，
    //    每个工作线程在被唤醒后发现退出标志，安全退出。
    pool.PoolQuit();

    printf("All tasks dispatched. ThreadPool quit.\n");
    return 0;
}
