#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/wait.h> 
#include <sys/reg.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>


int main(){

    long orig_eax ; 
    pid_t child = fork() ; 

    // child process : tracee
    if(child==0){
        printf("CHILD process started..\n");
        ptrace(PTRACE_TRACEME,0,NULL,NULL);
        execl("/bin/uname", "uname", NULL);

    } // parent process : tracer 
    else{
        printf("PARENT processing waiting for child to start... \n");
        wait(NULL) ; 
        orig_eax = ptrace(PTRACE_PEEKUSER,child,8*ORIG_RAX,NULL);
        ptrace(PTRACE_CONT,child,NULL,NULL);
    }

}