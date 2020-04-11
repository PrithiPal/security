all : basic_debugger basic_debugger2 basic_debugger4 ptrace_toy_example my_debugger

test: my_debugger dummy 

my_debugger: arg_parser
	gcc $@.c $^.c -o my_debugger 

arg_parser:
	gcc arg_parser.c -o arg_parser

dummy : dummy.c 
	gcc dummy.c -o dummy

basic_debugger: basic_debugger.c
	gcc basic_debugger.c -o  basic_debugger.o

basic_debugger2: basic_debugger2.c
	gcc basic_debugger2.c -o  basic_debugger2.o

basic_debugger4: dummy basic_debugger4.c
	gcc basic_debugger4.c -o basic_debugger4.o


ptrace_toy_example : ptrace_toy_example.c 
	gcc ptrace_toy_example.c -o ptrace_toy_example.o

clean:
	rm *.o
