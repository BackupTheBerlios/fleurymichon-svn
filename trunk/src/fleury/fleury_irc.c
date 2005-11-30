#include "fleury_irc.h"

void fleury_irc_process(int fd, struct s_cl *pcl)
{
  FILE *in;
  FILE *out;
  int fd2;
  char fleury_irc_cmd[64];
  char fleury_irc_param[256];

  in = fdopen(fd, "r");
  fd2 = dup(fd);
  out = fdopen(fd2, "w");

  fscanf(in, "%64s %256s\n", fleury_irc_cmd, fleury_irc_param);

  fclose(in);

  if (!strcmp(fleury_irc_cmd, "PASS"))
    {
      sscanf(fleury_irc_param, ":%128s", pcl->pass);
#ifdef FLEURY_DEBUG
      fprintf(dbgout, "Fleury: PASS %s\n", pcl->pass);
#endif
    }
  else
    {
      if (!strcmp(fleury_irc_cmd, "NICK"))
	{
#ifdef FLEURY_DEBUG
	  fprintf(dbgout, "Fleury: NICK\n");
#endif
	}
      else
	{
	  if (!strcmp(fleury_irc_cmd, "USER"))
	    {
#ifdef FLEURY_DEBUG
	      fprintf(dbgout, "Fleury: USER\n");
#endif
	    }
	  else
	    {
	      if (!strcmp(fleury_irc_cmd, "PONG"))
		{
#ifdef FLEURY_DEBUG
		  fprintf(dbgout, "Fleury: PONG\n");
#endif
		}
	      else
		{
		  if (!strcmp(fleury_irc_cmd, "QUIT"))
		    {
#ifdef FLEURY_DEBUG
		      fprintf(dbgout, "Fleury: QUIT\n");
#endif
		    }
		  else
		    {
#ifdef FLEURY_DEBUG
		      fprintf(dbgout, "Fleury: Unmatched command");
#endif
		    }
		}
	    }
	}
    }

  fflush(out);
  fclose(out);
}
