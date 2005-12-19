#include "gen_list.h"

t_list list_new(void *elt, unsigned int size)
{
  t_list new;

  new = malloc(sizeof(struct s_list) + size - sizeof(void *));
  new->next = NULL;
  memcpy(&(new->elt), elt, size);

  return new;
}
