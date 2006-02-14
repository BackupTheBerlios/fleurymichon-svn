#include "fleury_irc.h"

void fleury_irc_process(struct s_cl *pcl)
{
  t_list ltemp;
  struct s_ch *pchan;
  struct s_ch_user user;
  struct s_ch_user *pu;
  struct s_user_ch chch;
  struct s_user_ch *pch;
  struct s_ch chan;
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
				      /* TODO: tester les differents parametres */
				      fleury_irc_param = fleury_irc_last(fleury_irc_param);
				      int test(void *p)
					{
					  struct s_ch *ch;
					  
					  ch = (struct s_ch *)p;
					  return (!strcmp(fleury_irc_param, ch->name));
					}
				      int test2(void *p1, void *p2)
					{
					  return (!strcmp(((struct s_ch *)p1)->name, ((struct s_ch *)p2)->name) < 0);
					}
				      int test3(void *p)
					{
					  return (!strcmp(fleury_irc_param, ((struct s_user_ch *)p)->pch->name));
					}
				      int test4(void *p1, void *p2)
					{
					  return (!strcmp(((struct s_ch_user *)p1)->pcl->nick, ((struct s_ch_user *)p2)->pcl->nick) < 0);
					}
				      int test5(void *p1, void *p2)
					{
					  return (!strcmp(((struct s_user_ch *)p1)->pch->name, ((struct s_user_ch *)p2)->pch->name) < 0);
					}

				      pchan = list_search(fleury_conf.list_ch, test);

				      /* initialisation des champs du canal */
				      strcpy(chan.name, fleury_irc_param);
				      chan.topic[0] = 0;
				      chan.pass[0] = 0;
				      chan.list_users = NULL;
				      chan.list_ban = NULL;
				      chan.mode.r = 0;
				      
				      pch = list_search(pcl->list_chans, test3);

				      chch.status = 0;
				      user.status = 0;

				      if (!pch)
					{
					  if (!pchan)
					    {
					      if (fleury_conf.list_ch)
						{
						  fleury_conf.list_ch = list_add_sorted(fleury_conf.list_ch, &chan, sizeof(chan), test2);
						  pchan = list_search(fleury_conf.list_ch, test);	      
						}
					      else
						{
						  fleury_conf.list_ch = list_new(&chan, sizeof(chan));
						  pchan = (struct s_ch *)&(fleury_conf.list_ch->elt);
						}
					      chch.status = 'o';
					      user.status = 'o';
					    }
					  
					  chch.pch = pchan;
					  user.pcl = pcl;

					  pcl->list_chans = list_add_sorted(pcl->list_chans, &chch, sizeof(void *), test5);
					  pchan->list_users = list_add_sorted(pchan->list_users, &user, sizeof(void *), test4);
					

					  fprintf(pcl->out, ":%s!~%s@%s JOIN :%s\r\n", pcl->nick, pcl->user, pcl->host, pchan->name);
					  if (*(pchan->topic))
					    {
					      fprintf(pcl->out, ":%s 332 %s %s :%s\r\n", fleury_conf.host, pcl->nick, pchan->name, pchan->topic);
					    }

					  fprintf(pcl->out, ":%s 353 %s = %s :", fleury_conf.host, pcl->nick, pchan->name);
					  pu = NULL;
					  ltemp = pchan->list_users;
					  while (ltemp)
					    {
					      if (pu)
						{
						  fprintf(pcl->out, " ");
						}
					      pu = (struct s_ch_user *)&(ltemp->elt);
					      if (pu->status == 'o')
						{
						  fprintf(pcl->out, "@");
						}
					      fprintf(pcl->out, "%s", pu->pcl->nick);
					      if (pu->pcl != pcl)
						{
						  fprintf(pu->pcl->out, ":%s!~%s@%s JOIN :%s\r\n", pcl->nick, pcl->user, pcl->host, pchan->name);

#ifdef FLEURY_DEBUG
						  fprintf(dbgout, "Fleury: [%lu;%lu] JOIN (%s)\n", (unsigned long)(pcl->tid), (unsigned long)(pu->pcl->tid), fleury_irc_param);
#endif
						}
					      
					      ltemp = ltemp->next;
					    }
					  fprintf(pcl->out, "\r\n");
					  fprintf(pcl->out, ":%s 366 %s %s :End of /NAMES list.\r\n", fleury_conf.host, pcl->nick, pchan->name);
					}				      

#ifdef FLEURY_DEBUG
				      fprintf(dbgout, "Fleury: [%lu] JOIN (%s)\n", (unsigned long)(pcl->tid), fleury_irc_param);
#endif
				    }
				  else
				    {
				      if (!strcmp(fleury_irc_cmd, "PART"))
					{
					  fleury_irc_param = fleury_irc_last(fleury_irc_param);
					  int part_test1(void *p)
					    {
					      return(!strcmp(((struct s_user_ch *)p)->pch->name, fleury_irc_param));
					    }
					  pch = list_search(pcl->list_chans, part_test1);
					  if (pch)
					    {
					      int part_test2(void *p)
						{
						  return(!strcmp(((struct s_user_ch *)p)->pch->name, fleury_irc_param));
						}
					      pcl->list_chans = list_del(pcl->list_chans, part_test2);
					      
					      ltemp = pch->pch->list_users;
					      while (ltemp)
						{
						  pu = (struct s_ch_user *)&(ltemp->elt);
						  fprintf(pu->pcl->out, ":%s!~%s@%s PART %s\r\n", pcl->nick, pcl->user, pcl->host, fleury_irc_param);

#ifdef FLEURY_DEBUG
						  fprintf(dbgout, "Fleury: [%lu;%lu] PART (%s)\n", (unsigned long)(pcl->tid), (unsigned long)(pu->pcl->tid), fleury_irc_param);
#endif

						  ltemp = ltemp->next;
						}

					      int part_test3(void *p)
						{
						  return(!strcmp(((struct s_ch_user *)p)->pcl->nick, pcl->nick));
						}
					      pch->pch->list_users = list_del(pch->pch->list_users, part_test3);
					    }
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

char *fleury_irc_last(char *s)
{
  char *ss;

  ss = s;
  while(*s && (*s != ' '))
    {
      s++;
    }
  *s = 0;

  return ss;
}
