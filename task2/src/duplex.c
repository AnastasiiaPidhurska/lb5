#define _POSIX_C_SOURCE 200809L
#include "duplex.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <errno.h>

int create_pipes(int p1[2], int p2[2]) {
    if (pipe(p1) == -1) return -1;
    if (pipe(p2) == -1) { close(p1[0]); close(p1[1]); return -1; }
    return 0;
}

static void str_to_upper(char *s) {
    for (int i = 0; s[i]; ++i) s[i] = (char)toupper((unsigned char)s[i]);
}

void run_child(int p1[2], int p2[2]) {
    close(p1[1]);
    close(p2[0]);
    char buf[256];
    ssize_t n;
    while ((n = read(p1[0], buf, sizeof(buf)-1)) > 0) {
        buf[n] = '\0';
        if (strncmp(buf, "stop", 4) == 0) break;
        str_to_upper(buf);
        write(p2[1], buf, strlen(buf));
    }
    close(p1[0]);
    close(p2[1]);
    _exit(0);
}

void run_parent(int p1[2], int p2[2]) {
    close(p1[0]);
    close(p2[1]);
    char input[256], buf[256];
    while (1) {
        printf("Enter text (stop to exit): ");
        fflush(stdout);
        if (!fgets(input, sizeof(input), stdin)) break;
        input[strcspn(input, "\n")] = '\0';
        write(p1[1], input, strlen(input));
        if (strncmp(input, "stop", 4) == 0) break;
        ssize_t n = read(p2[0], buf, sizeof(buf)-1);
        if (n > 0) { buf[n] = '\0'; printf("Child: %s\n", buf); }
    }
    close(p1[1]);
    close(p2[0]);
}
