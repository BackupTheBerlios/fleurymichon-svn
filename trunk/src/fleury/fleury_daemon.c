#include "fleury_daemon.h"
#include "fleury_types.h"
#include "fleury_load.h"

int let_fleury_be_daemon()
{
  int fd;
  FILE *cfg;

  if (fork())
    {
      exit(EXIT_SUCCESS);
    }
  setsid();
  if (fork())
    {
      exit(EXIT_SUCCESS);
    }

#ifdef FLEURY_DEBUG
  dbgout = fopen("fleury_debug.log", "a");
  fprintf(dbgout, "Fleury: Logging on\n");
#endif

  if ((cfg = fopen("fleury.cfg", "r")))
    {
      fleury_load_param(&fleury_conf, cfg);
      fclose(cfg);
    }
  

  chdir("/");
  fd = open("/dev/null", O_RDWR);
  dup2(fd, 0);
  close(fd);
  fd = open("/dev/console", O_WRONLY);
  dup2(fd, 1);
  dup2(fd, 2);
  close(fd);

  return 0;
}
