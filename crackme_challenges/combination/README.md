# Challenge : combination
source: https://github.com/sigpwny/RE_Labs

1. As usual, some information about the executable: 
``` 
root@07acf29d2533:/home/vm/RE_Labs# file combination
combination: ELF 32-bit LSB executable, Intel 80386, version 1 (SYSV), dynamically linked, interpreter /lib/ld-linux.so.2, for GNU/Linux 2.6.24, BuildID[sha1]=d5c6f6c5e97578e8d74e882a503c82241f9af9e5, not stripped
```
It's an 32bit ELF executable.

2. Perform a string analysis
```
root@07acf29d2533:/home/vm/RE_Labs# strings combination
__JCR_LIST__
deregister_tm_clones
register_tm_clones
__do_global_dtors_aux
completed.6591
__do_global_dtors_aux_fini_array_entry
frame_dummy
__frame_dummy_init_array_entry
combination.c
tracerPid.3748
__FRAME_END__
__JCR_END__
__init_array_end
_DYNAMIC
__init_array_start
_GLOBAL_OFFSET_TABLE_
__libc_csu_fini
func3
strstr@@GLIBC_2.0
func1
read@@GLIBC_2.0
_ITM_deregisterTMCloneTable
__x86.get_pc_thunk.bx
data_start
printf@@GLIBC_2.0
fail
_edata
_fini
func1_helper
__stack_chk_fail@@GLIBC_2.4
__data_start
puts@@GLIBC_2.0
__gmon_start__
exit@@GLIBC_2.0
__dso_handle
open@@GLIBC_2.0
_IO_stdin_used
func4
__libc_start_main@@GLIBC_2.0
func2
func0
__libc_csu_init
_end
_start
_fp_hw
__bss_start
isDebuggerPresent
main
gpass
_Jv_RegisterClasses
exp@@GLIBC_2.0
atoi@@GLIBC_2.0
__TMC_END__
_ITM_registerTMCloneTable
_init
```
Some interesting things to note here. isDebuggerPresent gives a hint to the fact that the executable may contain some code to detect a tracer or a Debuger. We will find more about once we will disassembler.

3. I am using Ghidra for static analysis and disassembly. This is how the de-compiled C like code looks like : 
```
void main(int argc,undefined4 *argv)

{
  int second_arg_int;
  char *second_arg;
  int counter;
  code *apcStack36 [4];
  code *local_14;
  
  if (argc < 2) {
    printf("usage: %s <arg1> <arg2> ...\n",*argv);
                    /* WARNING: Subroutine does not return */
    exit(-1);
  }
  apcStack36[0] = func0;
  apcStack36[1] = func1;
  apcStack36[2] = func2;
  apcStack36[3] = func3;
  local_14 = func4;
  second_arg = (char *)argv[1];
  second_arg_int = atoi(second_arg);
  counter = 0;
  while (counter < second_arg_int) {
    if (argc + -1 <= counter) {
      fail(second_arg);
    }
    second_arg = (char *)atoi((char *)argv[counter + 1]);
    (*apcStack36[counter])();
    counter = counter + 1;
  }
  second_arg_int = isDebuggerPresent();
  if ((second_arg_int == 0) && (gpass == 1)) {
    win();
  }
  fail(second_arg);
  return;
}
```

I have renamed some variables for clarity such as ``second_arg`` ``second_arg_int`` and some more. A quick rough looks like we want to reach ```win()``` to win the challenge.
Probably on winning we will get a message like this : 
```
void win(void)

{
  puts("You win!");
                    /* WARNING: Subroutine does not return */
  exit(0);
}

```

