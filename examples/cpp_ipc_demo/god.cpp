#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

#include "ipc/ipc.h"
#include "ipc/debug.h"


const char * children[] =
{
  "/usr/bin/cpp_ipc_demo_prod",
  "/usr/bin/cpp_ipc_demo_cons",
};

const int n_children = ARRAY_LENGTH(children);

pid_t child_pids[n_children];

void sig_handler(int sig)
{
  debug_error("got signal %d\n", sig);
  for (int i=0; i < n_children; ++i)
    kill(child_pids[i], sig);

  ipc_cleanup();

  exit(sig);
}

int main()
{
  debug_error("starting god\n");


  int res = ipc_startup(child_pids, children, n_children);
  debug_assert(res == 0, return 1);

  char pids_string[20 * n_children] = {'\0'};
  for (int i=0; i < n_children; ++i)
    sprintf(pids_string, "%s %u", pids_string, child_pids[i]);
  debug_error("done spawning threads: %s\n", pids_string);

  signal(SIGINT, sig_handler);
  while (1)
  {
    usleep(200);
  }

  return 0;
}
