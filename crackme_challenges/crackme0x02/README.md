# Challenge : crackme0x00a

1. Some information about the file : 
``` file crackme0x00a ``` gives 
``` crackme0x02: ELF 32-bit LSB executable, Intel 80386, version 1 (SYSV), dynamically linked, interpreter /lib/ld-, for GNU/Linux 2.6.9, not stripped```
Hmm, not stripped first of all. So we do not have to worry about doing some extra work. It is 32bit ELF Binary. So make sure that your Linux have 32bit C libraries for development (https://askubuntu.com/questions/454253/how-to-run-32-bit-app-in-ubuntu-64-bit) is a good way to start.

2. Let's see some of the strings through ``` strings crackme0x00a```
It yields 
``` 
prithipalsingh@ubuntu:~/Desktop/shared/security/crackme_challenges/crackme0x02$ strings crackme0x02 | head -n 30 
/lib/ld-linux.so.2
__gmon_start__
libc.so.6
printf
scanf
_IO_stdin_used
__libc_start_main
GLIBC_2.0
PTRh
IOLI Crackme Level 0x02
Password: 
Password OK :)
Invalid Password!
GCC: (GNU) 3.4.6 (Gentoo 3.4.6-r2, ssp-3.4.6-1.0, pie-8.7.10)
GCC: (GNU) 3.4.6 (Gentoo 3.4.6-r2, ssp-3.4.6-1.0, pie-8.7.10)
GCC: (GNU) 3.4.6 (Gentoo 3.4.6-r2, ssp-3.4.6-1.0, pie-8.7.10)
GCC: (GNU) 3.4.6 (Gentoo 3.4.6-r2, ssp-3.4.6-1.0, pie-8.7.10)
GCC: (GNU) 3.4.6 (Gentoo 3.4.6-r2, ssp-3.4.6-1.0, pie-8.7.10)
GCC: (GNU) 3.4.6 (Gentoo 3.4.6-r2, ssp-3.4.6-1.0, pie-8.7.10)
GCC: (GNU) 3.4.6 (Gentoo 3.4.6-r2, ssp-3.4.6-1.0, pie-8.7.10)
.symtab
.strtab
.shstrtab
.interp
.note.ABI-tag
.gnu.hash
.dynsym
.dynstr
.gnu.version
.gnu.version_r
```

There is not any interesting string present here. The only recognizable strings appears in the prompt message for executable such as ``` IOLI Crackme Level 0x02 ```, ``` Password: ``` and ``` Invalid Password! ```.
3. As we already know that this executable is not malicious, so we can directly execute it on our machines without worrying about its ill-side effects. This is how it looks : 

``` 
prithipalsingh@ubuntu:~/Desktop/shared/security/crackme_challenges/crackme0x02$ ./crackme0x02 
IOLI Crackme Level 0x02
Password: f
Invalid Password!
```
4. Let's dissassemble the binary. I will be using Ghidra here : (https://github.com/NationalSecurityAgency/ghidra). It is one of the good open source disassemblers/decompilers with reach feature content. The decompiled look something like this : 
``` 
undefined4 main(void)
{
  int user_input;
  
  printf("IOLI Crackme Level 0x02\n");
  printf("Password: ");
  scanf("%d",&user_input);
  if (user_input == 0x52b24) {
    printf("Password OK :)\n");
  }
  else {
    printf("Invalid Password!\n");
  }
  return 0;
}
```
I have changed the pre-existing label to ``` user_input ``` to make code a bit clear. As evident from the high-level overview of our binary, it simply compares the user-input with the hexadecimal number ``` 0x52b24```. However I would take another approach to crack this. 

5. Using gdb, Below is the Intel 32bit disassembled output for the binary : 
``` 
(gdb) disas main 
Dump of assembler code for function main:
   0x080483e4 <+0>:	push   ebp
   0x080483e5 <+1>:	mov    ebp,esp
   0x080483e7 <+3>:	sub    esp,0x18
   0x080483ea <+6>:	and    esp,0xfffffff0
   0x080483ed <+9>:	mov    eax,0x0
   0x080483f2 <+14>:	add    eax,0xf
   0x080483f5 <+17>:	add    eax,0xf
   0x080483f8 <+20>:	shr    eax,0x4
   0x080483fb <+23>:	shl    eax,0x4
   0x080483fe <+26>:	sub    esp,eax
   0x08048400 <+28>:	mov    DWORD PTR [esp],0x8048548
   0x08048407 <+35>:	call   0x804831c <printf@plt>
   0x0804840c <+40>:	mov    DWORD PTR [esp],0x8048561
   0x08048413 <+47>:	call   0x804831c <printf@plt>
   0x08048418 <+52>:	lea    eax,[ebp-0x4]
   0x0804841b <+55>:	mov    DWORD PTR [esp+0x4],eax
   0x0804841f <+59>:	mov    DWORD PTR [esp],0x804856c
   0x08048426 <+66>:	call   0x804830c <scanf@plt>
   0x0804842b <+71>:	mov    DWORD PTR [ebp-0x8],0x5a
   0x08048432 <+78>:	mov    DWORD PTR [ebp-0xc],0x1ec
   0x08048439 <+85>:	mov    edx,DWORD PTR [ebp-0xc]
   0x0804843c <+88>:	lea    eax,[ebp-0x8]
   0x0804843f <+91>:	add    DWORD PTR [eax],edx
   0x08048441 <+93>:	mov    eax,DWORD PTR [ebp-0x8]
   0x08048444 <+96>:	imul   eax,DWORD PTR [ebp-0x8]
   0x08048448 <+100>:	mov    DWORD PTR [ebp-0xc],eax
   0x0804844b <+103>:	mov    eax,DWORD PTR [ebp-0x4]
   0x0804844e <+106>:	cmp    eax,DWORD PTR [ebp-0xc]
   0x08048451 <+109>:	jne    0x8048461 <main+125>
   0x08048453 <+111>:	mov    DWORD PTR [esp],0x804856f
   0x0804845a <+118>:	call   0x804831c <printf@plt>
   0x0804845f <+123>:	jmp    0x804846d <main+137>
   0x08048461 <+125>:	mov    DWORD PTR [esp],0x804857f
   0x08048468 <+132>:	call   0x804831c <printf@plt>
   0x0804846d <+137>:	mov    eax,0x0
   0x08048472 <+142>:	leave  
   0x08048473 <+143>:	ret  
```

We are interested in the comparison on instruction address ``` 0x0804844e ```. So we will set two breakpoints at ``` CMP ``` instruction and one before(I will tell you why~). Then we can run and directly jump before the breakpoint.
```
(gdb) break *0x0804844b
Breakpoint 1 at 0x804844b
(gdb) break *0x0804844e
Breakpoint 2 at 0x0804844e
(gdb) r
The program being debugged has been started already.
Start it from the beginning? (y or n) y
Starting program: /mnt/hgfs/vm/challenges/crackme0x02 
IOLI Crackme Level 0x02
Password: xxxx

Breakpoint 1, 0x0804844b in main ()
(gdb) p/x $eax
$2 = 0x52b24
(gdb) x/4cb $ebp-0xc
0xffffd11c:	36 '$'	43 '+'	5 '\005'	0 '\000'
(gdb) x/4xb $ebp-0xc
0xffffd11c:	0x24	0x2b	0x05	0x00
```
This is one our first breakpoint. As you can see both the operand of the ``` cmp ``` have value ``` 0x52b24``` (Because IA-32 s little-endian, so we will reverse the sequence bytes to interpret them). But what happens on the next address : 
```
(gdb) p/x $eax
$3 = 0xf7fb5000
(gdb) x/4xb $ebp-0xc
0xffffd11c:	0x24	0x2b	0x05	0x00
(gdb) 
```
6. Everything was going smoothly until ``` eax ``` changes it values from ``` 0x52b24``` to ```0xf7fb5000 ```. This means the ``` cmp ``` instruction will fail because ``` 0x52b24 ``` (``` $ebp-0xc``` value) != ``` 0xf7fb5000``` (``` EAX ```value ). So what we can do at this point is to manually one of the operands. In Debugging, it is easier to change registers value because changing memory results in SEGFAULT. 

```
(gdb) set $eax=0x52b24
(gdb) p/x $eax
$4 = 0x52b24
(gdb) x/4xb $ebp-0xc
0xffffd11c:	0x24	0x2b	0x05	0x00
```
Now both the operands are same. Lets run it again.
```
(gdb) step
Single stepping until exit from function main,
which has no line number information.
Password OK :)
0xf7df5e81 in __libc_start_main () from /lib/i386-linux-gnu/libc.so.6
```

See. we have bypassed the password check. Now recall from earlier, As i showed the ghidra's decompiled output, it has the same address ``` 0x52b24```. I think the decompiled output was correct in the sense that string ``` 0x52b24 ``` is being compared however when we stepped through the program, the program just switches the ``` EAX ``` values so that ``` cmp ``` will fail and it will raise ``` Invalid Password ! ```. This is interesting in this challenge. 

This crackme challenges specifically teaches us to perform all stack analysis (including decompiled output, disassembled output) on different tools to get a overall sense. In the end, we stepped through the program(dyanmic analysis) and altered the memory to make the control flow obey us. Although we have bypassed the executable through debugger, I have not successfully bypassed from stdin (Actually passing raw bytes from console). 




