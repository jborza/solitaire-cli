// more ARM but hardware oriented exercise:
// https://bob.cs.sonoma.edu/IntroCompOrg-RPi/sec-array.html

// pass-reference.c -- show pass by reference using pointers
/* compile with:
cc -g -Wall -o pass-reference pass-reference.c */

#include <stdio.h> // for printf()

int arrayAdd1(int n)
{
  int intArray[n];
  // register int index;
   
  for (int index = 0; index < n; index++)
    intArray[index] = index;

  int sum = 0;

  for (int index = 0; index < n; index++){
    printf("%i, ", intArray[index]);
    sum = sum + intArray[index];
  }

  printf("\n");

  return sum;
}

int arrayAdd2(int m, int intArray[])
{
  int sum = 0;

  for (int index = 0; index < m; index++){
    printf("%i, ", intArray[index]);
    sum = sum + intArray[index];
  }

  printf("\n");

  return sum;
}

int addemUp (int a, int b, int *result) {
    *result = a + b; // not using functional approach or return (result)
    return *result; // not result as it is a pointer nor *result but definitely not & result
} // addemUp

int main (int argc, char *argv[]) {
    int answer1, answer2;
    answer1=2;
    answer2=256*256;
    answer2 = addemUp (2, 256*256, &answer1);
    printf ("1 + 256*256 = %d or %d\n", answer1, answer2); 
    int n = 10;
    printf ("Add up to %i = %i\n", n, arrayAdd1(n));
    int m = 10;
    int intArray[m]; // beware this is NOT dynamic allocation
    for (int index = 0; index < m; index++)
        intArray[index] = index;
    printf ("Add up to %i = %i\n", m, arrayAdd2(m, intArray));
    printf ("Add up to %i = %i\n", m, arrayAdd2(m, &intArray[0]));
    // this is the danger or c array with implicit pointer reference
    //   first it use poiniter ref without telling you
    //   and when you use pointer ref it is not obvious
    //   you cannot use &intArray as it would be "int (*)[10]"
    //   what expect is "int *"
    

    return (0);
} // main

/*

For help, type "help".
Type "apropos word" to search for commands related to "word"...
Reading symbols from solitaire...
(gdb) linenumber
Undefined command: "linenumber".  Try "help".
(gdb) linenumber on
Undefined command: "linenumber".  Try "help".
(gdb) help
List of classes of commands:

aliases -- Aliases of other commands.
breakpoints -- Making program stop at certain points.
data -- Examining data.
files -- Specifying and examining files.
internals -- Maintenance commands.
obscure -- Obscure features.
running -- Running the program.
stack -- Examining the stack.
status -- Status inquiries.
support -- Support facilities.
tracepoints -- Tracing of program execution without stopping the program.
user-defined -- User-defined commands.

Type "help" followed by a class name for a list of commands in that class.
Type "help all" for the list of all commands.
Type "help" followed by command name for full documentation.
Type "apropos word" to search for commands related to "word".
Type "apropos -v word" for full documentation of commands related to "word".
Command name abbreviations are allowed if unambiguous.
(gdb) ehlp all
Undefined command: "ehlp".  Try "help".
(gdb) breakpoints 10
Undefined command: "breakpoints".  Try "help".
(gdb) breakpoint 10 
Undefined command: "breakpoint".  Try "help".
(gdb) run
Starting program: /src/linux/solitaire 
warning: Error disabling address space randomization: Operation not permitted

(gdb) b 15
Breakpoint 2 at 0x11cc: file passR.c, line 15.
(gdb) r
Starting program: /src/myTest/passR 
warning: Error disabling address space randomization: Operation not permitted

Breakpoint 1, main (argc=1, argv=0x7ffc3393cba8) at passR.c:14
14	    answer2 = addemUp (2, 256, &answer1);
(gdb) n

Breakpoint 2, main (argc=1, argv=0x7ffc3393cba8) at passR.c:15
15	    printf ("1 + 256 = %d or %d\n", answer1, answer2); 
(gdb) i r 
rax            0x102               258
rbx            0x56273184f200      94726334509568
rcx            0x56273184f200      94726334509568
rdx            0x102               258
rsi            0x100               256
rdi            0x2                 2
rbp            0x7ffc3393cab0      0x7ffc3393cab0
rsp            0x7ffc3393ca90      0x7ffc3393ca90
r8             0x0                 0
r9             0x7f53f20c2d50      139998519897424
r10            0x0                 0
r11            0x0                 0
r12            0x56273184f080      94726334509184
r13            0x7ffc3393cba0      140721173810080
r14            0x0                 0
r15            0x0                 0
rip            0x56273184f1cc      0x56273184f1cc <main+59>
eflags         0x202               [ IF ]
cs             0x33                51
ss             0x2b                43
ds             0x0                 0
es             0x0                 0
fs             0x0                 0
gs             0x0                 0
(gdb) x /4xb &answer1
0x7ffc3393caa0:	0x02	0x01	0x00	0x00
(gdb) x /4xb &answer2
0x7ffc3393caa4:	0x02	0x01	0x00	0x00
(gdb) x /4xw &answer2
0x7ffc3393caa4:	0x00000102	0x5ce67100	0xee4391f9	0x00000000
(gdb) p &answer1
$1 = (int *) 0x7ffc3393caa0
(gdb) p answer1
$2 = 258
(gdb) q

*/

/*

cc -g -o passR -DUNICODE -Wall passR.c -lncursesw -ltinfo
root@df550a16eb2d:/src/myTest# gdb ./passR
GNU gdb (Ubuntu 9.1-0ubuntu1) 9.1
Copyright (C) 2020 Free Software Foundation, Inc.
License GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>
This is free software: you are free to change and redistribute it.
There is NO WARRANTY, to the extent permitted by law.
Type "show copying" and "show warranty" for details.
This GDB was configured as "x86_64-linux-gnu".
Type "show configuration" for configuration details.
For bug reporting instructions, please see:
<http://www.gnu.org/software/gdb/bugs/>.
Find the GDB manual and other documentation resources online at:
    <http://www.gnu.org/software/gdb/documentation/>.

For help, type "help".
Type "apropos word" to search for commands related to "word"...
Reading symbols from ./passR...
(gdb) b 16
Breakpoint 1 at 0x11c1: file passR.c, line 16.
(gdb) b17
Undefined command: "b17".  Try "help".
(gdb) b 17
Breakpoint 2 at 0x11da: file passR.c, line 17.
(gdb) r
Starting program: /src/myTest/passR 
warning: Error disabling address space randomization: Operation not permitted

Breakpoint 1, main (argc=1, argv=0x7ffc824f8c78) at passR.c:16
16	    answer2 = addemUp (2, 256*256, &answer1);
(gdb) x /4xw &answer1
0x7ffc824f8b70:	0x00000002	0x00010000	0x86214000	0xb144a4b1
(gdb) p answer1
$1 = 2
(gdb) p &answer1
$2 = (int *) 0x7ffc824f8b70
(gdb) p *answer1
Cannot access memory at address 0x2
(gdb) n

Breakpoint 2, main (argc=1, argv=0x7ffc824f8c78) at passR.c:17
17	    printf ("1 + 256*256 = %d or %d\n", answer1, answer2); 
(gdb) x /4xw &answer1
0x7ffc824f8b70:	0x00010002	0x00010002	0x86214000	0xb144a4b1
(gdb) q
*/