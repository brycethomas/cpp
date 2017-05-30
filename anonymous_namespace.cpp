namespace {
  int foo() {}
}

int bar() {}

/*  foo has internal linkage, bar has external linkage.
    
    Compile: g++ -c anonymous_namespace.cpp -o anonymous_namespace.o

    Inspect: nm anonymous_namespace.o | c++filt

             0000000000000007 T bar()
             0000000000000000 t (anonymous namespace)::foo()


             objdump anonymous_namespace.o -t

             anonymous_namespace.o:     file format elf64-x86-64
             
             SYMBOL TABLE:
             0000000000000000 l    df *ABS*	0000000000000000 anonymous_namespace.cpp
             0000000000000000 l    d  .text	0000000000000000 .text
             0000000000000000 l    d  .data	0000000000000000 .data
             0000000000000000 l    d  .bss	0000000000000000 .bss
             0000000000000000 l     F .text	0000000000000007 _ZN12_GLOBAL__N_13fooEv
             0000000000000000 l    d  .note.GNU-stack	0000000000000000 .note.GNU-stack
             0000000000000000 l    d  .eh_frame	0000000000000000 .eh_frame
             0000000000000000 l    d  .comment	0000000000000000 .comment
             0000000000000007 g     F .text	0000000000000007 _Z3barv

     NB. nm uses T/t and objdump uses g/l for external/internal.

     See also: https://stackoverflow.com/a/22254251
 */
