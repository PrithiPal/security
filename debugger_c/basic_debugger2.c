#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/reg.h>
#include <sys/user.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/syscall.h>   /* For SYS_write etc */


int main()
{   
    
    pid_t child;
    long orig_eax, eax, orig_ebx,orig_ecx,orig_edx;
    long params[3];
    int status;
    int insyscall = 0;
    child = fork();

    // child process : TRACEE
    if(child == 0) {
        ptrace(PTRACE_TRACEME, 0, NULL, NULL);
        execl("/bin/ls", "ls", NULL);
    }
    
    // parent process : TRACER
    else {
       while(1) {

          wait(&status);

          // if child process exits.(contains non-zero values.)
          if(WIFEXITED(status))
              break;
            orig_eax = ptrace(PTRACE_PEEKUSER,
                     child, 8 * ORIG_RAX, NULL);

            //printf("value of orig_rax = %ld\n",orig_eax);

            /*
          orig_ebx = ptrace(PTRACE_PEEKUSER,child,4*ORIG_RBX,NULL);
          orig_ecx = ptrace(PTRACE_PEEKUSER,child,4*ORIG_RCX,NULL);
          orig_edx = ptrace(PTRACE_PEEKUSER,child,4*ORIG_RDX,NULL);
            */


          printf("%ld %ld %ld %ld\n",orig_eax,orig_ebx,orig_ecx,orig_edx);
          
          if(orig_eax == SYS_write) {
              //printf("inside\n");
             if(insyscall == 0) {
                /* Syscall entry */
                insyscall = 1;
                params[0] = ptrace(PTRACE_PEEKUSER,
                                   child, 8 * RBX,
                                   NULL);
                params[1] = ptrace(PTRACE_PEEKUSER,
                                   child, 8 * RCX,
                                   NULL);
                params[2] = ptrace(PTRACE_PEEKUSER,
                                   child, 8 * RDX,
                                   NULL);
                printf("Write called with "
                       "%ld, %ld, %ld\n",
                       params[0], params[1],
                       params[2]);
                }
          else { /* Syscall exit */
                eax = ptrace(PTRACE_PEEKUSER,
                             child, 8 * RAX, NULL);
                    printf("Write returned "
                           "with %ld\n", eax);
                    insyscall = 0;
                }
            }
            ptrace(PTRACE_SYSCALL,
                   child, NULL, NULL);


        }
    }
    return 0;
}