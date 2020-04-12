#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/reg.h>
#include <sys/user.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <syscall.h>
#include <sys/syscall.h>   /* For SYS_write etc */
                    

int main(int argc, char *argv[])
{   
    pid_t traced_process;
    struct user_regs_struct regs;
    
    int status; 
    
    if(argc != 2) {
        printf("Usage: %s <pid to be tracefd>\n",argv[0]);
        exit(1);
    }

    traced_process = atoi(argv[1]);
    printf("traced process = %d\n",traced_process);

    ptrace(PTRACE_ATTACH, traced_process, NULL, NULL);
   
    char c ;
    while(1){
        
        wait(NULL); 

        ptrace(PTRACE_GETREGS, traced_process, NULL, &regs);
        printf("EIP: %llx\n",regs.rip);
        printf("EAX: %llx\n",regs.orig_rax);
        printf("EBX: %llx\n",regs.rbx);
        printf("ECX: %llx\n",regs.rcx);
        printf("EDX: %llx\n",regs.rdx);

        printf("(r) >> ");
        c = getchar() ; 
        if (c=='b') break ; 

        ptrace(PTRACE_SINGLESTEP,traced_process,NULL,NULL);
    }

    ptrace(PTRACE_DETACH, traced_process, NULL, NULL);
    wait(NULL); 

    getchar();
    return 0;

}