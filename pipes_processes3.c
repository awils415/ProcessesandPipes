#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

/**
 * Executes the command "cat scores | grep Lakers | sort".  In this quick-and-dirty
 * implementation the parent doesn't wait for the child to finish and
 * so the command prompt may reappear before the child terminates.
 *
 */

int main(int argc, char **argv) {
  int pipefd_1[2];
  int pipefd_2[2];
  int pid_1, pid_2;

  char *cat_args[] = {"cat", "scores", NULL};
  char *grep_args[] = {"grep", argv[1], NULL};
  char *sort_args[] = {"sort", NULL};

  // make pipes
  if (pipe(pipefd_1) == -1) {
    fprintf(stderr, "Pipe failed.");
    return 1;
  }
  if (pipe(pipefd_2) == -1) {
    fprintf(stderr, "Pipe failed.");
    return 1;
  }
  
  pid_1 = fork();

  if (pid_1 == 0) {
    pid_2 = fork();
    if (pid_2 == 0) {
      // child 2 gets here and sorts
      // input to pipefd_2 input
      dup2(pipefd_2[0], 0);

      // close fds
      close(pipefd_1[0]);
      close(pipefd_1[1]);
      close(pipefd_2[0]);
      close(pipefd_2[1]);

      // execute sort
      execvp(sort_args[0], sort_args);
    }
    else {
      // child gets here and handles grep
      // input to pipefd_2 input
      dup2(pipefd_1[0], 0);
      // output to pipefd_2 output
      dup2(pipefd_2[1], 1);

      // close fds
      close(pipefd_1[0]);
      close(pipefd_1[1]);
      close(pipefd_2[0]);
      close(pipefd_2[1]);

      // execute grep
      execvp(grep_args[0], grep_args);
    }
  }
  else {
    // parent gets here and handles "cat scores"

    // output to pipefd_1
    dup2(pipefd_1[1], 1);

    // close fds
    close(pipefd_1[0]);
    close(pipefd_1[1]);
    close(pipefd_2[0]);
    close(pipefd_2[1]);

    // execute cat
    execvp("cat", cat_args);
  }
}