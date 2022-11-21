#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

/**
 * Executes the command "cat scores | grep Lakers".  In this quick-and-dirty
 * implementation the parent doesn't wait for the child to finish and
 * so the command prompt may reappear before the child terminates.
 *
 */

int main(int argc, char **argv)
{
  int pipefd[2];
  int pid_1;

  char *cat_args[] = {"cat", "scores", NULL};
  char *grep_args[] = {"grep", "Lakers", NULL};

  // make a pipe (fds go in pipefd[0] and pipefd[1])

  pipe(pipefd);

  pid_1 = fork(); // creates child

  if (pid_1 == 0)
    {
      // child gets here and handles "grep"

      // replace standard input with input part of pipe

      dup2(pipefd[0], 0);

      // close unused hald of pipe

      close(pipefd[1]);

      // execute grep

      execvp(grep_args[0], grep_args);
  }

  int pid_2 = fork(); // creates child's child

  // executes sort
  if (pid_2 == 0)
  {
    // child's child gets here and handles "sort"

    // replace input

    dup2(pipefd[0], 0);

    // close unused half of pipe

    close(pipefd[1]);

    // execute sort

    execvp(grep_args[0], grep_args);
  }

  else
    {
      // parent gets here and handles "cat"

      // replace standard output with output part of pipe

      dup2(pipefd[1], 1);

      // close unused unput half of pipe

      close(pipefd[0]);

      // execute cat

      execvp("cat", cat_args);
    }
}

