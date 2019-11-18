/* Write a fork program in which all variants of execv are used to execute ls. Explain why there are so many variants */

#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

int main(void){
  pid_t pid = fork();
  if (pid == 0){ // child process
    // now exec "ls"...
    char *myargs[2];
    myargs[0] = strdup("ls"); // program: ls
    myargs[1] = NULL; // mark end of array
    execvp(myargs[0], myargs); // runs word count
  }else{
    printf("Exit \n");
  }
}

/* p stands for path. PATH environment is used to search for the file. */
/* v stands for vector. */
/* E stands for environment. Custom environment variable can be passed to exec. */
