#include "gen_list.h"

t_list list_new(void *elt, unsigned int size)
{
  t_list new;

  new = malloc(sizeof(struct s_list) + size - sizeof(void *));
  new->next = NULL;
  memcpy(&(new->elt), elt, size);

  return new;
}

t_list list_add_head(t_list l, void *elt, unsigned int size)
{
  t_list new;

  new = list_new(elt, size);
  new->next = l;

  return new;
}

t_list list_add_tail(t_list l, void *elt, unsigned int size)
{
  t_list p;

  if (l)
    {
      p = l;
      while (p->next)
	{
	  p = p->next;
	}
      p->next = list_new(elt, size);
    }
  else
    {
      l = list_new(elt, size);
    }

  return l;
}

void *list_last(t_list l)
{
  if (l)
    {
      while (l->next)
	{
	  l = l->next;
	}
      return &(l->elt);
    }
  else
    {
      return NULL;
    }
}

int list_length(t_list l)
{
  int n;

  n = 0;

  while (l)
    {
      n++;
      l = l->next;
    }

  return n;
}


t_list list_del_default(t_list l, int (*pf) (void *))
{
  t_list p;
  t_list q;
  
  p = l;
  q = l;
  while (q && !(pf(&q->elt)))
    {
      p = q;
      q = q->next;
    }
  if (q)
    {
      p->next = q->next;
      free(q);
    }
  return l;
}

t_list list_del(t_list l, int (*pf) (void *))
{
  t_list p;
  
  if (!l)
    {
      return NULL;
    }
  else
    {
      if (pf(&p->elt))
	{
	  p = l->next;
	  free(l);
	  return p;
	}
      else
	{
	  return list_del_default(l, pf);
	}
    }
}
      
