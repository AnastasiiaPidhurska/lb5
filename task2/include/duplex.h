#ifndef DUPLEX_H
#define DUPLEX_H

int create_pipes(int p1[2], int p2[2]);
void run_parent(int p1[2], int p2[2]);
void run_child(int p1[2], int p2[2]);

#endif
