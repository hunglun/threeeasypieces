#define _GNU_SOURCE  
/***
In this homework, you’ll measure the costs of a system call and context
switch. Measuring the cost of a system call is relatively easy. For example,
you could repeatedly call a simple system call (e.g., performing a 0-byte
read), and time how long it takes; dividing the time by the number of
iterations gives you an estimate of the cost of a system call.
One thing you’ll have to take into account is the precision and accuracy of your timer. A typical timer that you can use is gettimeofday();
read the man page for details. What you’ll see there is that gettimeofday()
returns the time in microseconds since 1970; however, this does not mean
that the timer is precise to the microsecond. Measure back-to-back calls
to gettimeofday() to learn something about how precise the timer really is; this will tell you how many iterations of your null system-call
test you’ll have to run in order to get a good measurement result. If
gettimeofday() is not precise enough for you, you might look into
using the rdtsc instruction available on x86 machines.
Measuring the cost of a context switch is a little trickier. The lmbench
benchmark does so by running two processes on a single CPU, and
setting up two UNIX pipes between them; a pipe is just one of many
ways

processes in a UNIX system can communicate with one another. The first
process then issues a write to the first pipe, and waits for a read on the
second; upon seeing the first process waiting for something to read from
the second pipe, the OS puts the first process in the blocked state, and
switches to the other process, which reads from the first pipe and then
writes to the second. When the second process tries to read from the first
pipe again, it blocks, and thus the back-and-forth cycle of communication
continues. By measuring the cost of communicating like this repeatedly,
lmbench can make a good estimate of the cost of a context switch. You
can try to re-create something similar here, using pipes, or perhaps some
other communication mechanism such as UNIX sockets.
One difficulty in measuring context-switch cost arises in systems with
more than one CPU; what you need to do on such a system is ensure that
your context-switching processes are located on the same
processor. Fortunately, most operating systems have calls to bind a
process to a particular processor; on Linux, for example, the sched
setaffinity() call is what you’re looking for. By ensuring both processes are on the same
processor, you are making sure to measure the cost of the OS stopping
one process and restoring another on the same CPU. */
#include <assert.h>
#include <sched.h>
#include <stdio.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <unistd.h>
const unsigned int PRECISION_TEST_SIZE = 10;
int pipefd_alpha[2]; // 0 :  read, 1: write
int pipefd_beta[2]; // 0 :  read, 1: write
void test_timeofday_precision(){
  struct timeval tv[PRECISION_TEST_SIZE];
  assert(gettimeofday(&tv[0], NULL) == 0);
  assert(gettimeofday(&tv[1], NULL) == 0);  
  assert(gettimeofday(&tv[2], NULL) == 0);
  assert(gettimeofday(&tv[3], NULL) == 0);  
  assert(gettimeofday(&tv[4], NULL) == 0);
  assert(gettimeofday(&tv[5], NULL) == 0);  
  assert(gettimeofday(&tv[6], NULL) == 0);
  assert(gettimeofday(&tv[7], NULL) == 0);  
  assert(gettimeofday(&tv[8], NULL) == 0);
  assert(gettimeofday(&tv[9], NULL) == 0);  

  for (unsigned int i = 0; i < PRECISION_TEST_SIZE; i++){
    printf("iteration %d: %ld %ld\n", i, tv[i].tv_sec, tv[i].tv_usec);
  }

//  gettimeofday is Very precise in my machine!
/* iteration 0: 1574219904 689292 */
/* iteration 1: 1574219904 689292 */
/* iteration 2: 1574219904 689292 */
/* iteration 3: 1574219904 689292 */
/* iteration 4: 1574219904 689292 */
/* iteration 5: 1574219904 689292 */
/* iteration 6: 1574219904 689292 */
/* iteration 7: 1574219904 689292 */
/* iteration 8: 1574219904 689292 */
/* iteration 9: 1574219904 689292 */

  // it should never take more than 1 microsecond.
  long int timeTaken = (tv[9].tv_sec * 1000000 + tv[9].tv_usec) - (tv[0].tv_sec * 1000000 + tv[0].tv_usec);
  assert( timeTaken < 2);
  /* // it should extremely unlikely that it fails this test below. */
  /* // if it fails, it should pass the next run. */
  /* assert( timeTaken < 1);   */

}
// measure the costs of a system call 
void measure_system_call(){
  // measure how long it takes to execute printf("hello world!\n")
  printf("Measuring system call cost\n");
  long int timeTaken = 0; // in us
  struct timeval tv[2];
  assert(gettimeofday(&tv[0], NULL) == 0);
  printf("hello world!\n");
  assert(gettimeofday(&tv[1], NULL) == 0);  
  printf("Start time: %ld %ld\n", tv[0].tv_sec, tv[0].tv_usec);
  printf("End time  : %ld %ld\n", tv[1].tv_sec, tv[1].tv_usec);  
  timeTaken = (tv[1].tv_sec * 1000000 + tv[1].tv_usec) - (tv[0].tv_sec * 1000000 + tv[0].tv_usec);  
  printf("Time taken: %ld us\n", timeTaken);
}

