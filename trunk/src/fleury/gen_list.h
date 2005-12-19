#include <stdlib.h>
#include <string.h>

typedef struct s_list * t_list;

struct s_list
{
  t_list next;
  void *elt;
};

t_list list_new(void *, unsigned int);
