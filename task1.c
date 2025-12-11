#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/time.h>
#include <time.h>
#include <sys/wait.h>

void print_info(const char *name) {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    struct tm *tm_info = localtime(&tv.tv_sec);

    int hours   = tm_info->tm_hour;
    int minutes = tm_info->tm_min;
    int seconds = tm_info->tm_sec;
    long millis = tv.tv_usec / 1000;

    printf("[%s] pid=%d, ppid=%d, time=%02d:%02d:%02d:%03ld\n",
           name, (int)getpid(), (int)getppid(),
           hours, minutes, seconds, millis);
    fflush(stdout);
}

int main(void) {
    pid_t pid1, pid2;

    printf("Initially only one process exists\n");

    pid1 = fork();

    if (pid1 == 0) {
        print_info("Child 1");
        sleep(5);
        return 0;
    }

    pid2 = fork();

    if (pid2 == 0) {
        print_info("Child 2");
        sleep(5);
        return 0;
    }

    print_info("Parent");
    sleep(1);

    printf("\n=== Running ps -x from parent ===\n");
    system("ps -x");

    int status;
    waitpid(pid1, &status, 0);
    waitpid(pid2, &status, 0);

    printf("Parent: all child processes finished\n");

    return 0;
}
