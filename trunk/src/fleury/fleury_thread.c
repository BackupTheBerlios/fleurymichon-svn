#include "fleury_thread.h"
#include "fleury_irc.h"
#include "fleury_types.h"

void fleury_tasks()
{
  int k;

  for (k = 0; k < FLEURY_MAX_TASKS; k++)
    {
      struct s_task *pt;
      pt = tasks + k;
      pt->active = 0;
      /* sprintf(pt->cl.pass, "");
      sprintf(pt->cl.nick, "");
      sprintf(pt->cl.user, "");
      sprintf(pt->cl.name, ""); */
      *(pt->cl.pass) = 0;
      *(pt->cl.nick) = 0;
      *(pt->cl.user) = 0;
      *(pt->cl.name) = 0;
      pthread_mutex_init(&pt->lock, NULL);
      pthread_mutex_lock(&pt->lock);
      pthread_create(&pt->id, NULL, fleury_task_work, (void *)pt);
    }
}

void *fleury_task_work(void *data)
{
  struct s_task *pt;

  pt = data;

  while (1)
    {
      pthread_mutex_lock(&pt->lock);
      fleury_irc_process(pt->fd, &pt->cl);
      close(pt->fd);
      pt->active = 0;
    }
  return NULL;
}
