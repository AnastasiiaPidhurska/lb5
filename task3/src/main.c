#include <stdio.h>
#include <string.h>
#include "fifo_chat.h"

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s [writer|reader]\n", argv[0]);
        return 1;
    }
    if (strcmp(argv[1], "writer") == 0) run_writer();
    else if (strcmp(argv[1], "reader") == 0) run_reader();
    else printf("Invalid argument.\n");
    return 0;
}
