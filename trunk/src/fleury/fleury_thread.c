#include "fleury_thread.h"
#include "fleury_irc.h"
#include "fleury_socket.h"
#include "fleury_types.h"

void fleury_thread_init(int fd)
{
  struct s_cl cl;
  struct s_cl* pcl;
  struct sockaddr_in addr;
  size_t sz_addr;

  sz_addr = sizeof(addr);

  cl.fd = fd;
  *(cl.pass) = 0;
  *(cl.nick) = 0;
  *(cl.user) = 0;
  *(cl.name) = 0;
  *(cl.pingstr) = 0;
  cl.pingtime = 0;
  cl.logged = 0;
  cl.connected = 1;
  cl.mode.i = 0;
  cl.mode.s = 0;
  cl.mode.w = 0;
  cl.mode.o = 0;
  cl.list_chans = NULL;
  fleury_conf.list_cl = list_add_tail(fleury_conf.list_cl, &cl, sizeof(struct s_cl));
  pcl = list_last(fleury_conf.list_cl);
  pcl->in = fdopen(pcl->fd, "r");
  pcl->fd2 = dup(pcl->fd);
  pcl->out = fdopen(pcl->fd2, "w");
  pthread_create(&(pcl->tid), NULL, fleury_thread_proc, pcl);
  getsockname(pcl->fd, (struct sockaddr *) &addr, &sz_addr);
  strcpy(pcl->host, inet_ntoa(addr.sin_addr));
#ifdef FLEURY_DEBUG
  fprintf(dbgout, "Fleury: [%lu] %s connected\n", (unsigned long)(pcl->tid), pcl->host);
  /* fleury_socket_debug_client(pcl); */
  fprintf(dbgout, "Fleury: Total connections %d/%d\n", list_length(fleury_conf.list_cl), FLEURY_MAX_CONNECTIONS);  
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
  
  while (!fleury_conf.over && pcl->connected)
    {
      if (fileno(pcl->in) == -1)
	{

#ifdef FLEURY_DEBUG
	  fprintf(dbgout, "Fleury: [%lu] Invalid input stream\n", (unsigned long)(pcl->tid));
	  fflush(dbgout);
#endif 

	  break;
	}

#ifdef FLEURY_DEBUG
      /* fprintf(dbgout, "Fleury: [%lu] Iteration\n", (unsigned long)(pcl->tid)); */
#endif

      fleury_irc_process(pcl);   
                       
    }
  /*
  if (!fleury_conf.over)
    {
      int test(void *elt)
	{
	  struct s_cl *cl;

	  cl = (struct s_cl *)&elt;
	  return cl->tid = pcl->tid;	   
	}	
      
      fleury_conf.list_cl = list_del(fleury_conf.list_cl, test);
    }
  */

  if (pcl->connected)
    {
      fleury_socket_disconnect(pcl);
    }

#ifdef FLEURY_DEBUG
  fprintf(dbgout, "Fleury: [%lu] %s closed\n", (unsigned long)(pcl->tid), pcl->host);
#endif

  int test(void *p)
    {
      struct s_cl *cl;
      
      cl = (struct s_cl *)p;
      return (cl->tid == pcl->tid);	   
    }	
  
  fleury_conf.list_cl = list_del(fleury_conf.list_cl, test);

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
