#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <sys/wait.h>

#define PI 3.14159265358979323846

double compute_term(int k, double x) {
    int p = 2 * k + 1;

    double fact = 1.0;
    for (int j = 2; j <= p; j++)
        fact *= j;

    double power = 1.0;
    for (int j = 0; j < p; j++)
        power *= x;

    double sign = (k % 2 == 0) ? 1.0 : -1.0;

    return sign * power / fact;
}

int main() {
    int K, N, n;

    printf("Enter K: ");
    scanf("%d", &K);

    printf("Enter N: ");
    scanf("%d", &N);

    printf("Enter n (Taylor terms): ");
    scanf("%d", &n);

    FILE *f = fopen("results.txt", "w");
    if (!f) {
        perror("fopen");
        return 1;
    }

    for (int i = 0; i < K; i++) {
        double x = 2.0 * PI * i / N;
        double sum = 0.0;

        for (int k = 0; k < n; k++) {

            int fd[2];
            pipe(fd);

            pid_t pid = fork();

            if (pid == 0) {
                close(fd[0]);

                double term = compute_term(k, x);

                printf("Child process: pid=%d, ppid=%d, i=%d, term=%d, value=%lf\n",
                       getpid(), getppid(), i, k, term);

                write(fd[1], &term, sizeof(double));
                close(fd[1]);
                _exit(0);
            }
            else if (pid > 0) {
                close(fd[1]);

                double term_value;
                read(fd[0], &term_value, sizeof(double));
                close(fd[0]);

                wait(NULL);

                sum += term_value;
            }
            else {
                perror("fork");
                exit(1);
            }
        }

        fprintf(f, "i=%d  x=%lf  y=%lf\n", i, x, sum);
    }

    fclose(f);

    printf("Results written to results.txt\n");
    return 0;
}
