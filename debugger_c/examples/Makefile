all : basic_debugger.o \
		basic_debugger2.o \
		attach.o

%.o : %.c 
	gcc $^  -o  $@

clean:
	rm *.o