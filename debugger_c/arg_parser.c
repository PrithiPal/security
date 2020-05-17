#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <stdbool.h>

#include "arg_parser.h"


struct CLIArguments argParser(int argc, char *argv[]); 
void printCLIArgument(struct CLIArguments *current_args);
void printUsage();
bool validateUserInput(char *str);

static int i = 2 ; 

struct CLIArguments argParser(int argc, char *argv[]){

    struct CLIArguments my_args; 
    char **arg = argv; 
    int i = 1 ; // first arg is ./my_debugger
    
    printf("argc=%d\n",argc);
    
    if(argc == 1){
        printUsage(); 
        exit(1);         
    }

    while(i < argc ){

        if (strlen(argv[i]) > MAX_ARGUMENT_LEN){
            printf("Argument max length is 100\n");
            exit(0);
        }

        // note : move the -- statement above because - may also get triggered when -- 
        // short flags 
        if (argv[i][0]=='-'){
            
            // executable file provided.
            if(argv[i][1]=='e'){

                my_args.is_executable = true ; 
                my_args.is_attachable = false ; 


                if(argv[i+1])  {

                    my_args.executable_name = argv[i+1];
                    printf("good input = %s \n",my_args.executable_name ); 
                    i++; 
                }
                else{
                    printUsage(); 
                    exit(1);
                }
            }
            // attached external process 
            else if(argv[i][1]=='a'){
                
                my_args.is_executable = false ; 
                my_args.is_attachable = true ; 

                if(argv[i+1]){
                    my_args.attachable_pid =  atoi(argv[i+1]) ; 
                    printf("good attachable = %d \n",my_args.attachable_pid ); 
                    i++ ; 
                }
                else{
                    printUsage(); 
                    exit(1);
                }


            }
            // Help Usage
            else if(argv[i][1]=='h'){
                printUsage(); 
                exit(1);
            }
            else{
                printUsage(); 
                exit(1);                
            }
        }
        else{
                printUsage(); 
                exit(1);            
        }

        
        i++; 


    }
    printf("completed!\n");

    return my_args; 

}

void printCLIArgument(struct CLIArguments *current_args){

    printf("is_executable : %d\n",current_args->is_executable);
    printf("executable_name = %s\n",current_args->executable_name);
    
}

void printUsage(){
    printf("Usage:\n");
    printf("%s -e [Executable] \n", __FILE__);
    printf("%s -a [PID] \n",__FILE__);

}

bool validateUserInput(char *str){
    if(strlen(str) > MAX_ARGUMENT_LEN){
        return false ; 
    }
    return true ; 
}

/*
int main(){
    return 0 ; 
}
*/