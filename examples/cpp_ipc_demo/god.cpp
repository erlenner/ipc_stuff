#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>

#include "ipc/ipc.h"
#include "ipc/debug.h"


const char * child_names[] =
{
  "/usr/bin/cpp_ipc_demo_prod",
  "/usr/bin/cpp_ipc_demo_cons",
};

const int n_children = ARRAY_LENGTH(child_names);

pid_t children[n_children];

void exit_handler(int sig)
{
  debug_error("got signal %d\n", sig);


  // kill the children manually, since they're in a different group
  signal(SIGCHLD, SIG_IGN);
  for (int i=0; i < n_children; ++i)
    kill(children[i], sig);

  ipc_cleanup();

  exit(sig);
}

void child_handler(int sig)
{
  debug_assert(sig == SIGCHLD, return);
  debug_error("Got SIGCHLD\n");

  pid_t pid;
  int status;

  while (1)
  {
    pid_t tmp_pid = waitpid(-1, &status, WNOHANG);

    debug_assert(tmp_pid != -1, return);

    if (tmp_pid == 0)
      break;
    else
      pid = tmp_pid;
  }

  debug_error("child %u got killed with status %d\n", pid, status);

  
}

int main()
{
  debug_error("starting god\n");


  //int res = ipc_startup(children, child_names, n_children);
  //debug_assert(res == 0, return 1);
  for (int i=0; i < n_children; ++i)
  {
    pid_t pid = fork();

    debug_assert(pid != -1, return 1);

    if (pid == 0)
    {
      debug_error("child of %u with pid %u\n", getppid(), getpid());

      setpgid(0, 0); // switch process group so ctrl-c only interrupts god

      char * const child_argv[] = { (char*)child_names[i], NULL };
      execv(child_names[i], child_argv);
    }

    debug_error("parent of %u : %u\n", pid, getpid());
    children[i] = pid;
  }

  char pids_string[20 * n_children] = {'\0'};
  for (int i=0; i < n_children; ++i)
    sprintf(pids_string, "%s %u", pids_string, children[i]);
  debug_error("done spawning threads: %s\n", pids_string);

  signal(SIGINT, exit_handler);
  signal(SIGCHLD, child_handler);

  while(1)
    pause();

  return 0;
}
