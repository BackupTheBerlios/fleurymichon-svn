#ifndef FLEURY_TYPES_H

#define FLEURY_TYPES_H 1

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define FATALBUG(err) { perror((char *)err); exit(EXIT_FAILURE); }

#define FLEURY_CRLF "\r\n"
#define FLEURY_MAX_TASKS 5
#define FLEURY_SZ_PASS 128
#define FLEURY_SZ_NICK 128
#define FLEURY_SZ_USER 128
#define FLEURY_SZ_NAME 128

struct s_cl
{
  char pass[FLEURY_SZ_PASS];
  char nick[FLEURY_SZ_NICK];
  char user[FLEURY_SZ_USER];
  char name[FLEURY_SZ_USER];
};

struct s_task
{
  pthread_t id;
  pthread_mutex_t lock;
  int active;
  int fd;
  struct s_cl cl;
};

#ifdef FLEURY_DEBUG
FILE *dbgout;
#endif

int fleury_fd;
struct s_task tasks[FLEURY_MAX_TASKS];

#endif
