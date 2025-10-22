#include"myStdio.h"

int main()
{
    // 打开文件
    FILE_* f1 = fopen_("log.txt","w");

    // 写入文件
    char s[]= "ssss\n";
    fwrite_(s,strlen(s), f1);
    sleep(5);
    fclose(f1);
    return 0;
}
