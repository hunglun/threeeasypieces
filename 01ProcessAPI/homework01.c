#include  <stdio.h>
#include  <sys/types.h>
#include <unistd.h>
int g = 1000;
int main(void){
  int c = 100;
  printf("Hello\n");  
  pid_t pid=fork();
  if (pid == 0){
    // child process
    printf("   This line is from child, value = %d,%d\n", c, g);
    c = 99;
    g = 999;
    printf("   This second line is from child, value = %d,%d\n", c, g);    
  }else{
    // parent process
    printf("   This line is from parent, value = %d,%d\n", c, g);
    c = 98;
    g = 998;
    printf("   This second line is from parent, value = %d,%d\n", c, g);    
    
  }
}

   /* This line is from parent, value = 100 */
   /* This second line is from parent, value = 98 */
   /* This line is from child, value = 100 */
   /* This second line is from child, value = 99 */

/* Child does copy the stack from parents, hence the local variables have the same value. */
/* What if it is a global variable?   */

/* $ ./homework01.exe  */
/*    This line is from parent, value = 100,1000 */
/*    This second line is from parent, value = 98,998 */
/*    This line is from child, value = 100,1000 */
/*    This second line is from child, value = 99,999 */

// global variables are copied too.

// TODO on window system, the fork does not work as it would on linux system.
// Why?
