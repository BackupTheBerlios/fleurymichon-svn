#include <stdio.h>
#include <stdlib.h>
#include "gen_list.h"

int test(void *x)
{
  return (int)x == 43;
}

int main()
{
  t_list list;
  int i,j;
  int *p;
  int n;

  i = 42;
  list = list_new(&i, sizeof(int));
  p = (int *)&(list->elt);
  n = list_length(list);
  printf("%d l=%d\n", *p, n);  

  j = 45;
  list = list_add_head(list, &j, sizeof(int));
  
  int (*pf)(void *);

  pf = test;
  list = list_del(list, pf);
  printf("%i l=%i\n", *(int *)&(list->elt), list_length(list));
  return EXIT_SUCCESS;
}


