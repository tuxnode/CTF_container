#include <stdio.h>
#include <signal.h>
#include <unistd.h>

const int correct[] = {SIGUSR1, SIGUSR2, SIGUSR1, SIGUSR1, SIGUSR2};
int step = 0;

void handler(int sig) {
    if (sig == correct[step]) {
        step++;
        printf("[%d/5] 正确！继续...\n", step);
        fflush(stdout);
        if (step == 5) {
            FILE *f = fopen("/flag.txt", "r");
            char flag[64];
            fgets(flag, 64, f);
            printf("flag: %s\n", flag);
            fflush(stdout);
            _exit(0);
        }
    } else {
        printf("错误！重新开始\n");
        fflush(stdout);
        step = 0;
    }
}

int main() {
    printf("PID: %d\n", getpid());
    fflush(stdout);
    signal(SIGUSR1, handler);
    signal(SIGUSR2, handler);
    while(1) pause();
    printf("Fine!");
}
