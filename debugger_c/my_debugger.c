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

#define MAX_ARGUMENT_LEN 100 


struct CLIArguments {
    bool is_executable ; 
    char *executable_name   ; // name of the executable to run as child. 
}; 

struct DebugSettings {
    bool sys_track ; 
    int sys_track_number ; // sys call number(fd) to track. 
};

struct ChildSettings{
    pid_t child_pid ; 
    int child_status ; 
};

void printCLIArgument(struct CLIArguments *current_args){

    printf("is_executable : %d\n",current_args->is_executable);
    printf("executable_name = %s\n",current_args->executable_name);
    
}

void printUsage(){

    printf("Usage : %s -e [Executable] ", __FILE__);

}

/*
void printRegisters(struct user_regs_struct *current_reg_values){



}
*/

bool validateUserInput(char *str){
    if(strlen(str) > MAX_ARGUMENT_LEN){
        return false ; 
    }
    return true ; 
}

struct CLIArguments argParser(int argc, char *argv[]){

    struct CLIArguments my_args; 
    char **arg = argv; 
    int i = 0 ; 
    
    while(i < argc ){

        if (strlen(argv[i]) > MAX_ARGUMENT_LEN){
            printf("Argument max length is 100\n");
            exit(0);
        }

        // short flags 
        if (argv[i][0]=='-'){
            
            // executable file provided.
            if(argv[i][1]=='e'){
                
                my_args.is_executable = true ; 
            
                if(validateUserInput(argv[i+1]))  {
                    my_args.executable_name = argv[i+1];
                    printf("good input = %s \n",my_args.executable_name ); 
                    i++; 
                }else{
                    exit(1);
                }
            }
        }
        // long flags 
        else if(strncmp(argv[i],"--",2)==0){
            printf("double flag ");
        }
        // constants 
        else{
            printf("not flag ");
        }

        i++; 
    }

    return my_args; 

}


// creates a child process with this and parent process tracks the calls. 
void inspectExecutable(char *filename){

    pid_t child;
    long orig_rax;
    int child_status;
    char buff[100];

    printf("Create child process of %s ? ",filename);
    scanf("%s",buff);

    if(strncmp(buff,"yes",3)==0){
        printf("YESSS!!!!\n");
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

            
            char user_input[MAX_ARGUMENT_LEN];

            // only exits when child process exits or directly exit from user_input     
            while(1){
                
                wait(&child_status); // waits for the kernel signal to continue. kernel gives the authority to parent process.
                if(WIFEXITED(child_status)) break; 

                
                ptrace(PTRACE_GETREGS,child,NULL,&regs);

                // stepper program
                while(1)
                {   
                
                    scanf("%s",user_input);
                    printf("Your input = %s\n",user_input);
                    if(strncmp(user_input,"show regs",9)==0){
                        printf("show regs command !\n");
                        break; 
                    }
                    else if(strncmp(user_input,"cont",4)==0){
                        printf("Continue!!\n");
                        break ; 
                    }
                    else{
                        continue; 
                    }
                
                }

               
                printf("The child made a ""system call %lld\n", regs.orig_rax);
                    
                ptrace(PTRACE_SYSCALL, child, NULL, NULL);

            }

        }

    }
    else if(strncmp(buff,"no",2)==0){
        
        printf("Exiting program .. \n");
        exit(1);
    }   


}




int main(int argc, char *argv[]){


    struct CLIArguments my_cli_args = argParser(argc,argv);
    
    if(my_cli_args.is_executable){
        inspectExecutable(my_cli_args.executable_name);
    }

    return 0 ; 

}