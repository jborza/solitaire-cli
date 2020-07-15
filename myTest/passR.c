// more ARM but hardware oriented exercise:
// https://bob.cs.sonoma.edu/IntroCompOrg-RPi/sec-array.html

// pass-reference.c -- show pass by reference using pointers
/* compile with:
cc -g -Wall -o pass-reference pass-reference.c */

#include <stdio.h> // for printf()
#include "loadLinkList.h"

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

void printLinkList(struct linkList* inLinkList){
    printf("pLL: structure list is %p, %c, %i, %p\n", 
    inLinkList, 
    inLinkList->aChar, 
    inLinkList->anInt, 
    inLinkList->aLinkList);
}

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
    
    struct linkList LL1;
    struct linkList LL2;
    struct linkList LL3;

    loadLinkList(&LL1, 'a', 1, &LL3);
    loadLinkList(&LL2, 'b', 2, &LL1);
    loadLinkList(&LL3, 'c', 3, &LL2);

    //struct linkList LL4 = {&LL4, 'd', 4, &LL3}; // not working except for simple structure
    //struct linkList LL5;
    //LL5 = (LinkList) {&LL4, 'd', 4, &LL3};

    // pointer can be %p, %u, %x C99 use p
    // %i/%d
    // 

    printf("LL1: structure list is %p, %c, %i, %p\n", &LL1, LL1.aChar, LL1.anInt, LL1.aLinkList);
    printf("LL2: structure list is %p, %c, %i, %p\n", &LL2, LL2.aChar, LL2.anInt, LL2.aLinkList);
    printf("LL3: structure list is %p, %c, %i, %p\n", &LL3, LL3.aChar, LL3.anInt, LL3.aLinkList);
    //printf("LL4: structure list is %p, %c, %i, %p\n", &LL4, LL4.aChar, LL4.anInt, LL4.aLinkList);

    // LL1/2/3 is a variable of "type" linkList and use .
    // in the function you use pointer -> because the inLinkList 
    //              pointer "type" and is a pointer to structure, 

    printLinkList(&LL1);
    printLinkList(&LL2);
    printLinkList(&LL3);
    //printLinkList(&LL4);

    /* // core dump
    long *lp = (void *)0x10000000;
    for (int i = 0; i < 5; i++) {
        printf ("%d: %lx\n", i, *lp);
        lp++; }
    */

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

/* // https://stackoverflow.com/questions/8019615/strings-and-character-with-printf

#include<stdio.h>

void main()
{
 char name[]="siva";
 printf("name = %p\n", name);
 printf("&name[0] = %p\n", &name[0]);
 printf("name printed as %%s is %s\n",name);
 printf("*name = %c\n",*name);
 printf("name[0] = %c\n", name[0]);
}
Output is:

name = 0xbff5391b  
&name[0] = 0xbff5391b
name printed as %s is siva
*name = s
name[0] = s
So 'name' is actually a pointer to the array of characters in memory. If you try reading the first four bytes at 0xbff5391b, you will see 's', 'i', 'v' and 'a'

Location     Data
=========   ======

0xbff5391b    0x73  's'  ---> name[0]
0xbff5391c    0x69  'i'  ---> name[1]
0xbff5391d    0x76  'v'  ---> name[2]
0xbff5391e    0x61  'a'  ---> name[3]
0xbff5391f    0x00  '\0' ---> This is the NULL termination of the string
To print a character you need to pass the value of the character to printf. The value can be referenced as name[0] or *name (since for an array name = &name[0]).

To print a string you need to pass a pointer to the string to printf (in this case 'name' or '&name[0]').

*/