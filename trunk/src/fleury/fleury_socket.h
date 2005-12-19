#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

void fleury_server_start(int);
int fleury_server_tcp(int);
int fleury_server_wait(int);
void *fleury_server_listen(void *);
void fleury_server_end(int);
