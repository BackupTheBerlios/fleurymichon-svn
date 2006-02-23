#include "fleury_load.h"

void fleury_load_param(struct s_fleury_conf *pfc, FILE *pf)
{
  int	i;
  char	buffer[512];
  char	param[256];
  
  while ((!feof(pf)) && (fgets(buffer, 127, pf))) 
    {
      if ((*buffer != '[') || (!isspace(*buffer)))
	{
	  sscanf(buffer, "%s = ", param);
	  if (!strcmp(param, "size_pass"))
	    {
	      sscanf(buffer, "size_pass = %i", &i);
	      pfc->sfv.sz_pass = i;
	    }
	  else
	    {
	      if (!strcmp(param, "size_nick"))
		{
		  sscanf(buffer, "size_nick = %i", &i);
		  pfc->sfv.sz_nick = i;
		}
	      else
		{
		  if (!strcmp(param, "size_user"))
		    {
		      sscanf(buffer, "size_user = %i", &i);
		      pfc->sfv.sz_user = i;
		    }
		  else
		    {
		      if (!strcmp(param, "size_user_name"))
			{
			  sscanf(buffer, "size_user_name = %i", &i);
			  pfc->sfv.sz_name = i;
			}
		      else
			{
			  if (!strcmp(param, "size_chan_name"))
			    {
			      sscanf(buffer, "size_chan_name = %i", &i);
			      pfc->sfv.sz_chan = i;
			    }
			  else
			    {
			      if (!strcmp(param, "size_chan_topic"))
				{
				  sscanf(buffer, "size_chan_topic = %i", &i);
				  pfc->sfv.sz_topic = i;
				}
			      else
				{
				  if(!strcmp(param, "size_hostname"))
				    {
				      sscanf(buffer, "size_hostname = %i", &i);
				      pfc->sfv.sz_hostname = i;
				    }
				  else
				    {
				      if(!strcmp(param, "listen_on_port"))
					{
					  sscanf(buffer, "listen_on_port = %i", &i);
					  pfc->sfv.port = i;
					}
				      else
					{
					  if(!strcmp(param, "max_connections"))
					    {
					      sscanf(buffer, "max_connections = %i", &i);
					      pfc->sfv.max_con = i;
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
    }  
}
