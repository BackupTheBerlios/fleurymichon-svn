#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include "fleury_socket.h"
#include "fleury_types.h"

void fleury_irc_process(struct s_cl *);
void fleury_irc_ping(struct s_cl *, char *);
void fleury_irc_logged(struct s_cl *);
char *fleury_irc_last(char *);

int test_streq_cl_user(void *, void *);
int test_streq_cl(void *, void *);
int test_streq_ch_chan(void *, void *);
int test_streq_ch(void *, void *);
int test_strcmp_ch_chan(void *, void *);
int test_strcmp_ch(void *, void *);
int test_strcmp_cl_user(void *, void *);
int test_strcmp_cl(void *, void *);
int test_pthread_cl(void *, void *);
