#include <stdio.h>
#include <stdlib.h>
#include "gen_list.h"

int main()
{
  t_list list;
  int i;
  int *p;
  int n;

  i = 42;
  list = list_new(&i, sizeof(int));
  p = (int *)&(list->elt);
  n = list_length(list);
  printf("%d l=%d\n", *p, n);

  return EXIT_SUCCESS;
}

