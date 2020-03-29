#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/wait.h> 
#include <sys/reg.h>
#include <sys/user.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

struct user_regs_struct regs; 


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
        //orig_eax = ptrace(PTRACE_PEEKUSER,child,8*ORIG_RAX,NULL);
        //ptrace(PTRACE_CONT,child,NULL,NULL);


        ptrace(PTRACE_GETREGS,child,NULL,&regs);
        printf("the child made a system call %ld\n",regs.orig_rax);
    }

}