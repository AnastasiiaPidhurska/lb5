#ifndef SYNC_H
#define SYNC_H

#include <sys/types.h>

int spawn_children_and_work(int n, int fds[2]);
int wait_for_children_tokens(int n, int fd_r);
int reap_children(int n);

#endif
