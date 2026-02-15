# Task 1 - The Shell
AA2401221

## KH5003 - Operating Systems, Security and Networking


*The task is used to implement a shell in C++.*

___
## Requirements

- It must support internal commands like (cd, dir, environ, and so on...)
- Other command lines must be interpreted by shell forking and execing.
- The shell must support I/O-redirection on either or both stdin and/or stdout.
- The shell must include batch file mode.
- The shell must support background execution.
- Command line prompt has the pathname of the current directory.

___
## Simple CS Shell Commands
```
Command   | Description
----------|------------------------------------------------------------
cd X      | print current X or change current directory
dir X     | list directory X's contents
environ   | list environment variables
set X Y   | set value Y of variable X
echo X    | prints X
pause     | pause and wait for Enter
quit      | quits shell

Note: Other external commands work.
```
___
