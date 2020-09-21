#include <stdio.h>
#include <signal.h>

#include "ipc/ipc.h"
#include "ipc/debug.h"


constexpr char * const children[] =
{
  "/usr/bin/cpp_ipc_demo_prod",
  "/usr/bin/cpp_ipc_demo_cons",
};

const int n_children = ARRAY_LENGTH(children);

pid_t child_pids[n_children];

int run = 1;

void sig_handler(int sig)
{
  debug_error("life signal\n");
  for (int i=0; i < n_children; ++i)
    kill(child_pids[i], sig);
  run = 0;
}

int main()
{
  pid_t pid;

  debug_error("starting life\n");



  for (int i=0; i < n_children; ++i)
  {

    pid = fork();

    debug_assert(pid != -1, return 1);

    if (pid == 0)
    {
      debug_error("child of %u with pid %u\n", getppid(), getpid());

      char * const child_argv[] = { children[i], NULL };
      execv(children[i], child_argv);
    }

    debug_error("parent of %u : %u\n", pid, getpid());
    child_pids[i] = pid;
  }

  #define MAX_PID_STRING_SIZE 20
  char pids_string[MAX_PID_STRING_SIZE * n_children] = {'\0'};
  for (int i=0; i < n_children; ++i)
    sprintf(pids_string, "%s %u", pids_string, child_pids[i]);
  debug_error("done spawning threads: %s\n", pids_string);

  signal(SIGINT, sig_handler);
  while (run)
  {
    usleep(200);
  }

  ipc_cleanup();
}