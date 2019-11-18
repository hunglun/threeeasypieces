/* Write a wait program, and find out what does wait return in parent. What happen when wait is used in child process? 
Expect:
Wait will return 0 in parent when child exits.
Wait in child process causes child process to live forever.
*/

#include <assert.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main(void){
  pid_t pid = fork();
  if (pid == 0){ // child process
    printf("This is child process.\n");
    int result = wait(NULL);
    printf("wait returns in child : %d\n", result); 
    printf("Child process exits.\n");
  }else{
    /* int result = wait(NULL); */
    /* printf("parent pid : %d\n", pid); */
    /* printf("wait returns in parent : %d\n", result); */
    /* assert(result == pid); */
    printf("Parent exits \n");    
  }
}

/* Actual result: */
/* wait in parent process  returns the pid of parent. */
/* wait in child process would wait for parent process before proceeeding. */
