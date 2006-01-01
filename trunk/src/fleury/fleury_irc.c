#include "fleury_irc.h"

void fleury_irc_process(struct s_cl *pcl)
{
  char fleury_irc_cmd[64];
  char *fleury_irc_param;
  /* char *tmp; */

  /* commandes a rajouter : PING OPER NAMES PRIVMSG */

  pcl->buffer[0] = 0;
  fleury_irc_cmd[0] = 0;

  if (pcl->logged && pcl->pingtime && ((time(NULL) - pcl->pingtime) > fleury_conf.pto))
    {
      /* ping timeout here */
      fprintf(pcl->out, "Ping timeout\r\n");
      fleury_socket_disconnect(pcl);
#ifdef FLEURY_DEBUG
      fprintf(dbgout, "Fleury: [%lu] Ping timeout\n", (unsigned long)(pcl->tid));
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
   
      /* fprintf(pcl->out, "%s\r\n", fleury_irc_cmd); */
      if (!strcmp(fleury_irc_cmd, "PASS"))
	{
	  sscanf(fleury_irc_param, "%64s\n", pcl->pass);
#ifdef FLEURY_DEBUG
	  fprintf(dbgout, "Fleury: [%lu] PASS (%s)\n", (unsigned long)(pcl->tid), fleury_irc_param); 
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
	      fprintf(dbgout, "Fleury: [%lu] NICK (%s)\n", (unsigned long)(pcl->tid), fleury_irc_param);
#endif
	    }
	  else
	    {
	      if (!strcmp(fleury_irc_cmd, "USER"))
		{
		  sscanf(fleury_irc_param, "%64s\n", pcl->user);
		  if (pcl->nick[0] && pcl->user[0])
		    {
		      fleury_irc_logged(pcl);
		    }
		  if (fleury_conf.pou)
		    {
		      fleury_irc_ping(pcl, fleury_conf.hostname);
		    } 
#ifdef FLEURY_DEBUG
		  fprintf(dbgout, "Fleury: [%lu] USER (%s)\n", (unsigned long)(pcl->tid), fleury_irc_param);
#endif
		}
	      else
		{
		  if (!strcmp(fleury_irc_cmd, "PING"))
		    {
		      if (*fleury_irc_param)
			{
			  if (*fleury_irc_param == ':')
			    {
			      fprintf(pcl->out, ":%s PONG %s %s\r\n", fleury_conf.host, fleury_conf.host, fleury_irc_param);
			    }
			  else
			    {
			      fprintf(pcl->out, ":%s PONG %s :%s\r\n", fleury_conf.host, fleury_conf.host, fleury_irc_param);
			    }
			}
		      else
			{
			  /* erreur pas de parametre */
			}
#ifdef FLEURY_DEBUG
		      fprintf(dbgout, "Fleury: [%lu] PING (%s)\n", (unsigned long)(pcl->tid), fleury_irc_param);
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
			  fprintf(dbgout, "Fleury: [%lu] PONG (%s)\n", (unsigned long)(pcl->tid), fleury_irc_param);
#endif
			}
		      else
			{
			  if (!strcmp(fleury_irc_cmd, "QUIT"))
			    {
			      fleury_socket_disconnect(pcl);
#ifdef FLEURY_DEBUG
			      fprintf(dbgout, "Fleury: [%lu] QUIT (%s)\n", (unsigned long)(pcl->tid), fleury_irc_param);
#endif
			    }
			  else
			    {
			      if (!strcmp(fleury_irc_cmd, "MODE"))
				{
#ifdef FLEURY_DEBUG
				  fprintf(dbgout, "Fleury: [%lu] MODE (%s)\n", (unsigned long)(pcl->tid), fleury_irc_param);
#endif
				}
			      else
				{
				  if (!strcmp(fleury_irc_cmd, "JOIN"))
				    {
#ifdef FLEURY_DEBUG
				      fprintf(dbgout, "Fleury: [%lu] JOIN (%s)\n", (unsigned long)(pcl->tid), fleury_irc_param);
#endif
				    }
				  else
				    {
				      if (!strcmp(fleury_irc_cmd, "PART"))
					{
#ifdef FLEURY_DEBUG
					  fprintf(dbgout, "Fleury: [%lu] PART (%s)\n", (unsigned long)(pcl->tid), fleury_irc_param);
#endif
					}
				      else
					{			      
#ifdef FLEURY_DEBUG
					  fprintf(dbgout, "Fleury: [%lu] Unmatched command %s (%s)\n", (unsigned long)(pcl->tid), fleury_irc_cmd, fleury_irc_param);
#endif
					}
				    }
				}
			    }
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
  fprintf(pcl->out, "PING :%s\r\n", s);
  strcpy(pcl->pingstr, s);
  pcl->pingtime = time(NULL);
}

void fleury_irc_logged(struct s_cl *pcl)
{
  pcl->logged = 1;
  fprintf(pcl->out, ":%s 001 %s :Welcome to SERVER_NAME_HERE\r\n", fleury_conf.host, pcl->nick);
  fprintf(pcl->out, ":%s 002 %s :Your host is %s\r\n", fleury_conf.host, pcl->nick, fleury_conf.host);
  fprintf(pcl->out, ":%s 003 %s :Bla bla bla\r\n", fleury_conf.host, pcl->nick);
  fprintf(pcl->out, ":%s 004 %s :Bla bla bla\r\n", fleury_conf.host, pcl->nick);
  fprintf(pcl->out, ":%s 005 %s :Bla bla bla\r\n", fleury_conf.host, pcl->nick);
  fprintf(pcl->out, ":%s 005 %s :Bla bla bla\r\n", fleury_conf.host, pcl->nick);
}
