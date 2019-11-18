/* Create 2 child processes. Connect the output of one child process to the input of another child process. */

#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <stdlib.h>
int pipefd[2]; // 0 :  read, 1: write
void child_process_a(){
  char buf[10];
  read(pipefd[0],buf,5);
  printf("%c", buf[0]);
  printf("%c", buf[1]);
  printf("%c", buf[2]);
  printf("%c", buf[3]);
  printf("%c", buf[4]);
  printf("\n");
}

void child_process_b(){
  write(pipefd[1],"hello",5);
}
int main(void){
  pipe(pipefd);
  printf("pipes: %d , %d\n", pipefd[0], pipefd[1]);
  int pid = fork();

  if (pid == 0){ // child process A
    child_process_a();
    exit(0);
  }else{
    int pid2 = fork();
    if (pid2 == 0){ // child process B
      child_process_b();
      exit(0);
    }
  }
  wait(NULL); // wait for all child processes to terminate.
  printf("All child processes terminate.\n");
}
