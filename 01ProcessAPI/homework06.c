/* modify wait program with waitpid.
When is waitpid useful?
*/

#include <assert.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
void test1(void){
  printf("\ntest 1\n");
  pid_t pid = fork();
  if (pid == 0){ // child process
    printf("This is child process.\n");
    int result = waitpid(-1,NULL,0);
    printf("wait returns in child : %d\n", result); 
    printf("Child process exits.\n");
    exit(0);
  }else{
    sleep(1);
    waitpid(-1,NULL,0);
    printf("Parent continues \n");    
  }
}


void test2(void){
  printf("\ntest 2\n");
  pid_t pid = fork(); // pid is the pid of child process.
  printf("fork returns  pid : %d\n", pid);  
  if (pid == 0){ // child process
    printf("This is child process %d.\n", getpid());
    exit(0);
  }else{
    int result = waitpid(-1,NULL,0);
    printf("parent pid : %d\n", getpid());
    printf("wait returns in parent : %d\n", result);
    assert(result == pid);
    printf("Parent exits \n");
    exit(0);
  }
}

int main(void){
  test1();
  test2();
}

/* Actual result: */
//replace wait(null) with waitpid(-1,null,0);
// By default, waitpid waits for child process to terminate.