4. Okay, its time to analyze the main routine. After some local variable declarations, we go inside loop and would like to avoid triggering fail(). So We will make sure that ``argc`` have some values until we reach win.
Also to get access to ``win`` procedure, we need second_arg_int and gpass with appropriate values. After looking up a bit, we see program acquire those values inside ``func3()`` 
```
void func3(uint param_1)
{
  double dVar1;
  
  dVar1 = exp((double)(ulonglong)param_1);
  if (dVar1 != 26489122129.84347153) {
    fail();
  }
  gpass = 1;
  puts("You haven\'t lost yet!");
  return;
}
```
To reach this function, the loop has an array of function pointer to function prototype ```func[number]```. For earlier, we knew we atleast should have argc = 5 (which means 4 inputs because 1st argument is executable name ``combination``. Inside func1 function. So in function pointers array line inside main() ``` (*apcStack36[counter])() ```, there's no argument. If we look at x86 code, it looks like this : 
```
        08048701 89 44 24 18     MOV        dword ptr [ESP + local_28],second_arg_int
        08048705 8b 44 24 10     MOV        second_arg_int,dword ptr [ESP + counter]
        08048709 8b 44 84 1c     MOV        second_arg_int,dword ptr [ESP + second_arg_int
        0804870d 8b 54 24 18     MOV        EDX,dword ptr [ESP + local_28]
        08048711 89 14 24        MOV        dword ptr [ESP]=>second_arg,EDX
        08048714 ff d0           CALL       second_arg_int
        08048716 83 44 24        ADD        dword ptr [ESP + counter],0x1
                 10 01
```
the CALL to second_arg_int is preceeded by EDX, which means EDX contains the argument to the function pointer call which is second_arg_int. So our arguments to executable are inputs to individual functions `` func0()``, `` func1()``, `` func2()`` , `` func3()`` and ``func4()``.

4. So our understanding till now is, we provide command line arguments like ```./combination 4 1 2 3 4 .. ```, first number determines how many iterations in while loop will be. We don't want to execute func4() so, our first number should be 4 for sure. this way loop will execute function 0 till 3 and omit 4th which is bad exit function.

5. Inside ``func1()``, the decompiled looks like : 
```
void func1(undefined4 param_1)

{
  int iVar1;
  
  iVar1 = func1_helper(param_1);
  if (iVar1 != 0xa18) {
    fail();
  }
  puts("Another challenge complete.");
  return;
}
```
We want ``` 0xa18``` in the variable which is 2584 in base-10. We also notice that func1_helper is a fibbonacci series.
```
int func1_helper(int param_1)

{
  int iVar1;
  int iVar2;
  
  if (param_1 == 0) {
    iVar1 = 0;
  }
  else {
    if (param_1 == 1) {
      iVar1 = 1;
    }
    else {
      iVar2 = func1_helper(param_1 + -2);
      iVar1 = func1_helper(param_1 + -1);
      iVar1 = iVar1 + iVar2;
    }
  }
  return iVar1;
}
```
In this series, first element is 0 and second element is 1. A quick visit to https://www.dcode.fr/fibonacci-numbers will help us decide the value of ``n`` for desired output of 2584. It looks like N=18, has this fibbonaci number. Let's verify in the command line.

```
root@07acf29d2533:/home/vm/RE_Labs# ./combination 5 18         
Good first step.
Another challenge complete.
You lose!
```
Good. we solved another function. On our way to next function.

6. the ``func2()`` looks something like this : 

```
void func2(int param_1)

{
  if (((param_1 * 2 + -1) * param_1 & 0x7fffffffU) == 0x76355d06) {
    puts("You\'re almost there.");
  }
  else {
    fail();
  }
  return;
}
```
This if statment is hard to understand. Maybe its x86 version, will be more clear: 

```
        080487e0 8b 45 08        MOV        EAX,dword ptr [EBP + param_1]
        080487e3 01 c0           ADD        EAX,EAX
        080487e5 83 e8 01        SUB        EAX,0x1
        080487e8 0f af 45 08     IMUL       EAX,dword ptr [EBP + param_1]
        080487ec 01 c0           ADD        EAX,EAX
        080487ee d1 e8           SHR        EAX,1
        080487f0 89 45 f4        MOV        dword ptr [EBP + local_10],EAX
        080487f3 81 7d f4        CMP        dword ptr [EBP + local_10],0x76355d06
                 06 5d 35 76
        080487fa 75 0e           JNZ        LAB_0804880a

```

Okay. So let's say ``param_1=p``, add to itself, subtracting 1 and multipliying by p becomes `` p(2p-1)``. We add the quantity to itself which is essentially same as multiplying by 2. The quantity becomes ``` 2p(2p-1)```. the SHR instruction shifts bits to right which is equivalent to division by 2 in base-10 system. Our quantity is now back to ``` p(2p-1)```. Now this quantity is compared to ``0x76355d06`` or ``1983208710``. Now solving the equation ``p(2p-1)=1983208710`` for p will give us our command line argument. After solving on wolfram(https://www.wolframalpha.com/), we get p=31490. Let's check again,

```
root@07acf29d2533:/home/vm/RE_Labs# ./combination 4 18 31490
Good first step. ## from func2()
Another challenge complete. ## from func1()
You're almost there. ## from func2()
You lose!
```
Nice. Our number is correct. This function analysis is one of those examples where x86 helps us better understanding the mathematical operation. The de-compiled C like output did not gave good human interpretation(it's possible expert reverse-engineers can do without x86).

7. The next function ``func3()`` is easy. 
```
void func3(uint param_1)

{
  double dVar1;
  
  dVar1 = exp((double)(ulonglong)param_1);
  if (dVar1 != 26489122129.84347153) {
    fail();
  }
  gpass = 1;
  puts("You haven\'t lost yet!");
  return;
}
```
. The equation formed here is ``` e^p = 26489122129.84347153 ``` which yields 24. Its a good number again.
```
root@07acf29d2533:/home/vm/RE_Labs# ./combination 4 18 31490 24
Good first step. ## from 4 
Another challenge complete. ## from 18
You're almost there. ## from 31490
You haven't lost yet! ## from 24
You win! ## from not calling func4()
```.

Here it is. We solved the challenge.
