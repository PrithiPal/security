# Challenge : crackme0x00a

1. Some information about the file : 
``` file crackme0x00a ``` gives 
``` crackme0x00a: ELF 32-bit LSB executable, Intel 80386, version 1 (SYSV), dynamically linked, interpreter /lib/ld-, for GNU/Linux 2.6.15, BuildID[sha1]=a01d6d16a59c7f0d7ec00ab5454eed2eb22bd20d, not stripped```
Hmm, not stripped first of all. So we do not have to worry about doing some extra work. It is 32bit ELF Binary. So make sure that your Linux have 32bit C libraries for development (https://askubuntu.com/questions/454253/how-to-run-32-bit-app-in-ubuntu-64-bit) is a good way to start.

2. Let's see some of the strings through ``` strings crackme0x00a```
It yields 
``` 
/lib/ld-linux.so.2
__gmon_start__
libc.so.6
_IO_stdin_used
__isoc99_scanf
puts
__stack_chk_fail
printf
strcmp
__libc_start_main
GLIBC_2.7
GLIBC_2.4
GLIBC_2.0
PTRh
D$,1
T$,e3
UWVS
[^_]
Enter password: 
Congrats!
Wrong!
;*2$"
g00dJ0B!
GCC: (Ubuntu/Linaro 4.6.1-9ubuntu3) 4.6.1
.symtab
.strtab
.shstrtab
...
```

We are interested in strings which appears in .text regions. Look at the strings ``` Enter password: ``` , ``` Congrats!```.
3. As we already know that this executable is not malicious, so we can directly execute it on our machines without worrying about its ill-side effects. This is how it looks : 
``` 
prithipalsingh@ubuntu:~/Desktop/shared/security/crackme_challenges/crackme0x00a$ ./crackme0x00a 
Enter password: something
Wrong!
Enter password: something
Wrong!
Enter password: something
Wrong!
Enter password: something
Wrong!
Enter password: ^C
```

So we know that those strings we looked at earlier are used in the code here. There's another string we see : ``` 
g00dJ0B! ```. Is it our password?. Let's try it.
``` prithipalsingh@ubuntu:~/Desktop/shared/security/crackme_challenges/crackme0x00a$ ./crackme0x00a 
Enter password: g00dJ0b!
Wrong!
Enter password: ^[[A
Wrong!
Enter password: g00dJ0B!
Congrats!
``` 

VOILA!. We found our first password. One thing to remember that in this challenge, we did not disassembled the code, because we already saw some sample strings and it turned out to be a password. This may not be the case most of the times(actually always).

