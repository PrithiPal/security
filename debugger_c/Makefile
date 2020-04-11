test: my_debugger dummy

my_debugger: arg_parser.o 
	gcc $@.c $^ -o $@.o

dummy : dummy.c 
	gcc dummy.c -o dummy.o 

clean:
	rm *.o
