#include<stdio.h>
#include<time.h>
void print(int sum)
{
    long long timestamp = time(NULL);
    printf("结果是:%d ,时间戳:%lld \n",sum,timestamp);
}
int AddToVal(int from, int to)
{
    int sum = 0;
    int i;
    for(i=from; i <= to; i++)
    {
        sum += i;
    }
    return sum;
}

int main()
{
    int sum = AddToVal(0,100);
    print(sum);
    return 0;
}
