#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/reg.h>
#include <sys/user.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/syscall.h>   /* For SYS_write etc */
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

#include "arg_parser.h"


struct DebugSettings {
    bool sys_track ; 
    int sys_track_number ; // sys call number(fd) to track. 
};

struct ChildSettings{
    pid_t child_pid ; 
    int child_status ; 
};



void printDebuggerShellUsage(){
    char *buff=\
    "\t ------- Debugger Shell Usage ------- \n"\
    "\tstart : start the executable passed or attached process through command line\n"\
    "\thelp : Show Debugger Shell Usage\n"\
    "\texit : Exits the Debugger Shell\n";
    printf("%s",buff);
}


void lowerBuffer(char *str, int strSize){
    for(int i = 0 ; i < strSize; i ++ ){
        str[i] = tolower(str[i]);
    }
}



int processInput(char *inp,int inpSize, struct user_regs_struct *reg ){
    //printf("before string = %s\n",inp);
    
    // input validation control-block.
    if(inpSize==0){
        printf("Illegal command\n");
        return 1 ; 
    }
    char input[1000]="";
    
    strncpy(input,inp,inpSize);
    lowerBuffer(input,inpSize);
    
    
    char *p = strtok(input," "); 
    //printf("p = %s\n",p);
    
    if(strncmp(p,"show",4)==0){
        p = strtok(NULL," ");
        
        if(p==NULL){
            printf("unrecognized command\n");
            return 1 ; 
        }

        if(strncmp(p,"rax",3)==0){
            printf("rax : %lld (0x%llx)\n",reg->orig_rax,reg->orig_rax);
        }
        else if(strncmp(p,"rbx",3)==0){
            printf("rbx : %lld (0x%llx)\n",reg->rbx,reg->rbx);
        }
        else if(strncmp(p,"rcx",3)==0){
            printf("rcx : %lld (0x%llx)\n",reg->rcx,reg->rcx);
        }
        else if(strncmp(p,"rdx",3)==0){
            printf("rdx : %lld (0x%llx)\n",reg->rdx,reg->rdx);
        }
        else if(strncmp(p,"rsi",3)==0){
            printf("rsi : %lld (0x%llx)\n",reg->rsi,reg->rsi);
        }
        else if(strncmp(p,"rdi",3)==0){
            printf("rdi : %lld (0x%llx)\n",reg->rdi,reg->rdi);
        }
        else if(strncmp(p,"rsp",3)==0){
            printf("rsp : %lld (0x%llx)\n",reg->rsp,reg->rsp);
        }
        else if(strncmp(p,"rbp",3)==0){
            printf("rbp : %lld (0x%llx)\n",reg->rbp,reg->rbp);
        }
        else if(strncmp(p,"rip",3)==0){
            printf("rip : %lld (0x%llx)\n",reg->rip,reg->rip);
        }
        else{
            printf("unrecognized!!\n");
        }
        
    }
    else if(strncmp(p,"cont",4)==0){

        return 0 ; 
    }
    else if(strncmp(p,"exit",4)==0){
        printf("Exiting ... \n");
        exit(1);
    }
    else{
        printf("unrecognized command %s\n",p);
    }

    return 1 ; 

}




void StartDebuggingSession(char *filename){
    pid_t child;
    long orig_rax;
    int child_status;
    int user_buffer_size=0;

    //printf("Starting %s ... \n",filename);
    child = fork();
    
    // child process 
    if(child == 0) {
        // start tracing 
        ptrace(PTRACE_TRACEME, 0, NULL, NULL);
        char execute_cmd[2+strlen(filename)];
        sprintf(execute_cmd,"./%s",filename);
        execl(execute_cmd, filename, NULL);
    
    }
    else
    // parent process  
    {
        // definition stored in <sys/user.h>
        struct user_regs_struct regs;
        char user_input[MAX_ARGUMENT_LEN]="";
        char *ptr = user_input ; 
        int is_step = 1 ; 
        int userSize = 0 ; 
        char ch ; 
        // only exits when child process exits or directly exit from user_input     
        while(1){
            
            wait(&child_status); // waits for the kernel signal to continue. kernel gives the authority to parent process.
            if(WIFEXITED(child_status)) break; 
            ptrace(PTRACE_GETREGS,child,NULL,&regs);
            
            // stepper program shell
            ch = fgetc(stdin) ;    
            while(1){
                  
                printf("%s >> ",filename);
                
                // parses user input 
                while(1){    
                    ch = fgetc(stdin) ; 
                    if(ch=='\n'){
                        is_step = processInput(user_input,userSize,&regs);
                        ptr = user_input ; 
                        
                        
                        break ;  
                    }else{
                        *ptr++ = ch ; 
                        userSize++; 
                    }
                    
                }  
                userSize = 0 ; 
                if(!is_step) break ; 
            }

            printf("The child made a system call %lld\n", regs.orig_rax);
            ptrace(PTRACE_SYSCALL, child, NULL, NULL);
            
        }

    }

}
// creates a child process with this and parent process tracks the calls. 
void inspectExecutable(struct CLIArguments *cli_args){

    char buff[100];
    char filename[MAX_ARGUMENT_LEN] = "" ; 
    pid_t attach_pid = 0 ; 
    // note : take the debugger welcome input from the file.
    printf("Welcome to Prithi's debugger\n");
    
    if(cli_args->is_executable){
        strcpy(filename,cli_args->executable_name); 
        printf("Executable found : %s\n",filename);
    }
    else if(cli_args->is_attachable){
        attach_pid = cli_args->attachable_pid ; 
        printf("Attachable found : %d\n",attach_pid);
    }
    

    // note : validate scanf input.

    // Debugger shell 
    while(1){
        
        printf(">> ");
        scanf("%s",buff);     

        if(strncmp(buff,"start",3)==0){
            if(cli_args->is_executable) StartDebuggingSession(filename);
            if(cli_args->is_attachable) printf("START ATTACHABLE SESSION with pid %d\n",cli_args->attachable_pid); 
        }
        else if(strncmp(buff,"exit",2)==0){
            printf("Exiting program .. \n");
            exit(1);
        }
        else if(strncmp(buff,"help",4)==0){
            printDebuggerShellUsage();
            continue;
        }
        else{
            printf("Invalid command %s\n",buff);
            continue;
        }

    }

}



int main(int argc, char *argv[]){


    struct CLIArguments my_cli_args = argParser(argc,argv);
    inspectExecutable(&my_cli_args);


    return 0 ; 

}