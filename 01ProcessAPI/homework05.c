/* Write a wait program, and find out what does wait return in parent. What happen when wait is used in child process? 
Expect:
Wait will return 0 in parent when child exits.
Wait in child process causes child process to live forever.
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
    int result = wait(NULL);
    printf("wait returns in child : %d\n", result); 
    printf("Child process exits.\n");
    exit(0);
  }else{
    sleep(1);
    wait(NULL);
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
    int result = wait(NULL);
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
/* wait in parent process  returns the pid of child. */
/* wait in child process returns error -1 */
