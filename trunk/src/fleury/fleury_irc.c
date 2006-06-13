#include "fleury_irc.h"

void fleury_irc_process(struct s_cl *pcl)
{
  t_list ltemp;
  struct s_ch *pchan;
  struct s_cl *pcli;
  struct s_ch_user user;
  struct s_ch_user *pu;
  struct s_user_ch chch;
  struct s_user_ch *pch;
  struct s_ch chan;
  char minibuf[FLEURY_SZ_CHAN];
  char fleury_irc_cmd[64];
  char *fleury_irc_param;
  char *temp;
  char *temp2;
  int retval;
  unsigned long val;

  /* commandes a rajouter            : OPER                         */
  /* commandes a completer           : MODE PRIVMSG QUIT NICK       */
  /* commandes a completer plus tard : WHO JOIN LIST                */

  pcl->buffer[0] = 0;
  fleury_irc_cmd[0] = 0;

  if (pcl->pinged)
    {
      if (pcl->logged && pcl->pingtime && ((time(NULL) - pcl->pingtime) > fleury_conf.pto))
	{
	  /* ping timeout here */
	  fprintf(pcl->out, "Ping timeout\r\n");
	  fleury_socket_disconnect(pcl);
#ifdef FLEURY_DEBUG
	  fprintf(dbgout, "Fleury: [%lu] Ping timeout\n", (unsigned long)(pcl->tid));
#endif
	}
    }
  else
    {
      if (pcl->logged && pcl->pingtime && ((time(NULL) - pcl->pingtime) > fleury_conf.pto))
	{
	  fleury_irc_ping(pcl, fleury_conf.hostname);
	} 
    }

  retval = 0;
  ioctl(pcl->fd, FIONREAD, &val);
  if (val)
    {
      /* fgets(pcl->buffer, FLEURY_SZ_BUFFER, pcl->in); */

      while (val)
	{
	  recv(pcl->fd, pcl->buffer + pcl->ibuf, 1, 0);
	  if ((pcl->buffer[pcl->ibuf] == '\r') || (pcl->buffer[pcl->ibuf] == '\n'))
	    {
	      pcl->buffer[pcl->ibuf] = 0;
	      retval = 1;
	      val = 1;
	      pcl->ibuf = 0;
	    }
	  else
	    {
	      (pcl->ibuf)++;
	    }
	  val--;
	}

      fleury_irc_param = pcl->buffer;
      while(*fleury_irc_param && (*fleury_irc_param != '\r') && (*fleury_irc_param != '\n'))
	{
	  fleury_irc_param++;
	}
      *fleury_irc_param = 0;

      sscanf(pcl->buffer, "%64s\n", fleury_irc_cmd);

      if (retval && pcl->buffer[0] && fleury_irc_cmd[0])
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
		      sscanf(fleury_irc_param, "%64s", pcl->user);
		      temp = fleury_irc_next(fleury_irc_next(fleury_irc_next(fleury_irc_param)));
		      if (*temp == ':')
			{
			  temp++;
			}
		      strcpy(pcl->name, temp);

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
				  pcl->pingtime = time(NULL);
				  pcl->pinged = 0;
				}
#ifdef FLEURY_DEBUG
			      fprintf(dbgout, "Fleury: [%lu] PONG (%s)\n", (unsigned long)(pcl->tid), fleury_irc_param);
#endif
			    }
			  else
			    {
			      if (!strcmp(fleury_irc_cmd, "QUIT"))
				{
				  ltemp = pcl->list_chans;
				  while (ltemp)
				    {
				      pch = (struct s_user_ch *)&(ltemp->elt);
				      /* a completer, ne gerer chaque destinataire qu'une fois */
				      ltemp = ltemp->next;
				    }
				  fleury_socket_disconnect(pcl);
#ifdef FLEURY_DEBUG
				  fprintf(dbgout, "Fleury: [%lu] QUIT (%s)\n", (unsigned long)(pcl->tid), fleury_irc_param);
#endif
				}
			      else
				{
				  if (!strcmp(fleury_irc_cmd, "MODE"))
				    {
				      if (*fleury_irc_param)
					{
					  temp = fleury_irc_next(fleury_irc_param);
					  fleury_irc_param = fleury_irc_last(fleury_irc_param);

					  if (*temp)
					    {
					      /* parser les modes */
					    }
					  else
					    {
					      if (*fleury_irc_param == '#')
						{
						  if ((pchan = list_search_long(fleury_conf.list_ch, test_streq_ch, fleury_irc_param)))
						    {
						      fprintf(pcl->out, ":%s 324 %s %s +", fleury_conf.host, pcl->nick, fleury_irc_param);
						      if (pchan->mode.n)
							{
							  fprintf(pcl->out, "n");
							}
						      if (pchan->mode.t)
							{
							  fprintf(pcl->out, "t");
							}
						      if (pchan->mode.r)
							{
							  fprintf(pcl->out, "r");
							}

						      fprintf(pcl->out, "\r\n");
						      fprintf(pcl->out, ":%s 329 %s %s %lu\r\n", fleury_conf.host, pcl->nick, fleury_irc_param, (unsigned long)(pchan->date));
						    }
						  else
						    {
						      fprintf(pcl->out, ":%s 401 %s %s :No such nick/channel\r\n", fleury_conf.host, pcl->nick, fleury_irc_param);
						    }
						}
					      else
						{
						  if (!strcmp(fleury_irc_param, pcl->nick))
						    {
						      fprintf(pcl->out, ":%s 221 %s %s +", fleury_conf.host, pcl->nick, fleury_irc_param);
						      if (pcl->mode.i)
							{
							  fprintf(pcl->out, "i");
							}
						      if (pcl->mode.s)
							{
							  fprintf(pcl->out, "s");
							}
						      if (pcl->mode.w)
							{
							  fprintf(pcl->out, "w");
							}
						      if (pcl->mode.o)
							{
							  fprintf(pcl->out, "o");
							}
						      fprintf(pcl->out, "\r\n");
						    }
						  else
						    {
						      fprintf(pcl->out, ":%s 502 %s %s :Can't change mode for other users\r\n", fleury_conf.host, pcl->nick, fleury_irc_param);
						    }
						}
					    }
					}
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

					  if (*fleury_irc_param == '#')
					    {
					      pchan = list_search_long(fleury_conf.list_ch, test_streq_ch, fleury_irc_param);
					      
					      /* initialisation des champs du canal */
					      strcpy(chan.name, fleury_irc_param);
					      chan.topic[0] = 0;
					      chan.pass[0] = 0;
					      chan.topicauthor[0] = 0;
					      chan.list_users = NULL;
					      chan.list_ban = NULL;
					      chan.mode.n = 1;
					      chan.mode.t = 1;
					      chan.mode.r = 0;
					      chan.date = time(NULL);
					      chan.topicdate = 0;
					      pch = list_search_long(pcl->list_chans, test_streq_ch_chan, fleury_irc_param);
					      
					      chch.o = 0;
					      chch.v = 0;
					      user.o = 0;
					      user.v = 0;
					  
					      if (!pch)
						{
						  if (!pchan)
						    {
						      if (fleury_conf.list_ch)
							{
							  fleury_conf.list_ch = list_add_sorted(fleury_conf.list_ch, &chan, sizeof(chan), test_strcmp_ch);
							  pchan = list_search_long(fleury_conf.list_ch, test_streq_ch, fleury_irc_param);	      
							}
						      else
							{
							  fleury_conf.list_ch = list_new(&chan, sizeof(chan));
							  pchan = (struct s_ch *)&(fleury_conf.list_ch->elt);
							}
						      chch.o = 1;
						      user.o = 1;
						    }
						  
						  chch.pch = pchan;
						  user.pcl = pcl;
						  
						  pcl->list_chans = list_add_sorted(pcl->list_chans, &chch, sizeof(chch), test_strcmp_ch_chan);
						  pchan->list_users = list_add_sorted(pchan->list_users, &user, sizeof(user), test_strcmp_cl_user);
						  
						  
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
						      if (pu->o)
							{
							  fprintf(pcl->out, "@");
							}
						      else
							{
							  if (pu->v)
							    {
							      fprintf(pcl->out, "+");
							    }
							}
						      fprintf(pcl->out, "%s", pu->pcl->nick);
						      if (pu->pcl != pcl)
							{
							  fprintf(pu->pcl->out, ":%s!~%s@%s JOIN :%s\r\n", pcl->nick, pcl->user, pcl->host, pchan->name);
							  
#ifdef FLEURY_DEBUG
							  fprintf(dbgout, "Fleury: [%lu;%lu] JOIN Broadcast (%s)\n", (unsigned long)(pcl->tid), (unsigned long)(pu->pcl->tid), fleury_irc_param);
#endif
							}
						      
						      ltemp = ltemp->next;
						    }
						  fprintf(pcl->out, "\r\n");
						  fprintf(pcl->out, ":%s 366 %s %s :End of /NAMES list.\r\n", fleury_conf.host, pcl->nick, pchan->name);
						}
					    }
					  else
					    {
					      fprintf(pcl->out, ":%s 403 %s %s :No such channel\r\n", fleury_conf.host, pcl->nick, fleury_irc_param);
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
					      pch = list_search_long(pcl->list_chans, test_streq_ch_chan, fleury_irc_param);
					      if (pch)
						{
						  fprintf(pcl->out, ":%s!~%s@%s PART %s\r\n", pcl->nick, pcl->user, pcl->host, fleury_irc_param);

						  ltemp = pch->pch->list_users;
						  while (ltemp)
						    {
						      pu = (struct s_ch_user *)&(ltemp->elt);
						      if (pu->pcl != pcl)
							{
							  fprintf(pu->pcl->out, ":%s!~%s@%s PART %s\r\n", pcl->nick, pcl->user, pcl->host, fleury_irc_param);
							  
#ifdef FLEURY_DEBUG
							  fprintf(dbgout, "Fleury: [%lu;%lu] PART Broadcast (%s)\n", (unsigned long)(pcl->tid), (unsigned long)(pu->pcl->tid), fleury_irc_param);
#endif
							}
						      ltemp = ltemp->next;
						    }
						  pcl->list_chans = list_del_long(pcl->list_chans, test_streq_ch_chan, fleury_irc_param);
						  if (!(pch->pch->list_users = list_del_long(pch->pch->list_users, test_streq_cl_user, pcl->nick)))
						    {
						      fleury_conf.list_ch = list_del_long(fleury_conf.list_ch, test_streq_ch, pch->pch->name);
						    }
						}
					      else
						{
						  fprintf(pcl->out, ":%s 442 %s %s :You're not on that channel\r\n", fleury_conf.host, pcl->nick, fleury_irc_param);						  
						}
#ifdef FLEURY_DEBUG
					      fprintf(dbgout, "Fleury: [%lu] PART (%s)\n", (unsigned long)(pcl->tid), fleury_irc_param);
#endif
					      
					    }
					  else
					    {
					      if (!strcmp(fleury_irc_cmd, "PRIVMSG"))
						{
						  char dest[128];
						  char *msg;
						  char c;
						  int  i;
						  int  j;
						  int  l;
						  struct s_cl *tmp;
						  
						  l = strlen(fleury_irc_param);
						  
						  for (i = 0; (i < l) && (c != ' '); i++)
						    {
						      c = *(fleury_irc_param + i);
						      *(dest + i) = c; 
						    }
						  
						  *(dest + i - 1) = 0;
						  
						  msg = malloc((l - i + 1) * sizeof(char));
						  
						  
						  
						  
						  for (j = ++i, i = 0; j < l ; i++, j++)
						    {
						      c = *(fleury_irc_param + j);
						      *(msg + i) = c; 
						    }				
						  
						  *(msg + i) = 0;
						  
						  /* test nick/channel a optimiser */

						  tmp = list_search_long(fleury_conf.list_cl, test_streq_cl, dest);
						  
						  if (tmp)
						    {
						      fprintf(tmp->out, ":%s!~%s@%s PRIVMSG %s :%s\r\n", pcl->nick, pcl->user, pcl->host, pcl->nick, msg);					    
						    }
						  else
						    {
						      pchan = list_search_long(fleury_conf.list_ch, test_streq_ch, dest);
						      if (pchan && (!(pchan->mode.n) || list_search_long(pchan->list_users, test_streq_cl_user, pcl->nick)))
							{
							  ltemp = pchan->list_users;

							  while (ltemp)
							    {
							      pu = (struct s_ch_user *)&(ltemp->elt);
							      if (strcmp(pcl->nick, pu->pcl->nick))
								{
								  fprintf(pu->pcl->out, ":%s!~%s@%s PRIVMSG %s :%s\r\n", pcl->nick, pcl->user, pcl->host, pchan->name, msg); 
								}
							      ltemp = ltemp->next;
							    }						     
							}						  
						      else
							{
							  fprintf(pcl->out, ":%s 401 %s %s :No such nick/channel\r\n", fleury_conf.host, pcl->nick, fleury_irc_param);
							}
						    }
						  
						  free(msg);
#ifdef FLEURY_DEBUG
						  fprintf(dbgout, "Fleury: [%lu] PRIVMSG (%s)\n", (unsigned long)(pcl->tid), fleury_irc_param);
#endif
						}
					      else
						{
						  if (!strcmp(fleury_irc_cmd, "NAMES"))
						    {
						      if (fleury_irc_param)
							{
							  pchan = list_search_long(fleury_conf.list_ch, test_streq_ch, fleury_irc_param);
							  if (pchan)
							    {
							      ltemp = pchan->list_users;
							      fprintf(pcl->out, ":%s 353 %s = %s :", fleury_conf.host, pcl->nick, pchan->name);
							      while (ltemp)
								{
								  if (ltemp != pchan->list_users)
								    {
								      fprintf(pcl->out, " ");
								    }
								  pu = (struct s_ch_user *)&(ltemp->elt);
								  if (pu->o)
								    {
								      fprintf(pcl->out, "@");
								    }
								  else
								    {
								      if (pu->v)
									{
									  fprintf(pcl->out, "+");
									}
								    }
								  fprintf(pcl->out, "%s", pu->pcl->nick);
								  /* espace terminal, mode operateur */
								  ltemp = ltemp->next;
								}
							      fprintf(pcl->out, "\r\n");
							      fprintf(pcl->out, ":%s 366 %s %s :End of /NAMES list.\r\n", fleury_conf.host, pcl->nick, pchan->name);
							    }

							}
						      else
							{
							  /* erreur ? */
							}
#ifdef FLEURY_DEBUG
						  fprintf(dbgout, "Fleury: [%lu] NAMES (%s)\n", (unsigned long)(pcl->tid), fleury_irc_param);
#endif
						    }
						  else
						    {
						      if (!strcmp(fleury_irc_cmd, "WHO"))
							{
							  fleury_irc_param = fleury_irc_last(fleury_irc_param);
							  if (fleury_irc_param && *fleury_irc_param)
							    {
							      if (*fleury_irc_param == '#')
								{
								  if ((pch = list_search_long(pcl->list_chans, test_streq_ch_chan, fleury_irc_param)))
								    {
								      ltemp = pch->pch->list_users;
								      while (ltemp)
									{
									  pu = (struct s_ch_user *)&(ltemp->elt);
									  fprintf(pcl->out, ":%s 352 %s %s %s %s %s %s H :0 %s\r\n", fleury_conf.host, pcl->nick, pch->pch->name, pu->pcl->user, pu->pcl->host, fleury_conf.host, pu->pcl->nick, pu->pcl->name);
									  ltemp = ltemp->next;
									}
								    }
								}
							      else
								{
								  if((pcli = list_search_long(fleury_conf.list_cl, test_streq_cl, fleury_irc_param)))
								    {
								      if (pcli->list_chans)
									{
									  strcpy(minibuf, ((struct s_user_ch *)&(pcli->list_chans->elt))->pch->name);
									}
								      else
									{
									  sprintf(minibuf, "*");
									}
								      fprintf(pcl->out, ":%s 352 %s %s %s %s %s %s H :0 %s\r\n", fleury_conf.host, pcl->nick, minibuf, pcli->user, pcli->host, fleury_conf.host, pcli->nick, pcli->name);
								    }
								}
							    }
							  else
							    {
							      /* ignorage */
							    }
							  fprintf(pcl->out, ":%s 315 %s %s :End of /WHO list.\r\n", fleury_conf.host, pcl->nick, fleury_irc_param);

#ifdef FLEURY_DEBUG
							  fprintf(dbgout, "Fleury: [%lu] WHO (%s)\n", (unsigned long)(pcl->tid), fleury_irc_param);
#endif
							}
						      else
							{
							  if (!strcmp(fleury_irc_cmd, "TOPIC"))
							    {
							      temp = fleury_irc_next(fleury_irc_param);
							      fleury_irc_param = fleury_irc_last(fleury_irc_param);
							      if (*fleury_irc_param == '#')
								{
								  if ((pchan = list_search_long(fleury_conf.list_ch, test_streq_ch, fleury_irc_param)))
								    {
								      if (temp && (!(pchan->mode.t) || ((struct s_ch_user *)list_search_long(pchan->list_users, test_streq_cl_user, pcl->nick))->o))
									{
									  if (*temp == ':')
									    {
									      temp++;
									    }
									  strcpy(pchan->topic, temp);
									  pchan->topicdate = time(NULL);
									  ltemp = pchan->list_users;
									  fprintf(pcl->out, ":%s!~%s@%s TOPIC %s :%s\r\n", pcl->nick, pcl->user, pcl->host, pchan->name, pchan->topic);
									  while (ltemp)
									    {
									      pu = (struct s_ch_user *)&(ltemp->elt);
									      if (pu->pcl != pcl)
										{
										  fprintf(pu->pcl->out, ":%s!~%s@%s TOPIC %s :%s\r\n", pcl->nick, pcl->user, pcl->host, pchan->name, pchan->topic);
#ifdef FLEURY_DEBUG
										  fprintf(dbgout, "Fleury: [%lu;%lu] TOPIC Broadcast (%s)\n", (unsigned long)(pcl->tid), (unsigned long)(pu->pcl->tid), fleury_irc_param);
#endif

										}

									      ltemp = ltemp->next;
									    }
									}
								      else
									{
									  if (pchan->topicdate)
									    {
									      fprintf(pcl->out, ":%s 332 %s %s :%s\r\n", fleury_conf.host, pcl->nick, fleury_irc_param, pchan->topic);
									      fprintf(pcl->out, ":%s 333 %s %s %s %lu\r\n", fleury_conf.host, pcl->nick, fleury_irc_param, pchan->topicauthor, (unsigned long)(pchan->topicdate));
									    }
									  else
									    {
									      fprintf(pcl->out, ":%s 331 %s %s :No topic is set.\r\n", fleury_conf.host, pcl->nick, fleury_irc_param);
									    }
									}
								    }
								  else
								    {
								      fprintf(pcl->out, ":%s 403 %s %s :No such channel\r\n", fleury_conf.host, pcl->nick, fleury_irc_param);
								    }
								}
							      else
								{
								  fprintf(pcl->out, ":%s 403 %s %s :No such channel\r\n", fleury_conf.host, pcl->nick, fleury_irc_param);
								}
#ifdef FLEURY_DEBUG
							      fprintf(dbgout, "Fleury: [%lu] TOPIC (%s)\n", (unsigned long)(pcl->tid), fleury_irc_param);
#endif
							    }
							  else
							    {
							      if (!strcmp(fleury_irc_cmd, "LIST"))
								{
								  if (*fleury_irc_param)
								    {
								      /* a completer */
								    }
								  else
								    {
								      fprintf(pcl->out, ":%s 321 %s Channel :Users  Name\r\n", fleury_conf.host, pcl->nick);
								      ltemp = fleury_conf.list_ch;
								      while (ltemp)
									{
									  pchan = (struct s_ch *)&(ltemp->elt);
									  fprintf(pcl->out, ":%s 322 %s %s %d :[+", fleury_conf.host, pcl->nick, pchan->name, list_length(pchan->list_users));
									  if (pchan->mode.n)
									    {
									      fprintf(pcl->out, "n");
									    }
									  if (pchan->mode.t)
									    {
									      fprintf(pcl->out, "t");
									    }
									  if (pchan->mode.r)
									    {
									      fprintf(pcl->out, "r");
									    }
									  fprintf(pcl->out, "] %s\r\n", pchan->topic);
									  ltemp = ltemp->next;
									}
								      fprintf(pcl->out, ":%s 323 %s :End of /LIST\r\n", fleury_conf.host, pcl->nick);
								    }
#ifdef FLEURY_DEBUG
								  fprintf(dbgout, "Fleury: [%lu] LIST (%s)\n", (unsigned long)(pcl->tid), fleury_irc_param);
#endif
								}
							      else
								{
								  if (!strcmp(fleury_irc_cmd, "KICK"))
								    {
#ifdef FLEURY_DEBUG
								      fprintf(dbgout, "Fleury: [%lu] KICK (%s)\n", (unsigned long)(pcl->tid), fleury_irc_param);
#endif
								      temp = fleury_irc_next(fleury_irc_param);
								      temp2 = fleury_irc_next(temp);
								      if (*temp2 == ':')
									{
									  temp2++;
									}
								      else
									{
									  if (!(*temp2))
									    {
									      temp2 = pcl->nick;
									    }
									}
								      temp = fleury_irc_last(temp);
								      fleury_irc_param = fleury_irc_last(fleury_irc_param);
								      if (*fleury_irc_param == '#')
									{
									  if ((pch = list_search_long(pcl->list_chans, test_streq_ch_chan, fleury_irc_param)) && pch->o)
									    {
									      if ((pu = list_search_long(pch->pch->list_users, test_streq_cl_user, temp)))
										{
										  fprintf(pcl->out, ":%s!~%s@%s KICK %s %s :%s\r\n", pcl->nick, pcl->user, pcl->host, pch->pch->name, temp, temp2);		
										  if (pu->pcl != pcl)
										    {
										      fprintf(pu->pcl->out, ":%s!~%s@%s KICK %s %s :%s\r\n", pcl->nick, pcl->user, pcl->host, pch->pch->name, temp, temp2);
										    }
										  pch->pch->list_users = list_del_long(pch->pch->list_users, test_streq_cl_user, temp);
										  pu->pcl->list_chans = list_del_long(pu->pcl->list_chans, test_streq_ch_chan, fleury_irc_param);
										  
										  ltemp = pch->pch->list_users;
										  while (ltemp)
										    {
										      pu = (struct s_ch_user *)&(ltemp->elt);
										      if (pu->pcl != pcl)
											{
											  fprintf(pu->pcl->out, ":%s!~%s@%s KICK %s %s :%s\r\n", pcl->nick, pcl->user, pcl->host, pch->pch->name, temp, temp2);
#ifdef FLEURY_DEBUG
											  fprintf(dbgout, "Fleury: [%lu;%lu] KCK Broadcast (%s)\n", (unsigned long)(pcl->tid), (unsigned long)(pu->pcl->tid), fleury_irc_param);
#endif
											  
											}
										      
										      ltemp = ltemp->next;
										    }
										
										}
									      else
										{
										  fprintf(pcl->out, ":%s 401 %s %s :No such nick/channel\r\n", fleury_conf.host, pcl->nick, temp);
										}
									    }
									  else
									    {
									      fprintf(pcl->out, ":%s 482 %s %s :You're not channel operator\r\n", fleury_conf.host, pcl->nick, fleury_irc_param);
									    }
									}
								      else
									{
									  fprintf(pcl->out, ":%s 403 %s %s :No such channel\r\n", fleury_conf.host, pcl->nick, fleury_irc_param);
									}
								    }
								  else
								    {
								      fprintf(pcl->out, ":%s 421 %s %s :Unknown command\r\n", fleury_conf.host, pcl->nick, fleury_irc_cmd);
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
			    }
			}
		    }
		}
	    }
	}
    }
  else
    {
      sleep(0);
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
  pcl->pinged = 1;
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

char *fleury_irc_next(char *s)
{
  while (*s && (*s != ' '))
    {
      s++;
    }
  if (*s)
    {
      return (++s);
    }
  else
    {
      return (s);
    }
}

int test_streq_cl_user(void *p1, void *p2)
{
  return (!strcmp((char *)p1, ((struct s_ch_user *)p2)->pcl->nick));  
}

int test_streq_cl(void *p1, void *p2)
{
  return (!strcmp((char *)p1, ((struct s_cl *)p2)->nick));  
}

int test_streq_ch_chan(void *p1, void *p2)
{
  return (!strcmp((char *)p1, ((struct s_user_ch *)p2)->pch->name));  
}

int test_streq_ch(void *p1, void *p2)
{
  return (!strcmp((char *)p1, ((struct s_ch *)p2)->name));  
}

int test_strcmp_cl_user(void *p1, void *p2)
{
  return (strcmp(((struct s_ch_user *)p1)->pcl->nick, ((struct s_ch_user *)p2)->pcl->nick) < 0);
}

int test_strcmp_cl(void *p1, void *p2)
{
  return (strcmp(((struct s_cl *)p1)->nick, ((struct s_cl *)p2)->nick) < 0);
}

int test_strcmp_ch_chan(void *p1, void *p2)
{
  return (strcmp(((struct s_user_ch *)p1)->pch->name, ((struct s_user_ch *)p2)->pch->name) < 0);
}

int test_strcmp_ch(void *p1, void *p2)
{
  return (strcmp(((struct s_ch *)p1)->name, ((struct s_ch *)p2)->name) < 0);
}

int test_pthread_cl(void *p1, void *p2)
{
  return (((struct s_cl *)p1)->tid == ((struct s_cl *)p2)->tid);
}
