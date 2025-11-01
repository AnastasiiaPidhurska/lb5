#define _POSIX_C_SOURCE 200809L
#include "sync.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/wait.h>
#include <time.h>

static void child_work_and_signal(int write_fd, int idx) {
    struct timespec ts;
    unsigned seed = (unsigned)(time(NULL) ^ (getpid()<<16) ^ (idx*1103515245u));
    int delay_ms = 100 + (seed % 600);
    ts.tv_sec  = delay_ms / 1000;
    ts.tv_nsec = (delay_ms % 1000) * 1000000L;
    nanosleep(&ts, NULL);
    dprintf(STDERR_FILENO, "[child %d | pid=%d] work done after %d ms\n", idx, (int)getpid(), delay_ms);
    const char token = 1;
    ssize_t wr;
    do { wr = write(write_fd, &token, 1); } while (wr == -1 && errno == EINTR);
    if (wr != 1) _exit(2);
    close(write_fd);
    _exit(0);
}

int spawn_children_and_work(int n, int fds[2]) {
    for (int i = 0; i < n; ++i) {
        pid_t pid = fork();
        if (pid < 0) return -1;
        else if (pid == 0) {
            close(fds[0]);
            child_work_and_signal(fds[1], i);
        }
    }
    close(fds[1]);
    return 0;
}

int wait_for_children_tokens(int n, int fd_r) {
    int got = 0;
    char buf;
    while (got < n) {
        ssize_t rd = read(fd_r, &buf, 1);
        if (rd == 1) ++got;
        else if (rd == 0) { errno = EPIPE; return -1; }
        else { if (errno == EINTR) continue; return -1; }
    }
    return 0;
}

int reap_children(int n) {
    int status;
    for (int i = 0; i < n; ++i) {
        pid_t rc = wait(&status);
        if (rc < 0) return -1;
    }
    return 0;
}
