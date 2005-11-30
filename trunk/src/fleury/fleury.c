#include "fleury_types.h"
#include "fleury_daemon.h"
#include "fleury_socket.h"

int main()
{

#ifdef FLEURY_DEBUG
  dbgout = fopen("fleury_debug.log", "a");
#endif

  let_fleury_be_daemon();

  fleury_server_start(6667);

#ifdef FLEURY_DEBUG
  fclose(dbgout);
#endif

  return EXIT_SUCCESS;
}
