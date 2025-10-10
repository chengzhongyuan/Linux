 #include <stdio.h>
 #include <unistd.h>
 #include <stdlib.h>
 #include <sys/types.h>

int g_val = 0;
 
int main()
 {
    pid_t id = fork();
    if(id < 0)
    {
        perror("fork");
        return 0;
    }
    else if(id == 0)
    { //child
        int cnt = 0;
        while(cnt <= 10)
        {
            printf("child[%d]: %d : %p\n", getpid(), g_val, &g_val);
            sleep(1);
            cnt++;
            if(cnt == 10)
            {
                g_val = 100;
                printf("子进程已经更改全局变量了......   ");
            }
        }
    }
    else
    { //parent
        while(1)
        {

            printf("parent[%d]: %d : %p\n", getpid(), g_val, &g_val);
            sleep(2);
        }    
    }
    return 0;
 }
