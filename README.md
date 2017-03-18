# IS-521 Activity: Backdoor

### Contents
* ./interpreter/
  * interpreter.c: Read file, put codes in allocated memory, and trigger handler of opcodes. Remove pc and put it in VMContext.
  * minivm.h: Added pc, code, codesize and heap in VMContext. Also contains prototypes of opcodes
  * minivm.c: Implements opcodes. *dispatch* function now checks validity of opcode.

* ./login/
  * login.mini: Login program in mini language.
* ./test/ test.mini: Test program in mini language. Refer to [Link](https://github.com/KAIST-IS521/backdoor-ian0371/blob/master/test/test.md)
  * test.md: Description of test program.
  * test.c: C implementation of test program.

* ./backdoor/
  * minivm.c: Backdoor code added.
  * interpreter.c, minivm.c: Same as above (interpreter)

* ./compiler/
  * compiler.ml: Converts mini language into bytecode that can be run on interpreter.

### Backdoor logic
In first call to *gets*, it checks if user is *superuser*.If it is, it sets the flag `is_superuser` and replace the string "superuser" with "user" to bypass first check.

In second call to *gets* where password is checked, it first checks if `is_superuser` flag is set. 
If it is, backdoor fills input buffer with "password" to bypass second check. In this step, printing "Password: " is skipped by also checking the flag.

Since all checks are bypassed, the program will simply print "Success\n". Not only does it work with my login.mini, but also with others.

### What I learned
I learned how to convert a program given in C to bytecode using only given sets of assembly. Also I had never thought about low-level backdoors such as hardware or interpreter, and it was quite a shock to know their existence and pretty fun implementing it by myself. Those kinds of backdoors will be very strong and I learned that a software cannot be trusted by its source code level.

