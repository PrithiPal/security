#ifndef ARG_PARSER_H
#define ARG_PARSER_H

#define MAX_ARGUMENT_LEN 100 

struct CLIArguments {
    bool is_executable ; 
    char *executable_name   ; // name of the executable to run as child. 
    bool is_attachable ; 
    pid_t attachable_pid ; 
};

struct CLIArguments argParser(int argc, char *argv[]); 
void printCLIArgument(struct CLIArguments *current_args);
void printUsage();
bool validateUserInput(char *str);


#endif 