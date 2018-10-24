     1                                  extern puts
     2                                  global main
     3                                  section .text
     4                                  main:
     5 00000000 B841000000                 MOV EAX, 65
     6 00000005 50                         PUSH RAX
     7 00000006 58                         POP RAX
     8 00000007 89C7                       MOV EDI, EAX
     9 00000009 E8(00000000)               CALL puts
    10                                  
    11 0000000E B801000000                 MOV EAX, 1
    12 00000013 C3                         RET
    13                                  
