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
    bool is_executable  ; 
    char *executable_name ; // name of the executable to run as child. 
}; 

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
            printf("flag ");
            my_args.is_executable = true ; 
            
            if(validateUserInput(argv[i+1]))  {
                my_args.executable_name = argv[i+1];
                printf("good input = %s \n",my_args.executable_name ); 
                i++; 
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

        printf("argv= %s\n",argv[i]); 
    
        i++; 
    }

}

void printUsage(){

    printf("Usage : %s -e [Executable] ", __FILE__);

}



int main(int argc, char *argv[]){

    printf("argc= %d\n",argc);
    
    /*
    int i = 0 ; 
    char **arg = argv ; 
    while(*arg++){
        printf("argv = %s\n",*(argv+i));
        i++;
    }
    */

   argParser(argc,argv);



    return 0 ; 

}