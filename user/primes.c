#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
void prime_sieve_pipe(int fd) {
    int num;
    if (read(fd, &num, 4) <= 0) {
        close(fd);
        return;
    }
    printf("prime %d\n", num);

    int p[2];
    if (pipe(p) < 0) {
        fprintf(2, "pipe failed\n");
        exit(1);
    }

    if (fork() == 0) {
        close(p[1]);
        close(fd);
        prime_sieve_pipe(p[0]);
        exit(0);
    } 
		else {
        close(p[0]);
        int tmp;
        while (read(fd, &tmp, 4) > 0) {
            if (tmp % num != 0) {
                write(p[1], &tmp, 4);
            }
        }
        close(p[1]);
        close(fd);
        wait(0);
    }
}

int main(int argc, char* argv[]) {
    int p[2];
    if (pipe(p) < 0) {
        fprintf(2, "pipe failed\n");
        exit(1);
    }

    if (fork() == 0) {
        close(p[1]);
        prime_sieve_pipe(p[0]);
        exit(0);
    } 
		else {
        close(p[0]);
        for (int i = 2; i < 281; i++) {
            write(p[1], &i, 4);
        }
        close(p[1]);
        wait(0);
    }

    exit(0);
}