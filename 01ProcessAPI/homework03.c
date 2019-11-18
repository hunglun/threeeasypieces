/* Write a fork program in which the parent writes "hello", the child "world". Make the program print hello world without using wait. */

// use sleep
#include <fcntl.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>

int main(void){
  pid_t pid = fork();
  if (pid == 0){ // child process
    printf("world \n");
  }else{
    sleep(1);
    printf("hello \n");
  }
}