// Create 2 processes on the same CPU using setaffinity()
void run_process_on_one_cpu(cpu_set_t * cpu_set){
    assert(0 ==  sched_getaffinity(getpid(), sizeof(cpu_set_t), cpu_set));
    for (int i=1; i<CPU_COUNT(cpu_set); i++)
      CPU_CLR(i,cpu_set);
    assert(0 ==  sched_setaffinity(getpid(), sizeof(cpu_set_t), cpu_set));
    assert(CPU_COUNT(cpu_set) == 1);
    assert(CPU_ISSET(0,cpu_set));
}

// measure the costs of context switch.
void measure_context_switch(){
  int pid;
  cpu_set_t cpu_set;
  char buf_alpha[6];
  char buf_beta[6];
  struct timeval tv[3];  
  run_process_on_one_cpu(&cpu_set);

  // Create 2 pipes
  pipe(pipefd_alpha);
  printf("Create pipes: %d , %d\n", pipefd_alpha[0], pipefd_alpha[1]);  
  pipe(pipefd_beta);
  printf("Create pipes: %d , %d\n", pipefd_beta[0], pipefd_beta[1]);    
  
  buf_alpha[5] = (char) 0 ; // null terminator for string
  buf_beta[5] = (char) 0 ; // null terminator for string
  
  pid = fork();
  if (pid == 0){ // child process
    // child writes to pipe_alpha while P2 read pipe_alpha
    // Event 2: 208
    assert(gettimeofday(&tv[0], NULL) == 0);
    write(pipefd_alpha[1],"hello",5);
    // Event 5: 251
    assert(gettimeofday(&tv[1], NULL) == 0);
    read(pipefd_beta[0],buf_beta,5);
    // Event 6: 253
    assert(gettimeofday(&tv[2], NULL) == 0);

    printf("Read from beta pipe: %s\n",buf_beta);
    for(int i=0; i<3; i++){
      printf("Child Timestamp %d   : %ld %ld\n", i+1, tv[i].tv_sec, tv[i].tv_usec);
    }
    write(pipefd_alpha[1],&tv[1].tv_usec,8);

     

    
  }else{
    // parent writes to pipe beta while child read pipe beta
    // Event 1 :  185
    assert(gettimeofday(&tv[0], NULL) == 0);    
    read(pipefd_alpha[0],buf_alpha,5);
    // Event 3 :  220
    assert(gettimeofday(&tv[1], NULL) == 0);          
    write(pipefd_beta[1],"world",5);
    // Event 4 :  221
    assert(gettimeofday(&tv[2], NULL) == 0);
    wait(NULL); // wait for child process
    
    printf("Read from alpha pipe: %s\n",buf_alpha);

    //Note:  the time difference between timestamp 3 and 4 is interesting:
    // let's use it to compute the context switch time  

    for(int i=0; i<3; i++){
      printf("Parent Timestamp %d  : %ld %ld\n", i+1, tv[i].tv_sec, tv[i].tv_usec);
    }

    long int ct_cost = 0;
    long int child_second_tv_usec = 0;
    read(pipefd_alpha[0],&child_second_tv_usec,8);
    ct_cost = child_second_tv_usec - tv[2].tv_usec;
    printf("\nTime taken for context switch: %ld us\n", ct_cost);
  }

}

int main(void){
  test_timeofday_precision();
  measure_system_call();
  measure_context_switch();  
}
