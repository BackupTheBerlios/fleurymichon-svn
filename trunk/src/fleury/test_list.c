#include <stdio.h>
#include <stdlib.h>
#include "gen_list.h"

int main()
{
  t_list list;
  int i;
  int *p;

  i = 42;
  list = list_new(&i, sizeof(int));
  p = (int *)&(list->elt);
  printf("%d\n", *p);

  return EXIT_SUCCESS;
}

