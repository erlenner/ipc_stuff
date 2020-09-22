#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>

#include "ipc/ipc.h"
#include "ipc/debug.h"



typedef struct
{
  const char *name;
  int restart;  // -1 = infinite , 0 = never, 1 = once, 2 = twice, etc.

// internal
  pid_t pid;
  int alive;

} child;

child children[] =
{
//==================== FILL IN PROCESSES ====================
  {
    .name = "/usr/bin/cpp_ipc_demo_prod",
  },
  {
    .name = "/usr/bin/cpp_ipc_demo_cons",
    .restart = -1,
  },
//===========================================================
};

#define n_children ARRAY_LENGTH(children)


int fork_child(child *c)
{
  pid_t pid = fork();

  debug_assert(pid != -1, return 1);

  if (pid == 0)
  {
    debug_error("child %u (%s) was spawned by parent %u\n", getpid(), c->name, getppid());

    setpgid(0, 0); // switch process group so ctrl-c only interrupts god

    char * const child_argv[] = { (char*)(c->name), NULL };
    execv(c->name, child_argv);
  }

  c->pid = pid;
  c->alive = 1;
  debug_error("parent %u spawned child %u (%s)\n", getpid(), c->pid, c->name);

  return 0;
}

void exit_handler(int sig)
{
  debug_error("got signal %d\n", sig);


  // kill the children manually, since they're in a different group
  signal(SIGCHLD, SIG_IGN);
  for (int i=0; i < n_children; ++i)
  {
    if (children[i].alive)
    {
      debug_error("killing %u (%s)\n", children[i].pid, children[i].name);
      debug_assert(kill(children[i].pid, sig) == 0);
    }
  }

  ipc_cleanup();

  debug_error("exiting %u (god process) with status %d\n", getpid(), sig);

  exit(sig);
}

void child_handler(int sig)
{
  pid_t pid;
  int status;

  for (pid_t tmp_pid; (tmp_pid = waitpid(-1, &status, WNOHANG)) > 0;)
    pid = tmp_pid;

  int child_index = -1;
  for (int i = 0; i < n_children; ++i)
    if (children[i].pid == pid)
      child_index = i;

  debug_assert(child_index != -1, return);
  child *c = children + child_index;

  c->alive = 0;
  debug_error("child %u (%s) exited with status %d\n", c->pid, c->name, status);

  // TODO: move restart logic to main thread
  if (c->restart != 0)
  {
    debug_error("restarting child\n");
    fork_child(c);

    if (c->restart > 0)
      --c->restart;
  }
}

int main()
{
  debug_error("starting %u (god process)\n", getpid());

  for (int i=0; i < n_children; ++i)
    fork_child(children + i);

  signal(SIGINT, exit_handler);
  signal(SIGCHLD, child_handler);

  while(1)
    pause();

  return 0;
}
