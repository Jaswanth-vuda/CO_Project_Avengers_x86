.data
arr:    .word 10,20,30,40,
.text
.globl main
main:
    ADDI R2,R2,4,
    ADDI R3,R3,5,
    LW R7,4(arr),
    SW R7,8(arr),
    ADD R1,R2,R3,
    SUB R4,R2,R3,
    ADD R5,R1,R4,
loop:
    BEQ     R2,R3,lab,
    SUBI    R10,R5,15,
    ADD     R11,R10,R5,
    BNE     R10,R5,lab1,
    ADDI    R2,R2,1,
    jump    loop,
exit,
lab:
  ADDI    R6,R1,10,
exit,
lab1:
    ADDI    R8,R8,8,
exit,