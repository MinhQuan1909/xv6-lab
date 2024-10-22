#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/param.h"

#define MAX_BUF 128

void execute_command(char *cmd, char *args[]) {
    if (fork() == 0) {
        exec(cmd, args);
        exit(0);
    } 
		else {
        wait(0);
    }
}

void read_execute(char *cmd, char *args[], int arg_start) {
    char buf[MAX_BUF];
    int pos = 0;
    char c;

    while (read(0, &c, 1) > 0) {
        if (c == '\n') {
            buf[pos] = '\0';
            args[arg_start] = buf;
            execute_command(cmd, args);
            pos = 0;
        } 
				else {
            buf[pos++] = c;
        }
    }
}

int main(int argc, char *argv[]) {
    int index = (strcmp(argv[1], "-n") == 0) ? 3 : 1;
    char *cmd = argv[index];
    char *args[MAXARG];

    for (int i = index; i < argc; ++i) {
        args[i - index] = argv[i];
    }
    int arg_start = argc - index;
    args[arg_start + 1] = 0;

    read_execute(cmd, args, arg_start);
    exit(0);
}