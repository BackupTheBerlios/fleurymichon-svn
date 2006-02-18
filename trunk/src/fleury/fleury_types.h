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
#define FLEURY_SZ_CHAN 128
#define FLEURY_SZ_TOPIC 1024
#define FLEURY_SZ_BUFFER 1024
#define FLEURY_SZ_HOSTNAME 128
#define FLEURY_MAX_CONNECTIONS 64

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
  int ibuf;
  int fd;
  int fd2;
  FILE *in;
  FILE *out;
  time_t pingtime;
  char pass[FLEURY_SZ_PASS];
  char nick[FLEURY_SZ_NICK];
  char user[FLEURY_SZ_USER];
  char name[FLEURY_SZ_NAME];
  char host[FLEURY_SZ_HOSTNAME];
  char pingstr[FLEURY_SZ_HOSTNAME];
  char buffer[FLEURY_SZ_BUFFER];
  t_list list_chans;
  struct s_mode_cl mode;
};

struct s_mode_ch
{
  unsigned int n: 1;
  unsigned int t: 1;
  unsigned int r: 1;
};

struct s_ch
{
  char name[FLEURY_SZ_CHAN];
  char topic[FLEURY_SZ_TOPIC];
  char pass[FLEURY_SZ_PASS];
  time_t date;
  t_list list_users;
  t_list list_ban;
  struct s_mode_ch mode;
};

struct s_ch_user
{
  struct s_cl* pcl;
  char status;
};

struct s_user_ch
{
  struct s_ch* pch;
  char status;
};

struct s_fleury_conf
{
  pthread_t lt;
  int over;
  int fleury_fd;
  char hostname[FLEURY_SZ_HOSTNAME];
  char domain[FLEURY_SZ_HOSTNAME];
  char host[FLEURY_SZ_HOSTNAME * 2];
  int pon;
  int pou;
  int pto;
  t_list list_cl;
  t_list list_ch;
} fleury_conf;

#ifdef FLEURY_DEBUG
FILE *dbgout;
#endif

#endif
