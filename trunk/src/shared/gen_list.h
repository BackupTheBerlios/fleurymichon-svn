#include <stdlib.h>
#include <string.h>

typedef struct s_list * t_list;

struct s_list
{
  t_list next;
  void *elt;
};

t_list list_new(void *, unsigned int);
t_list list_add_head(t_list, void *, unsigned int);
t_list list_add_tail(t_list, void *, unsigned int);
t_list list_add_sorted(t_list, void *, unsigned int, int (*) (void *, void *));
void *list_last(t_list);
int list_length(t_list);
t_list list_del_default(t_list, int (*) (void *));
t_list list_del(t_list, int (*) (void *));
