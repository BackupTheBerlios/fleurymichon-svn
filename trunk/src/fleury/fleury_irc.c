#include "fleury_irc.h"

void fleury_irc_process(struct s_cl *pcl)
{
  char fleury_irc_cmd[64];
  char *fleury_irc_param;

  pcl->buffer[0] = 0;
  fleury_irc_cmd[0] = 0;

  if (pcl->logged && pcl->pingtime && ((time(NULL) - pcl->pingtime) > fleury_conf.pto))
    {
      /* ping timeout here */
      fprintf(pcl->out, "Ping timeout\n");
      pcl->connected = 0;
      fflush(pcl->out);
      fclose(pcl->in);
      fclose(pcl->out);
      shutdown(pcl->fd2, 2);
      shutdown(pcl->fd, 2);
      close(pcl->fd2);
      close(pcl->fd);
	     
#ifdef FLEURY_DEBUG
      fprintf(dbgout, "Fleury: Ping timeout\n"); 
#endif
    }

  fgets(pcl->buffer, FLEURY_SZ_BUFFER, pcl->in);

  fleury_irc_param = pcl->buffer;
  while(*fleury_irc_param && (*fleury_irc_param != '\r') && (*fleury_irc_param != '\n'))
    {
      fleury_irc_param++;
    }
  *fleury_irc_param = 0;

  sscanf(pcl->buffer, "%64s\n", fleury_irc_cmd);

  if (pcl->buffer[0] && fleury_irc_cmd[0])
    {
      fleury_irc_param = pcl->buffer + 1;
      while (*fleury_irc_param && (*fleury_irc_param != ' '))
	{
	  fleury_irc_param++;
	}
      if (*fleury_irc_param)
	{
	  fleury_irc_param++;
	}
   
      fprintf(pcl->out, "%s\r\n", fleury_irc_cmd);
      if (!strcmp(fleury_irc_cmd, "PASS"))
	{
	  sscanf(fleury_irc_param, "%64s\n", pcl->pass);
#ifdef FLEURY_DEBUG
	  fprintf(dbgout, "Fleury: PASS (%s)\n", fleury_irc_param); 
#endif
	}
      else
	{
	  if (!strcmp(fleury_irc_cmd, "NICK"))
	    {
	      sscanf(fleury_irc_param, "%64s\n", pcl->nick);
	      if (fleury_conf.pon)
		{
		  fleury_irc_ping(pcl, fleury_conf.hostname);
		} 
#ifdef FLEURY_DEBUG
	      fprintf(dbgout, "Fleury: NICK (%s)\n", fleury_irc_param);
#endif
	    }
	  else
	    {
	      if (!strcmp(fleury_irc_cmd, "USER"))
		{
		  sscanf(fleury_irc_param, "%64s\n", pcl->user);
		  if (pcl->nick[0] && pcl->user[0])
		    {
		      pcl->logged = 1;
		    }
		  if (fleury_conf.pou)
		    {
		      fleury_irc_ping(pcl, fleury_conf.hostname);
		    } 
#ifdef FLEURY_DEBUG
		  fprintf(dbgout, "Fleury: USER (%s)\n", fleury_irc_param);
#endif
		}
	      else
		{
		  if (!strcmp(fleury_irc_cmd, "PONG"))
		    {
		      if (!strcmp(fleury_irc_param, pcl->pingstr) || (*fleury_irc_param == ':' && !strcmp(fleury_irc_param + 1, pcl->pingstr)))
			{
			  *(pcl->pingstr) = 0;
			  pcl->pingtime = 0;
			}
#ifdef FLEURY_DEBUG
		      fprintf(dbgout, "Fleury: PONG (%s)\n", fleury_irc_param);
#endif
		    }
		  else
		    {
		      if (!strcmp(fleury_irc_cmd, "QUIT"))
			{
#ifdef FLEURY_DEBUG
			  fprintf(dbgout, "Fleury: QUIT (%s)\n", fleury_irc_param);
			  
#endif
			}
		      else
			{
#ifdef FLEURY_DEBUG
			  fprintf(dbgout, "Fleury: Unmatched command %s (%s)\n", fleury_irc_cmd, fleury_irc_param);
#endif
			}
		    }
		}
	    }
	}
    }

#ifdef FLEURY_DEBUG
  fflush(dbgout);
#endif

  fflush(pcl->out);
}

void fleury_irc_ping(struct s_cl *pcl, char *s)
{
  fprintf(pcl->out, "PING :%s\n", s);
  strcpy(pcl->pingstr, s);
  pcl->pingtime = time(NULL);
}
