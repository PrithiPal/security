#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/reg.h>
#include <sys/user.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/syscall.h>   /* For SYS_write etc */

int main()
{   pid_t child;
    const int long_size = sizeof(long);
    child = fork();
    
    if(child == 0) {
        printf("child process is going to start\n");
        ptrace(PTRACE_TRACEME, 0, NULL, NULL);
        execl("./dummy", "dummy", NULL);
    }
    else 
    {   
        printf("Parent process\n");
        
        int status;
        union u {
            long val;
            char chars[long_size];
        }data;


        struct user_regs_struct regs;
        int start = 0;
        long ins;
        while(1) {
            
            wait(&status);
            if(WIFEXITED(status))  break;
            
            ptrace(PTRACE_GETREGS,child, NULL, &regs);
            
            if(start == 1) {
                ins = ptrace(PTRACE_PEEKTEXT, child, regs.rip, NULL);
                printf("EIP: %llx Instruction "
                       "executed: %lx\n",regs.rip, ins);
            }

            if(regs.orig_rax == SYS_write) {
                start = 1;
                ptrace(PTRACE_SINGLESTEP, child, NULL, NULL);
            }

            else ptrace(PTRACE_SYSCALL, child, NULL, NULL);

        }
    }
    return 0;
}