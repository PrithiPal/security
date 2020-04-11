#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/wait.h> 
#include <sys/reg.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main()
{   pid_t child;
    long orig_eax;
    child = fork();
    if(child == 0) {
        ptrace(PTRACE_TRACEME, 0, NULL, NULL);
        execl("/bin/ls", "ls", NULL);
    }
    else {
        wait(NULL); // waits for the kernel signal to continue. kernel gives the authority to parent process.
        orig_eax = ptrace(PTRACE_PEEKUSER, // parent process can read the contents of the user segment(reg values.) now. 
                          child, 4 * ORIG_RAX,
                          NULL);
        printf("The child made a "
               "system call %ld\n", orig_eax);
        ptrace(PTRACE_CONT, child, NULL, NULL);
    }
    return 0;
}