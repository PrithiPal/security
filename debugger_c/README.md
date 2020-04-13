# Toy Debugger Project # 

I started this project with intention to learn more about debugging tools. The most common one is GDB(GNU Debugger) and I wanted to replicate some of its features in my version of debugger. 

# Current Features #

- Interactive Shell 
- Analyze static executable 
- Attach external running process with PID 
- Track system calls
- Watch general purpose register values (RAX,RBX,RCX,RDX,RBP,RSP,RSI,RDI) and data-segiment registers(FS,GS) (only 64 bits for now) 

# In progress features # 

- Set breakpoints 
- Inject custom shellcode 
- Change/Alter registers values

# Usage #
``` 
./my_debugger.o 
Usage:
./my_debugger.o  -e [Executable] 
./my_debugger.o  -a [PID] 
```



```
prithipalsingh@ubuntu:~/Desktop/shared/security/debugger_c$ ./my_debugger.o -e dummy.o
Welcome to Prithi's debugger
Executable found : dummy.o
>> 
```
It will spawn a shell to start with debugging the file. Usually the accepted commands are : 
```
>> help
	 ------- Debugger Shell Usage ------- 
	start : start the executable passed or attached process through command line
	help : Show Debugger Shell Usage
	exit : Exits the Debugger Shell
```

The debugger have two modes of operations : executable and attachable process. You can give the 32bit executable to the debugger and it will start tracing it.
#### Executable mode

```
>> start
Creating instance and tracking dummy.o ... 
[23975] dummy.o  
PARENT TRACKING 23975
dummy.o >> show rax
rax : 59 (0x3b)
dummy.o >> show rbx
rbx : 0 (0x0)
dummy.o >> show regs
rax : 59 (0x3b)
rbx : 0 (0x0)
rcx : 0 (0x0)
rdx : 0 (0x0)
rsi : 0 (0x0)
rdi : 0 (0x0)
rsp : 140730443943392 (0x7ffe5c1ecde0)
rbp : 0 (0x0)
rip : 140233669484688 (0x7f8ab20e2090)
dummy.o >> exit
Exiting ... 
```
Upon the ``` start ```, the program will spawn another ``` stdin ``` with ``` dummy.o >>``` which means this shell will accept the examining commands to look into various information about the Tracee (Executable in this case). 

#### Attach mode
This mode allows the user to attach an ongoing external process via its PID and let the debugger take over its ownership(or become its parent for a while). To demonstrate it, here I will start a background instance of a C program which decrements counter from 100 to 0 and calls sleep each iteration. 
```
prithipalsingh@ubuntu:~/Desktop/shared/security/debugger_c$ ./dummy.o &
[1] 24015
prithipalsingh@ubuntu:~/Desktop/shared/security/debugger_c$ Counter  ; 99
Counter  ; 98
Counter  ; 97
...
```
In another shell, we will start the debugger with pid ```24015```. 
```
prithipalsingh@ubuntu:~/Desktop/shared/security/debugger_c$ sudo ./my_debugger.o -a 24015
[sudo] password for prithipalsingh: 
argc=3
good attachable = 24015 
completed!
Welcome to Prithi's debugger
Attachable found : 24015
>> start
Attaching 24015 to debugger ... 
24015 >> 
```
When we start with an attachable process, the ``` stdin ``` shell will have tag ``` [PID] >> ``` as opposed to executable name in Executable mode. It is **important** to use ``` sudo ``` to start the ``` my_debugger ``` because by default, linux will require the root privileges to a process to track another process. Inside the ``` stdin ``` shell, we can look at its register values as we did in Executable mode : 

```
24015 >> show regs
rax : 35 (0x23)
rbx : -128 (0xffffffffffffff80)
rcx : 139652950641060 (0x7f037c83c9a4)
rdx : 0 (0x0)
rsi : 140722949978112 (0x7ffc9d71f800)
rdi : 140722949978112 (0x7ffc9d71f800)
rsp : 140722949978104 (0x7ffc9d71f7f8)
rbp : 0 (0x0)
rip : 139652950641060 (0x7f037c83c9a4)
24015 >> show rax
rax : 35 (0x23)
24015 >> show rbx
rbx : -128 (0xffffffffffffff80)
24015 >> show rcx
rcx : 139652950641060 (0x7f037c83c9a4)
24015 >> exit
Exiting ... 
```

By default, ``` >> exit ``` will kill the traced process as well. 
### Accepted Commands 
I will update the guide as I wil add more functionality to the debugger. As of now, following commands are accepted : 

- `show [Register]` : Show value of a `Register`. Accepts registers are 
    - `rax`, `rbx`, `rcx`, `rdx`, `rbp`, `rsp`, `rbp`, `rip`
- `show regs` : Show values of all registers





*Some Notes* : 

1. Ptrace example fetched from https://www.linuxjournal.com/article/6100?page=0,1. The example is written for the 32bit systems(IA-32 or i386).
2. Change the EAX = RAX (64bit register identifiers), change <linux/user.h> to <sys/reg.h>
3. reg.h contains the macros containing the offset of these registers.
4. reg values for each sys call list : https://filippo.io/linux-syscall-table/ 
Other 32bit ot 64bit example for this code : http://theantway.com/2013/01/notes-for-playing-with-ptrace-on-64-bits-ubuntu-12-10/
