#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include "fleury_socket.h"
#include "fleury_types.h"

void fleury_irc_process(struct s_cl *);
void fleury_irc_ping(struct s_cl *, char *);
void fleury_irc_logged(struct s_cl *);
