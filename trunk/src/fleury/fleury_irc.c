#include "fleury_irc.h"

void fleury_irc_process(struct s_cl *pcl)
{
  char fleury_irc_cmd[64];
  /* char fleury_irc_param[256]; */

  /* fscanf(in, "%64s %256s\n", fleury_irc_cmd, fleury_irc_param); */

  pcl->buffer[0] = 0;
  fleury_irc_cmd[0] = 0;

  fgets(pcl->buffer, FLEURY_SZ_BUFFER, pcl->in);

  sscanf(pcl->buffer, "%64s\n", fleury_irc_cmd);

  if (pcl->buffer[0] && fleury_irc_cmd[0])
    {
      fprintf(pcl->out, "%s\r\n", fleury_irc_cmd);
      if (!strcmp(fleury_irc_cmd, "PASS"))
	{
	  /* sscanf(fleury_irc_param, ":%128s", pcl->pass); */
#ifdef FLEURY_DEBUG
	  fprintf(dbgout, "Fleury: PASS\n");	  
	  sscanf(pcl->buffer, "%64s\n", pcl->pass);
#endif
	}
      else
	{
	  if (!strcmp(fleury_irc_cmd, "NICK"))
	    {
#ifdef FLEURY_DEBUG
	      fprintf(dbgout, "Fleury: NICK\n");
	      sscanf(pcl->buffer, "%64s\n", pcl->nick);
#endif
	    }
	  else
	    {
	      if (!strcmp(fleury_irc_cmd, "USER"))
		{
#ifdef FLEURY_DEBUG
		  fprintf(dbgout, "Fleury: USER\n");
		  sscanf(pcl->buffer, "%64s\n", pcl->user);
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
			  fprintf(dbgout, "Fleury: Unmatched command\n");
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
