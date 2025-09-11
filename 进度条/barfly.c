#include <unistd.h>
#include <string.h>
#include <stdio.h>

int main() {
    int i = 0;
    char bar[101];  // 100 chars + '\0'
    memset(bar, 0, sizeof(bar));
    const char *lable = "|/-\\";

    while (i <= 100) {
        printf("[%-100s][%3d%%][%c]\r", bar, i, lable[i % 4]);
        fflush(stdout);
        if (i < 100) bar[i] = '#';
        bar[i + 1] = '\0';
        usleep(10000);
        i++;
    }

    printf("\n");
    return 0;
}


