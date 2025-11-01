#define _POSIX_C_SOURCE 200809L
#include "fifo_chat.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <ctype.h>
#include <sys/stat.h>

static void str_to_upper(char *s) {
    for (int i = 0; s[i]; ++i)
        s[i] = (char)toupper((unsigned char)s[i]);
}

void run_writer(void) {
    mkfifo(FIFO1, 0666);
    mkfifo(FIFO2, 0666);
    int fdw = open(FIFO1, O_WRONLY);
    int fdr = open(FIFO2, O_RDONLY);
    char buf[256], reply[256];
    while (1) {
        printf("Enter text (stop to exit): ");
        fflush(stdout);
        if (!fgets(buf, sizeof(buf), stdin)) break;
        buf[strcspn(buf, "\n")] = '\0';
        write(fdw, buf, strlen(buf));
        if (strncmp(buf, "stop", 4) == 0) break;
        ssize_t n = read(fdr, reply, sizeof(reply) - 1);
        if (n > 0) {
            reply[n] = '\0';
            printf("Reader: %s\n", reply);
        }
    }
    close(fdw);
    close(fdr);
    unlink(FIFO1);
    unlink(FIFO2);
}

void run_reader(void) {
    mkfifo(FIFO1, 0666);
    mkfifo(FIFO2, 0666);
    int fdr = open(FIFO1, O_RDONLY);
    int fdw = open(FIFO2, O_WRONLY);
    char buf[256];
    ssize_t n;
    while ((n = read(fdr, buf, sizeof(buf) - 1)) > 0) {
        buf[n] = '\0';
        if (strncmp(buf, "stop", 4) == 0) break;
        str_to_upper(buf);
        write(fdw, buf, strlen(buf));
    }
    close(fdr);
    close(fdw);
}
