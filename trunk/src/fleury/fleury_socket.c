#include "fleury_socket.h"
#include "fleury_thread.h"
#include "fleury_types.h"

void fleury_server_start(int port)
{
  int cl_id;
  int cl_fd;
  struct sigaction end_action;

  cl_id = 0;

  end_action.sa_handler = fleury_server_end;
  sigemptyset(&end_action.sa_mask);
  end_action.sa_flags = 0;
  sigaction(SIGINT, &end_action, NULL);

  fleury_fd = fleury_server_tcp(port);
  fleury_tasks();
  printf("Fleury: Server listening on port %d\n", port);

  while (1)
    {
      struct sockaddr_in addr;
      size_t sz_addr;
      int k;

      sz_addr = sizeof(addr);

      printf("Fleury: Before wait\n");

      /* cl_fd = fleury_server_wait(fleury_fd); */

      cl_fd = accept(fleury_fd, NULL, NULL);

      printf("Fleury: After wait\n");
      getsockname(cl_fd, (struct sockaddr *) &addr, &sz_addr);
      cl_id++;
      for (k = 0; k < FLEURY_MAX_TASKS; k++)
	{
	  if (!(tasks[k].active))
	    {
	      break;
	    }
	  if (k == FLEURY_MAX_TASKS)
	    {
	      close(cl_fd);
	      printf("Fleury: Client %d [%s] rejected\n", cl_id, inet_ntoa(addr.sin_addr));
	    }
	  else
	    {
	      tasks[k].fd = cl_fd;
	      tasks[k].active = 1;
	      pthread_mutex_unlock(&tasks[k].lock);
	      printf("Fleury: Client %d [%s] processed by task %d\n", cl_id, inet_ntoa(addr.sin_addr), k);
	    }
	}
    }

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
      FATALBUG("Fleury: Socket bind error");
    }

  listen(fd, 4);

  return fd;
}

int fleury_server_wait(int fd)
{
  int cl_fd;

  while ((cl_fd = accept(fd, NULL, NULL)) < 0)
    {
      if (errno != EINTR)
	{
	  FATALBUG("Fleury: Socket accept error");
	}
    }
  return cl_fd;
}

void *fleury_server_listen(void *data)
{
  int cl_fd;

  while ((cl_fd = accept(*(int *)data, NULL, NULL)) < 0)
    {
      if (errno != EINTR)
	{
	  FATALBUG("Fleury: Socket accept error");
	}
    }
  return NULL;
}

void fleury_server_end(int sig)
{
  shutdown(fleury_fd, 2);
  close(fleury_fd);
#ifdef FLEURY_DEBUG
  fclose(dbgout);
#endif
  printf("Fleury: Server terminated (%s)\n", sys_siglist[sig]);
  exit(EXIT_SUCCESS);
}
