#include "fleury_types.h"
#include "fleury_daemon.h"
#include "fleury_socket.h"

int main()
{
  let_fleury_be_daemon(fopen("fleury.cfg","r"));
  /* printf("%i\n", fleury_conf.sfv.port); */

  fleury_server_start(fleury_conf.sfv.port);

#ifdef FLEURY_DEBUG
  fprintf(dbgout, "Fleury: Logging off\n");
  fflush(dbgout);
  fclose(dbgout);
#endif

  return EXIT_SUCCESS;
}
