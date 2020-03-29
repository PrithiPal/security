all : basic_debugger basic_debugger2 ptrace_toy_example


basic_debugger: basic_debugger.c
	gcc basic_debugger.c -o  basic_debugger.o

basic_debugger2: basic_debugger2.c
	gcc basic_debugger2.c -o  basic_debugger2.o

ptrace_toy_example : ptrace_toy_example.c 
	gcc ptrace_toy_example.c -o ptrace_toy_example.o

clean:
	rm *.o
