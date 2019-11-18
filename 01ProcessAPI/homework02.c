/* Write a program that opens a file (with the open() system call) */
/* and then calls fork() to create a new process. Can both the child */
/* and parent access the file descriptor returned by open()? What */
/* happens when they are writing to the file concurrently, i.e., at the */
/* same time?  */

/* Answer: yes they can. Concurrent access will corrupt the file. */



#include <fcntl.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>

int main(void){
  close(STDOUT_FILENO);  
  open("hw02.txt", O_CREAT|O_WRONLY|O_TRUNC, S_IRWXU);
  pid_t pid = fork();
  if (pid == 0){ // child process
    // now exec "wc"...
    char *myargs[3];
    myargs[0] = strdup("wc"); // program: wc (word count)
    myargs[1] = strdup("homework02.c"); // arg: file to count
    myargs[2] = NULL; // mark end of array
    execvp(myargs[0], myargs); // runs word count
  }else{
    //    write( fd, "world", 5);
  }

}
