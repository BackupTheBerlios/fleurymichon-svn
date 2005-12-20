#include "fleury_thread.h"
#include "fleury_irc.h"
#include "fleury_types.h"

void fleury_thread_init(int fd)
{
  struct s_cl cl;
  struct s_cl* pcl;

  cl.fd = fd;
  *(cl.pass) = 0;
  *(cl.nick) = 0;
  *(cl.user) = 0;
  *(cl.name) = 0;
  list_cl = list_add_tail(list_cl, &cl, sizeof(struct s_cl));
  pcl = list_last(list_cl);
  pcl->in = fdopen(pcl->fd, "r");
  pcl->fd2 = dup(pcl->fd);
  pcl->out = fdopen(pcl->fd2, "w");
  pthread_create(&(pcl->tid), NULL, fleury_thread_proc, pcl);
#ifdef FLEURY_DEBUG
  fprintf(dbgout, "Fleury: New client thread %lu\n", pcl->tid);
#endif
}

/* ne pas oublier fclose(in), fflush(out)?, fclose(out) */

/* void fleury_tasks()
{
  int k;

  for (k = 0; k < FLEURY_MAX_TASKS; k++)
    {
      struct s_task *pt;
      pt = tasks + k;
      pt->active = 0;
      sprintf(pt->cl.pass, "");
      sprintf(pt->cl.nick, "");
      sprintf(pt->cl.user, "");
      sprintf(pt->cl.name, "");
      *(pt->cl.pass) = 0;
      *(pt->cl.nick) = 0;
      *(pt->cl.user) = 0;
      *(pt->cl.name) = 0;
      pthread_mutex_init(&pt->lock, NULL);
      pthread_mutex_lock(&pt->lock);
      pthread_create(&pt->id, NULL, fleury_task_work, (void *)pt);
    }
} */

void *fleury_thread_proc(void *data)
{
  struct s_cl *pcl;

  pcl = data;
  
  while (1)
    {
      if (fileno(pcl->in) == -1)
	{

#ifdef FLEURY_DEBUG
	  fprintf(dbgout, "Fleury: Invalid input stream on thread %lu\n", pcl->tid);
	  fflush(dbgout);
#endif 

	  break;
	}

#ifdef FLEURY_DEBUG
      /* fprintf(dbgout, "Fleury: Iteration on thread %lu\n", pcl->tid); */
#endif

      fleury_irc_process(pcl);
    }

  return NULL;
}

/* void *fleury_task_work(void *data)
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
} */
