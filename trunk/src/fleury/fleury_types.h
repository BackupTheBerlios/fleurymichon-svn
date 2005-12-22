#ifndef FLEURY_TYPES_H

#define FLEURY_TYPES_H 1

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include "gen_list.h"

#define FATALBUG(err) { perror((char *)err); exit(EXIT_FAILURE); }

#define FLEURY_CRLF "\r\n"
/* #define FLEURY_MAX_TASKS 5 */
#define FLEURY_SZ_PASS 128
#define FLEURY_SZ_NICK 128
#define FLEURY_SZ_USER 128
#define FLEURY_SZ_NAME 128
#define FLEURY_SZ_BUFFER 1024
#define FLEURY_SZ_HOSTNAME 128

struct s_mode_cl
{
  unsigned int i: 1;
  unsigned int s: 1;
  unsigned int w: 1;
  unsigned int o: 1;
};

struct s_cl
{
  pthread_t tid;
  pthread_mutex_t lock;
  int connected;
  int logged;
  int fd;
  int fd2;
  FILE *in;
  FILE *out;
  time_t pingtime;
  char pass[FLEURY_SZ_PASS];
  char nick[FLEURY_SZ_NICK];
  char user[FLEURY_SZ_USER];
  char name[FLEURY_SZ_NAME];
  char pingstr[FLEURY_SZ_HOSTNAME];
  char buffer[FLEURY_SZ_BUFFER];
  struct s_mode_cl mode;
};

struct s_fleury_conf
{
  int over;
  int fleury_fd;
  t_list list_cl;
  char hostname[FLEURY_SZ_HOSTNAME];
  char domain[FLEURY_SZ_HOSTNAME];
  char host[FLEURY_SZ_HOSTNAME * 2];
  int pon;
  int pou;
  int pto;
} fleury_conf;

#ifdef FLEURY_DEBUG
FILE *dbgout;
#endif

#endif
