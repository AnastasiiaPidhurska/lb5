#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include "sync.h"

static void usage(const char *prog) {
    fprintf(stderr, "Usage: %s <n_children>\n", prog);
}

int main(int argc, char **argv) {
    if (argc != 2) { usage(argv[0]); return EXIT_FAILURE; }
    char *end = NULL;
    long n = strtol(argv[1], &end, 10);
    if (end == argv[1] || *end != '\0' || n <= 0 || n > 10000) {
        fprintf(stderr, "Invalid n_children\n"); return EXIT_FAILURE;
    }

    int fds[2];
    if (pipe(fds) == -1) { perror("pipe"); return EXIT_FAILURE; }

    fprintf(stderr, "[parent pid=%d] forking %ld children...\n", (int)getpid(), n);
    if (spawn_children_and_work((int)n, fds) < 0) { perror("fork"); return EXIT_FAILURE; }

    if (wait_for_children_tokens((int)n, fds[0]) == -1) {
        perror("wait_for_children_tokens"); close(fds[0]); reap_children((int)n); return EXIT_FAILURE;
    }

    fprintf(stderr, "[parent] received %ld/%ld tokens\n", n, n);
    close(fds[0]);
    reap_children((int)n);
    printf("Parent final action: ALL children finished. ✅\n");
    return EXIT_SUCCESS;
}
