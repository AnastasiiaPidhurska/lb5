#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include "duplex.h"

int main() {
    int p1[2], p2[2];
    if (create_pipes(p1, p2) == -1) { perror("pipe"); return 1; }
    pid_t pid = fork();
    if (pid < 0) { perror("fork"); return 1; }
    else if (pid == 0) run_child(p1, p2);
    else run_parent(p1, p2);
    return 0;
}
