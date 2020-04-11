# Toy Debugger Project # 

I started this project with intention to learn more about debugging tools. The most common one is GDB(GNU Debugger) and I wanted to replicate some of its features in my version of debugger. 

# Current Features #

- Interactive Shell 
- Analyze static executable 
- Attach external running process with PID 
- Track system calls
- Watch general purpose register values (RAX,RBX,RCX,RDX,RBP,RSP,RSI,RDI) and data-segiment registers(FS,GS) (only 64 bits for now) 

# In progress features # 
- set breakpoints - COMING SOON
- inject custom shellcode - COMING SOON
- Change registers values - COMING SOON


# Usage #





*Some Notes* : 

1. Ptrace example fetched from https://www.linuxjournal.com/article/6100?page=0,1. The example is written for the 32bit systems(IA-32 or i386).
2. Change the EAX = RAX (64bit register identifiers), change <linux/user.h> to <sys/reg.h>
3. reg.h contains the macros containing the offset of these registers.
4. reg values for each sys call list : https://filippo.io/linux-syscall-table/ 
Other 32bit ot 64bit example for this code : http://theantway.com/2013/01/notes-for-playing-with-ptrace-on-64-bits-ubuntu-12-10/
