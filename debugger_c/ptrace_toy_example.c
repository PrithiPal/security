#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/wait.h> 
#include <sys/reg.h>
#include <sys/user.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

struct user_regs_struct regValues; 


// fetches values from regValues
void printRegisterValues(){
    printf("General purpose values are \n");
    printf("rax(SYS_CALL)\t%lld\n",regValues.orig_rax);
    printf("rbx\t%lld\n",regValues.rbx);
    printf("rcx\t%lld\n",regValues.rcx);
    printf("rdx\t%lld\n",regValues.rdx);
    printf("rsi\t%lld\n",regValues.rsi);
    printf("rdi\t%lld\n",regValues.rdi);
    printf("rsp\t%lld\n",regValues.rsp);
    printf("rbp\t%lld\n",regValues.rbp);
    printf("rip\t%lld\n",regValues.rip);
}

int main(){

    long orig_rax ; 
    pid_t child = fork() ; 
    int child_status ;
    long rax_prev_value ; 



    // child process : tracee
    if(child==0){
        printf("CHILD process started..\n");
        ptrace(PTRACE_TRACEME,0,NULL,NULL);
        execl("/bin/ls", "ls", NULL);

    } // parent process : tracer 
    else{
        printf("PARENT processing waiting for child to start... \n");

        // run indefinitely 
        while(1){
            // check status of child 
            wait(&child_status);
            if(WIFEXITED(child_status)){
                printf("Child has exited or terminated\n");
                break;
            }

            ptrace(PTRACE_GETREGS,child,NULL,&regValues); // update register values from child process
            printRegisterValues();

            rax_prev_value = regValues.orig_rax ; 
            // ptrace(PTRACE_CONT, child, NULL, NULL);
            ptrace(PTRACE_SYSCALL,child, NULL, NULL);
        }
    }

}