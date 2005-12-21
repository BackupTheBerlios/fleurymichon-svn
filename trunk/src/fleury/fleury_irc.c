#include "fleury_irc.h"

void fleury_irc_process(struct s_cl *pcl)
{
  char fleury_irc_cmd[64];
  char *fleury_irc_param;

  pcl->buffer[0] = 0;
  fleury_irc_cmd[0] = 0;

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
#ifdef FLEURY_DEBUG
	      fprintf(dbgout, "Fleury: NICK (%s)\n", fleury_irc_param);
#endif
	    }
	  else
	    {
	      if (!strcmp(fleury_irc_cmd, "USER"))
		{
		  sscanf(fleury_irc_param, "%64s\n", pcl->user);
#ifdef FLEURY_DEBUG
		  fprintf(dbgout, "Fleury: USER (%s)\n", fleury_irc_param);
#endif
		}
	      else
		{
		  if (!strcmp(fleury_irc_cmd, "PONG"))
		    {
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
