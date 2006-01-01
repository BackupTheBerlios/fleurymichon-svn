#include "fleury_socket.h"
#include "fleury_thread.h"
#include "fleury_types.h"

void fleury_server_start(int port)
{
  int cl_id;
  struct sigaction end_action;
  void *ret;

  cl_id = 0;

  end_action.sa_handler = fleury_server_end;
  sigemptyset(&end_action.sa_mask);
  end_action.sa_flags = 0;
  sigaction(SIGINT, &end_action, NULL);
  sigaction(SIGKILL, &end_action, NULL);
  sigaction(SIGQUIT, &end_action, NULL);
  sigaction(SIGABRT, &end_action, NULL);

  fleury_conf.pon = 0;
  fleury_conf.pou = 1;
  fleury_conf.pto = 10;
  fleury_conf.over = 0;
  fleury_conf.list_cl = NULL;
  fleury_conf.list_ch = NULL;
  fleury_conf.fleury_fd = fleury_server_tcp(port);
#ifdef FLEURY_DEBUG
  fleury_socket_debug_server();
#endif
  gethostname(fleury_conf.hostname, FLEURY_SZ_HOSTNAME);
  getdomainname(fleury_conf.domain, FLEURY_SZ_HOSTNAME);
  if ((strlen(fleury_conf.domain) > 0) && !strncmp(fleury_conf.domain, "(none)", 6))
    {
      sprintf(fleury_conf.host, "%s.%s", fleury_conf.hostname, fleury_conf.domain);
    }
  else
    {
      sprintf(fleury_conf.host, "%s", fleury_conf.hostname);
    }

  pthread_create(&(fleury_conf.lt), NULL, fleury_server_listen, NULL);
#ifdef FLEURY_DEBUG
  fprintf(dbgout, "Fleury: [%lu] Server listening on %s:%d\n", (unsigned long)(fleury_conf.lt), fleury_conf.host, port);
#endif
  pthread_join(fleury_conf.lt, &ret);
	
/*
      struct sockaddr_in addr;
      size_t sz_addr;

      sz_addr = sizeof(addr);

      getsockname(cl_fd, (struct sockaddr *) &addr, &sz_addr);
	      close(cl_fd);
	      pthread_mutex_unlock(&tasks[k].lock);
	      printf("Fleury: Client %d [%s] processed by task %d\n", cl_id, inet_ntoa(addr.sin_addr), k);
  */

}

int fleury_server_tcp(int port)
{
  int fd;
  struct sockaddr_in server_addr;
  size_t sz_server_addr;

  sz_server_addr = sizeof(server_addr);

  fd = socket(AF_INET, SOCK_STREAM, 0);
  if (fd < 0)
    {
      FATALBUG("Fleury: Socket error");
    }

  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = INADDR_ANY;
  server_addr.sin_port = htons(port);

  if (bind(fd, (struct sockaddr *)&server_addr, sz_server_addr) < 0)
    {
#ifdef FLEURY_DEBUG
      fprintf(dbgout, "Fleury: Socket bind error\n");
#endif
    }

  listen(fd, 4);

  return fd;
}

void *fleury_server_listen(void *data)
{
  int cl_fd;

  data = NULL;
  
  while (!fleury_conf.over)
    {
      while (!fleury_conf.over && (cl_fd = accept(fleury_conf.fleury_fd, NULL, NULL)) < 0)
	{
	  if (errno != EINTR)
	    {
#ifdef FLEURY_DEBUG
	      fprintf(dbgout, "Fleury: Socket accept error\n");
#endif
	    }
	}
      if (!fleury_conf.over)
	{
	  fleury_thread_init(cl_fd);
	}
    }

#ifdef FLEURY_DEBUG
  fprintf(dbgout, "Fleury: [%lu] No more listening\n", (unsigned long)(fleury_conf.lt));
  fflush(dbgout);
#endif

  return NULL;
}

void fleury_server_end(int sig)
{
  fleury_conf.over = 1;
  shutdown(fleury_conf.fleury_fd, 2);
  close(fleury_conf.fleury_fd);
#ifdef FLEURY_DEBUG
  fprintf(dbgout, "Fleury: Server terminated (%s)\n", sys_siglist[sig]);
  fflush(dbgout);
#endif
  /* exit(EXIT_SUCCESS); */
}

void fleury_socket_disconnect(struct s_cl *pcl)
{
  pcl->connected = 0;
  fflush(pcl->out);
  fclose(pcl->in);
  fclose(pcl->out);
  shutdown(pcl->fd2, 2);
  shutdown(pcl->fd, 2);
  close(pcl->fd2);
  close(pcl->fd);
}

#ifdef FLEURY_DEBUG
void fleury_socket_debug_server()
{
  fprintf(dbgout, "Fleury: Server file descriptor is %d\n", fleury_conf.fleury_fd);
}

void fleury_socket_debug_client(struct s_cl *pcl)
{
  fprintf(dbgout, "Fleury: [%lu] Client sockets: fd=%d fd2=%d in=%p out=%p\n", (unsigned long)(pcl->tid), pcl->fd, pcl->fd2, pcl->in, pcl->out);
}
#endif
