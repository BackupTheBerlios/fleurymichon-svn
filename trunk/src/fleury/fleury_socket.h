#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "fleury_types.h"

void fleury_server_start(int);
int fleury_server_tcp(int);
int fleury_server_wait(int);
void *fleury_server_listen(void *);
void fleury_server_end(int);
void fleury_socket_disconnect(struct s_cl *);

#ifdef FLEURY_DEBUG
void fleury_socket_debug_server(void);
void fleury_socket_debug_client(struct s_cl *);
#endif
