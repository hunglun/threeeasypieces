/* create a child process that close standard output. What happens when it writes to standard output? */
#include <stdio.h>
#include <unistd.h>
void test1(void){
  int pid = fork();
  if (pid == 0) {
    close(STDOUT_FILENO);
    printf("hello\n");
  }
}

int main(void){
  test1();
}

//Answer:
//Nothing get printed.
