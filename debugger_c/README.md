Debugger Examples


1. Ptrace example fetched from https://www.linuxjournal.com/article/6100?page=0,1. The example is written for the 32bit systems(IA-32 or i386).
2. Change the EAX = RAX (64bit register identifiers), change <linux/user.h> to <sys/reg.h>
3. reg.h contains the macros containing the offset of these registers.

Other 32bit ot 64bit example for this code : http://theantway.com/2013/01/notes-for-playing-with-ptrace-on-64-bits-ubuntu-12-10/
