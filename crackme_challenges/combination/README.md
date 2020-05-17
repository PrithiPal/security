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
Also to get access to ``win`` procedure, we need second_arg_int and gpass with appropriate values. After looking up a bit, we see program acquire those values inside ``func3()`` void func3(uint param_1)
```
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
